// Client

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

void main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	// // 주소 초기화
	// memset(&serv_addr, 0, sizeof(serv_addr));
	// serv_addr.sin_family=AF_INET;
	// serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	// serv_addr.sin_port=htons(atoi(argv[2]));
	
	// // 소켓을 이용해 서버의 정보를 지닌 구조체를 가지고 접속 요청을 한다
	// connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)
	
    printf("%s", sock);

	close(sock);  
	return 0;
}
