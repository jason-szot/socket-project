#include <sys/time.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

int main(){
	time_t ltime;
	ltime = time(NULL);
	printf("%s",asctime( localtime(&ltime)));
	return 0;
}