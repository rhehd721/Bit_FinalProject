/* client.c */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

// error 함수
void error_handling(char *message);

int main(int argc, char* argv[]){

	int serv_sock, fd;
    int str_len, len;
	struct sockaddr_in serv_addr;
	char message[30], buf[BUFSIZ];
    FILE* file = NULL;
    
	// IP와 PORT 안알려주면 error 메세지
	if(argc!=3){
		printf("Usage : %s <IP> <PORT> \n", argv[0]);
		exit(1);
	}

	// serv_sock을 IPv4와 연결지향형으로 만들겠다 (소켓생성)
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
	// serv_sock이 생성되지 않으면 error 메세지
	if(serv_sock == -1){
		error_handling("socket() error");
    }

	// 주소정보 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));	// 서버소켓의 정보를 저장할 구조체를 초기화 한다.
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
    
	// 소켓을 이용해 서버의 정보를 지닌 구조체를 가지고 접속 요청을 한다
	if(connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
		error_handling("connect() error!");
	}
	
    // test
	str_len=read(serv_sock, message, sizeof(message)-1);
	// ssize_t read(int fd, void *buf, size_t nbytes);
	// int fd : 읽을 파일의 파일 디스크립터
 	// void *buf : 읽어들인 데이터를 저장할 버퍼(배열)
	// size_t nbytes : 읽어들일 데이터의 최대 길이 (buf의 길이보다 길어선 안됨)
	// 반환값 : 읽어들인 데이터의 길이 무조건 nbytes 가 리턴되는 것은 아님. 중간에 파일의 끝을 만난다면 거기까지만 읽기 때문


	if(str_len==-1){
		error_handling("read() error!");
	}
	printf("Message from server: %s \n", message);
    
	// jpg 
	size_t fsize, nsize = 0;
	size_t fsize2;
    
    /* 전송할 파일 이름을 작성합니다 */
	file = fopen("aurora.jpg" /* 파일이름 */, "rb");
	// 텍스트 모드로 열지 바이너리 모드로 열지 rt, wt, at 텍스트 모드, rb, wb, ab 바이너리모드 이렇게 옵션
	
    /* 파일 크기 계산 */
    // move file pointer to end
	fseek(file, 0, SEEK_END);
	// calculate file size
	fsize=ftell(file);
	// move file pointer to first
	fseek(file, 0, SEEK_SET);

	// send file size first
	// fsize2 = htonl(fsize);
	// send file size
	// send(serv_sock, &fsize2, sizeof(fsize), 0);

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
		send(serv_sock, buf, fpsize, 0);
	}	

	// 소켓과 파일 닫아주기
	fclose(file);
	close(serv_sock);
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}