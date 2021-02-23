// Server Recv
// Server의 경우 Raspberry에게 보낼 Command와 DB에 저장할 Image, Txt파일을 받는다.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

char * RecvName(int socket){
	int clnt_sock = socket;
	char *msg = NULL;
	int str_len = 0;
	char *name = NULL;

	while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0){
		if (str_len != -1){
			name = (char*)malloc(strlen(msg)+1);
			// 그냥 이름을 받아와서 리턴
			// msg 길이 파악 + 1 NULL
			sprintf(name, "%s", msg);
			return name;
		}
	}
	return NULL;
}