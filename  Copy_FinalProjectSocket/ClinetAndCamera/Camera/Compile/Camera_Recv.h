// Camera Recv
// 카메라의 경우 명령어만 받으므로 그에따라 작성한다.
// 완료
#define BUF_SIZE 100
#define NAME_SIZE 20

#include "./Common_Send.h"

void RecvCommand(int sock)
{
	// void * sock
	int socket=sock;
	char Command[NAME_SIZE+BUF_SIZE];
	int str_len;
	
	// 명령을 받은경우 명령을 실행하는 부분
	while(1)
	{
		str_len=read(socket, Command, NAME_SIZE+BUF_SIZE-1);
		if(str_len != -1){
			printf("%s", Command);
			// Camera가 받는 명령어를 추가해준다.
			if (*((int*)Command) == "1"){
				fputs("Start \n", stderr);
				Send(socket, 1, "", 0);
				Send(socket, 3, "./dog.jpg", 0);
			}
			else if (*((int*)Command) == "2"){
				fputs("Stop \n", stderr);
			}
			else if (*((int*)Command) == "3"){
				fputs("Left \n", stderr);
			}
			else if (*((int*)Command) == "4"){
				fputs("Right \n", stderr);
			}
			else if (*((int*)Command) == "5"){
				fputs("Up \n", stderr);
			}
			else if (*((int*)Command) == "6"){
				fputs("Down \n", stderr);
			}
			// Thread를 빠져나오는 부분
			else{
				fputs("???????? \n", stderr);;
			}
		}
	}
}