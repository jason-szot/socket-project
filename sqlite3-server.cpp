#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <cstring>
#include <unistd.h>
#include <sqlite3.h>
#include <fstream>
#include <iostream>

using namespace std;

#define MAXLINE 4096	/* max text line length*/
#define SERV_PORT 9790	/* server port  */
#define LISTENQ 8		/* maximum number of client connections*/

int main(int argc, char **argv)
{
	int listenfd, connfd, n, rc;
	pid_t childpid;
	sqlite3 *db;
	socklen_t clilen;
	char buf[MAXLINE];
	struct sockaddr_in cliaddr, servaddr;
	std::ofstream slog;

	//Create a socket for the socket
	//If sockfd<0 there was an error in the creation of the socket
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) <0) {
		perror("Problem in creating the socket");
		exit(2);
	}

	//preparation of the socket address
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	//bind the socket
	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

	//listen to the socket by creating a connection queue, then wait for clients
	listen(listenfd, LISTENQ);
	printf("%s\n", "Server running...waiting for connections.");

	for (;;) {
		clilen = sizeof(cliaddr);
		//accept a connection
		connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
		printf("%s\n", "Received request...");

		if ((childpid = fork()) == 0) {       //if it’s 0, it’s child process
			printf("%s\n", "Child created for dealing with client requests");
			//close listening socket
			close(listenfd);

			while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)  {
				printf("%s", "Command received from the client:");
				puts(buf);
				// place your code to do the server part here and send the result.
				// log the request to a file
				slog.open("server_log.txt", std::ios::app);
				time_t ltime;
				ltime = time(NULL);
				slog << asctime( localtime(&ltime)) << "\n" << "command received: \n" << buf << "\n";
				slog.close();
				//handle request
				char* reply = (char*)calloc(MAXLINE,sizeof(char));
				rc = sqlite3_open("music.db", &db);
				if (rc){
					fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
					exit(0);
				}
				else{
					fprintf(stdout, "Opened database successfully\n");
				}

				// prepare sql statement and output data to ofstream;

				sqlite3_stmt *statement;

				if (sqlite3_prepare(db, buf, -1, &statement, 0) == SQLITE_OK)
				{
					int ctotal = sqlite3_column_count(statement);
					int res = 0;
					while (1)
							{
								res = sqlite3_step(statement);
								if (res == SQLITE_ROW)
								{
									for (int i = 0; i < ctotal; i++)
									{
										strcat(reply, (char*)sqlite3_column_text(statement, i));

										// output code here with formatting
										if (i < ctotal - 1){
										strcat(reply, " | ");
										}
									}
									strcat(reply, "\n");
								}
								if (res == SQLITE_DONE || res == SQLITE_ERROR)
								{
								break;
								}
							}	// end of while
				}	// end of if prepare
				sqlite3_close(db);
				send(connfd, reply, strlen(reply), 0);
				free(reply);
				// clear buf
				for ( int i = 0; i < MAXLINE; i++){
					buf[i] = '\0';
				}
			}
			if (n < 0)
				printf("%s\n", "Read error");
			exit(0);
		}
		close(connfd);
	}
}
