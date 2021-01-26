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

void error_handling(char *message);
int main(int argc, char* argv[]){

	int serv_sock, fd;
    	int str_len, len;
	struct sockaddr_in serv_addr;
	char message[30], buf[BUFSIZ];
    	FILE* file = NULL;
    
	if(argc!=3){
		printf("Usage : %s <IP> <PORT> \n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
	if(serv_sock == -1)
		error_handling("socket() error");
        
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
    
	if(connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) 
		error_handling("connect() error!");
	
    // test
	str_len=read(serv_sock, message, sizeof(message)-1);

	if(str_len==-1)
		error_handling("read() error!");
	printf("Message from server: %s \n", message);
    
	// jpg 
	size_t fsize, nsize = 0;
	size_t fsize2;
    
    /* 전송할 파일 이름을 작성합니다 */
	file = fopen("aurora.jpg" /* 파일이름 */, "rb");
	
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
		nsize += fpsize;
		send(serv_sock, buf, fpsize, 0);
	}	

	fclose(file);
	close(serv_sock);
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}