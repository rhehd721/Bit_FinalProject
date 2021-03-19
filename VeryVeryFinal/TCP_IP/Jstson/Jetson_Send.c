// 완료

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

int Send(int server_socket, int Order, char * FileName, int check);
int SendFile(int server_socket, char *FileName);


// 소켓정보와 이름과 메세지를 받아 Command를 보낸다.
// Order 0. 라즈베리정보, 1. Command, 2. Txt파일, 3. image파일 
int Send(int server_socket, int Order, char * FileName, int check)
{
	int sock= server_socket;
	char Command[BUF_SIZE];
	
	if (Order == 1){
		sprintf(Command,"%d", check);
		write(sock, Command, strlen(Command));
	}
	else{
		SendFile(sock, FileName);
	}

	return 0;
    
}

int SendFile(int server_socket, char *FileName){
    // 소켓의 정보를 받아온다.
    int sock= server_socket;

    FILE* file = NULL;
    char buf[BUFSIZ];

    size_t fsize, nsize = 0;
    
	file = fopen(FileName, "rb");
	
    /* 파일 크기 계산 */
    // move file pointer to end
	fseek(file, 0, SEEK_END);
	// calculate file size
	fsize=ftell(file);
	// move file pointer to first
	fseek(file, 0, SEEK_SET);


	// send file contents
	while (nsize!=fsize) {
		int fpsize = fread(buf, 1, 256, file);

		nsize += fpsize;
		send(sock, buf, fpsize, 0);
	}	

	// 파일 닫아주기
	fclose(file);
    
	return 0;
}
