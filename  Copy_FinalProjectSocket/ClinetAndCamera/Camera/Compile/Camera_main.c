#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>

#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "./Camera_OpenSocket.h"
#include "./Camera_Recv.h"
#include "./Camera_CloseSocket.h"

#define IP_Num "127.0.0.1"
#define PORT_Num "8080"

#define BUF_SIZE 100
#define NAME_SIZE 20

int main(){

    pthread_t t_id;

    int socket;

    socket = OpenSocket();

    RecvCommand(socket);

    // pthread_create(&t_id, NULL, RecvCommand, socket);
	
	// pthread_detach(t_id);

    EndSocket(socket);

}