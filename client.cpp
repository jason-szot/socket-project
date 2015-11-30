#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <fstream>
#include <iostream>


#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 9790 /*port*/

int
main(int argc, char **argv) 
{
 int sockfd;
 std::ofstream output;
 struct sockaddr_in servaddr;
 char *sendline, *recvline;
 time_t ltime;
 sendline = (char*)calloc(MAXLINE,sizeof(char));

	
 //basic check of the arguments
 //additional checks can be inserted
 if (argc !=2) {
  perror("Usage: TCPClient <IP address of the server"); 
  exit(1);
 }
	
 //Create a socket for the client
 //If sockfd<0 there was an error in the creation of the socket
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  perror("Problem in creating the socket");
  exit(2);
 }
	
 //Creation of the socket
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr(argv[1]);
 servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order
	
 //Connection of the client to the socket 
 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  perror("Problem in connecting to the server");
  exit(3);
 }
	int choice = 0;
  char *opt;
 while (1) {
      printf("enter 9 to quit");
      printf("\nenter your command: ");
      fgets(sendline, MAXLINE, stdin);
      if (atoi(sendline) == 9)
      {
      return 0;
      }
	                 //fgets(sendline, MAXLINE, stdin) != NULL
  ltime = time(NULL);
  output.open("client_log.txt", std::ios::app);
  output << asctime( localtime(&ltime))  << "command sent:" << "\n" << sendline<< "\n";
  output.close();
  send(sockfd, sendline, strlen(sendline), 0);
  recvline = (char*)calloc(MAXLINE,sizeof(char));		
  if (recv(sockfd, recvline, MAXLINE,0) == 0){
   //error: server terminated prematurely
   perror("The server terminated prematurely"); 
   exit(4);
  }
  printf("%s%s", "String received from the server: \n", recvline);
  output.open("client_log.txt", std::ios::app);
  ltime = time(NULL);
  output << asctime( localtime(&ltime)) << "reply recieved:" << "\n" << recvline << "\n";
  output << "---------------------------------------\n";
  output.close();
  free(recvline);

  // empty the sendline array
  for ( int i = 0; i < 800; i++){
    sendline[i] = '\0';
  }
 }

 exit(0);
}