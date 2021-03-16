#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "/usr/include/mysql/mysql.h"

#include "./Server/Server_Open.h"
#include "./Server/Server_Close.h"

#include "./DB/DB_Connect.h"
#include "./DB/DB_Close.h"

#define BUF_SIZE 100
#define MAX_CLNT 3
#define PORT "8080"

#define DB_HOST "127.0.0.1"
#define DB_USER "bit"
#define DB_PASS "1234"
#define DB_NAME "Bit"

int main(){
    MYSQL * connection=NULL;
	connection = DB_Connect();  // DB 연결

    ServerOpen();   // Server 연결 및 관리






    DB_Close(connection);
    Server_Close(); // 종료
}