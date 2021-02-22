// Client
// Clinet의 경우 TxtFile과 ImageFile을 받으므로 그에 맞게 코드를 작성한다.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void Recv(int socket)   // read thread main
{
	char Command[NAME_SIZE+BUF_SIZE];
	int str_len;
	while(1)
	{
		str_len=read(socket, Command, NAME_SIZE+BUF_SIZE-1);
		if(str_len != -1){
            if (Command == "image"){
				break;
			}
			else if (Command == "txt"){
				break;
			}
			// Thread를 빠져나오는 부분
			else{
				break;
			}
            }
	}
}

int RecvFile(int soket)
{
	char buf[256];	// 받을 메세지

	int byte1 = 0;
    int byte2 = 0;
    size_t filesize = 0, bufsize = 0;
    FILE *file = NULL;

	// 쓰기모드로 바이너리 파일 열기
    file = fopen("aurora.jpg"/* 새로 만들 파일 이름 */, "wb");

    bufsize = 256;
 

	while((byte1 = recv(soket, buf, bufsize, 0) != 0)){
 		fwrite(buf, sizeof(char), byte1, file);
	}


	// 파일과 소켓을 닫아준다
	fclose(file);

	return 0;
}