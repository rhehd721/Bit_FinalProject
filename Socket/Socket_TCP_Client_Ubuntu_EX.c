// TCP 소켓의 특성인 전송되는 데이터의 경계(Boundary)가 존재하지 않는다
// 라는 특성을 확인해보기 위한 코드

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[]){

    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;
    int idx = 0, read_len = 0;

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

    // read함수를 반복 호출되고 호출될때마다 1바이트씩 데이터를 읽어드린다.
    while(read_len = read(sock, &message[idx++], 1)){
        if(str_len == -1){
            error_handling("read() error");
        }
        // 읽어들인 바이트 저장
        str_len += read_len;
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