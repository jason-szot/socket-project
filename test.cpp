#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

int main(){
	struct stat st = {0};
	printf("%s\n", "testing file I/O");
	if (stat("log/",&st) == -1){
		mkdir("log/", 0777);
	}
	std::ofstream output;
	time_t ltime;
	ltime = time(NULL);
	char *dir;
	char *curr_time = asctime( localtime(&ltime));
	dir = (char*)calloc(1024,sizeof(char));
	strcat(dir,"log/");
	output.open("log/test1.txt");
	output.close();
	strcat(dir,curr_time);
	strcat(dir,".txt");
	output.open(dir);
	output.close();
	return 0;
}