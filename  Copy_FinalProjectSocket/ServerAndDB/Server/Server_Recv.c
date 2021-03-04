// Server Recv
// Server의 경우 Raspberry에게 보낼 Command와 DB에 저장할 Image, Txt파일을 받는다.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int Recv(int socket);
int RcvFlie(int socket, int Type, char * FileName);

int Recv(int socket){
	int clnt_sock = socket;
	char *msg = NULL;
	int str_len = 0;

	// 1단계 : 보낸사람 확인
	// 2단계 : 보낼 파일 확인
	// 3단계 : 파일 or 커멘드 받기
	while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0){
		if (str_len != -1){
			if (msg == "Command"){	// 해당 경우는 무조건 Client가 보낸 것
				// Command를 받는 경우는 Raspberry에게 명령 전달
				while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
				if (str_len != -1){
					
				}
			}
			// 아래의 두 경우는 Camera가 보낸 것
			else if (msg == "Txt"){	
				while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
				if (str_len != -1){
					RcvFlie(socket, 0, "아직 못정함");
				}
			}
			else if (msg == "Imgae"){
				while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
				if (str_len != -1){
					RcvFlie(socket, 1, "아직 못정함");
				}
			}
			else{	// 예외처리
				return -1;
			}
		}
	}
	return 0;
}

int RcvFlie(int socket, int Type, char * FileName)
{
	int clnt_sock;	// 클라이언트 소셋
	char buf[256];	// 받을 메세지

	int nbyte = 256;
    size_t filesize = 0, bufsize = 0;
    FILE *file = NULL;

	if (Type == 0){
    	file = fopen(FileName, "wb");
	}
	else if (Type == 1){
		file = fopen(FileName, "wt");
	}
	else{
		return -1;
	}

    bufsize = 256;

    while(/*filesize != 0*/nbyte!=0) {
 		//if(filesize < 256) bufsize = filesize;
        nbyte = recv(clnt_sock, buf, bufsize, 0);
		// int recv(int s, void *buf, size_t len, int flags);
		// int s	: 소켓 디스크립터
		// void *buf	: 수신할 버퍼 포인터 데이터
		// size_t len	: 버퍼의 바이트 단위 크기
		// int flags	: 아래와 같은 옵션을 사용할 수 있습니다.

        fwrite(buf, sizeof(char), nbyte, file);		
        //nbyte = 0;
    }
 

	// 파일과 소켓을 닫아준다
	fclose(file);
	return 0;
}