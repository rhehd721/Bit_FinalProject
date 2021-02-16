// Client
// 완료

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

// 소켓을 받아와서 종료
int EndSocket(void * arg)
{
	int sock=*((int*)arg);
	close(sock);  
	return 0;
}