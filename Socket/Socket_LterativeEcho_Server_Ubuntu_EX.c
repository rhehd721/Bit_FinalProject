// 다수의 Client에게 echo서비스를 제공하는 server

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[]){

    int serv_sock;
    int clnt_sock;

    char message[BUF_SIZE];
    int str_len, i;

    struct sockaddr_in serv_adr;
    struct sockaddr_in clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 소켓생성 ( 전화기 생성 )
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    // 소켓번호 지정 ( 전화기 번호지정 )
    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind() error");
    }
    // 통신받을 준비 ( 전화기 수신준비 완료 )
    if(listen(serv_sock, 5)==-1){
        error_handling("listen() error");
    }

    clnt_adr_sz = sizeof(clnt_adr);

    for (i = 0; i <5; i++){
        // 5개의 client 요청이 오면 수락할 준비
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1){
            error_handling("accept() error");
        }
        else{
            printf("Connected client %d \n", i+1);
        }

        // echo 서비스 동작 부분
        while((str_len=read(clnt_sock, message, BUF_SIZE))!=0){
            write(clnt_sock, message, str_len);
        }

        close(clnt_sock);

    }
    // 5개의 client 서비스 제공 후 서버소켓 종료
    close(serv_sock);
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}