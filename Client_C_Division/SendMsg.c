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

void SendMsg(int socket, char *name, char *message);
void error_handling(char * msg);

void SendMsg(int socket, char *name, char *message)   // send thread main
{
	int sock= socket;
	char name_msg[NAME_SIZE+BUF_SIZE];

	printf(" %d, %s, %s \n", sock, name, message);
	
	if(message == "Start"){
		printf("들어왔니??");
		sprintf(name_msg,"%s %s", name, "1");
		write(sock, name_msg, strlen(name_msg));
	}
	else{
		write(sock, "error", strlen("error"));
		printf("여기야?????");
	}

	return NULL;
}

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}