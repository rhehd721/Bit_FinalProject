/* server.c */

#define BUF_SIZE 100
#define NAME_SIZE 20

void RecvCommand(int socket)   // read thread main
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
			// 
			else{
				break;
			}
            }
	}
}


// int Recv(int soket)
// {
// 	char buf[256];	// 받을 메세지

// 	int byte1 = 0;
//     int byte2 = 0;
//     size_t filesize = 0, bufsize = 0;
//     FILE *file = NULL;

// 	// 쓰기모드로 바이너리 파일 열기
//     file = fopen("aurora.jpg"/* 새로 만들 파일 이름 */, "wb");

//     bufsize = 256;
 

// 	while((byte1 = recv(soket, buf, bufsize, 0) != 0)){
//  		fwrite(buf, sizeof(char), byte1, file);
// 	}


// 	// 파일과 소켓을 닫아준다
// 	fclose(file);

// 	return 0;
// }