// Client
// 완료

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>


// IP와 PORT 받아와서 소켓 열기
int main()
{

	printf("hello world\n");

	int sock = 0;
	int a = 0;
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	a=socket(PF_INET, SOCK_STREAM, 0);

	printf("%d, %d \n", sock, a);
	

	return 0;
}

