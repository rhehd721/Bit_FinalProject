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

int SendFile(int socket){
    // 소켓의 정보를 받아온다.
    int sock= socket;

    FILE* file = NULL;
    char buf[BUFSIZ];

    size_t fsize, nsize = 0;
	size_t fsize2;
    
    /* 전송할 파일 이름을 작성합니다 */
	file = fopen("test.txt" /* 파일이름 */, "rb");
	// 텍스트 모드로 열지 바이너리 모드로 열지 rt, wt, at 텍스트 모드, rb, wb, ab 바이너리모드 이렇게 옵션
	
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