// Client and Raspberry Send
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

void Send(int server_socket, int Order, char * FileName, int check);
int SendFile(int server_socket, int Type, char *FileName);


// 소켓정보와 이름과 메세지를 받아 Command를 보낸다.
// Order 0. 라즈베리정보, 1. Command, 2. Txt파일, 3. image파일 
void Send(int server_socket, int Order, char * FileName, int check)
{
	int sock= server_socket;
	char Command[BUF_SIZE];
	
	if (Order == 1){
		if(check == 0){	// image
			sprintf(Command,"%s", "2");
		}
		else{	// Txt
			sprintf(Command,"%s", "1");
		}
		write(sock, Command, strlen(Command));
	}
	else{
		SendFile(sock, Order, FileName);
	}
    
}

int SendFile(int server_socket, int Type, char *FileName){
    // 소켓의 정보를 받아온다.
    int sock= server_socket;

    FILE* file = NULL;
    char buf[BUFSIZ];

    size_t fsize, nsize = 0;
    
    /* 전송할 파일 이름을 작성합니다 */
    // Txt경우 1, Image는 나머지 경우
    if (Type == 2){
		file = fopen(FileName, "rt");
	}
	else if (Type == 3){
		file = fopen(FileName, "rb");
	}
    else{
        printf("error");
	}
	
    /* 파일 크기 계산 */
    // move file pointer to end
	fseek(file, 0, SEEK_END);
	// calculate file size
	fsize=ftell(file);
	// move file pointer to first
	fseek(file, 0, SEEK_SET);


	// send file contents
	while (nsize!=fsize) {
		// read from file to buf
		// 1byte * 256 count = 256byte => buf[256];
		int fpsize = fread(buf, 1, 256, file);
		// size_t fread (void * DstBuf, size_t ElementSize, size_t Count, FILE * FileStream)
		// DstBuf : 입력받은 데이터를 저장할 버퍼의 주소
		// ElementSize : 원소 1개의 크기
		// Count : 입력 받을 원소의 개수
		// FileStream : 파일 스트림

		nsize += fpsize;
		send(sock, buf, fpsize, 0);
	}	

	// 파일 닫아주기
	fclose(file);
    
	return 0;
}
