/* client.c */


#define BUF_SIZE 100
#define NAME_SIZE 20

void SendCommand(int socket, char *name, char *message);
int SendFile(int socket, int Type);


// 소켓정보와 이름과 메세지를 받아 Command를 보낸다.
void SendCommand(int socket, char *name, char *message)   // send thread main
{
	fputs("SEND START\n", stderr);
	int sock= socket;
	char name_msg[NAME_SIZE+BUF_SIZE];
	
    sprintf(name_msg,"%s_%s", name, "1");
    write(sock, name_msg, strlen(name_msg));

	fputs("SEND END\n", stderr);
}

int SendFile(int socket, int Type){
    // 소켓의 정보를 받아온다.
    int sock= socket;

    FILE* file = NULL;
    char buf[BUFSIZ];

    size_t fsize, nsize = 0;
    
    /* 전송할 파일 이름을 작성합니다 */
    // Image 경우 1, Txt경우 0
    if (Type == 1){
	    file = fopen("dog.jpg" /* 파일이름 */, "rb");
	}
    else{
        file = fopen("dog.txt" /* 파일이름 */, "rt");
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
