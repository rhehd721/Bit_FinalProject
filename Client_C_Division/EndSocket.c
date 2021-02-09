// Client

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>


int EndSocket(void * arg)
{
	int sock=*((int*)arg);
	close(sock);  
	return 0;
}