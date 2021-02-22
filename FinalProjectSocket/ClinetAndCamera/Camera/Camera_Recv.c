/* server.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void RecvCommand(int socket)
{
	char Command[NAME_SIZE+BUF_SIZE];
	int str_len;
	
	// 명령을 받은경우 명령을 실행하는 부분
	while(1)
	{
		str_len=read(socket, Command, NAME_SIZE+BUF_SIZE-1);
		if(str_len != -1){
			if (Command == "Start"){
				break;
			}
			else if (Command == "Stop"){
				break;
			}
		}
	}
}