// 다수의 lterative Client

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char *argv[]){

    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_addr;
    

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    // 소켓생성 ( 전화기 생성 )
    sock = socket(PF_INET, SOCK_STREAM, 0);

    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // connect 함수
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
        error_handling("connect() error");
    }
    else{
        puts("Connected.............");
    }

    // read함수를 반복 호출되고 호출될때마다 1바이트씩 데이터를 읽어드린다.
    while(1){
        fputs("Input message(Q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")){
            break;
        }

        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF_SIZE-1);
        message[str_len] = 0;
        printf("Message From server: %s", message);
    }

    printf("Message from server : %s \n", message);
    printf("Function read call count : %d \n", str_len);
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

// 문제점 // 

// write(sock, message, strlen(message));
// str_len = read(sock, message, BUF_SIZE-1);
// message[str_len] = 0;
// printf("Message From server: %s", message);

// read, write 함수가 호출될 떄마다 문자열 단위로 실제 입출력이 이뤄진다.
// 즉, 둘 이상의 write 함수호출로 전달된 문자열 정보가 묶여서 한번에 서버에 전송될 수 있다.
// 클라이언트 또한 둘 이상의 문자열 정보를 서버로부터 돌려받아 원하는 결과값을 얻지 못할 수 있다.

// 이는 클라이언트가 수신해야 할 데이터 크기만큼 수신될때까지 기다리면 해결이 가능하다.