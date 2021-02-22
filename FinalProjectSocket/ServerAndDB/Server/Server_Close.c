// Server

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

int Server_Close(int Socket)
{
	int serv_sock = Socket;
	close(serv_sock);
	return 0;
}