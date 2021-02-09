// Client

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void * send_msg(void * arg);
void error_handling(char * msg);

char msg[BUF_SIZE];

void * send_msg(void * arg, char *name, char *message)   // send thread main
{
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	
	if(*message == "Start"){
		sprintf(name_msg,"%s %s", *name, "1");
		write(sock, name_msg, strlen(name_msg));
	}
	else{
		write(sock, "error", strlen("error"));
	}

	return NULL;
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}