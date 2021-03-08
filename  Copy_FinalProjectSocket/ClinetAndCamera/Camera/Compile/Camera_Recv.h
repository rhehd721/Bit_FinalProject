// Camera Recv
// 카메라의 경우 명령어만 받으므로 그에따라 작성한다.
// 완료
#define BUF_SIZE 100
#define NAME_SIZE 20

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
			printf("%s", Command)
			// Camera가 받는 명령어를 추가해준다.
			if (Command == "1"){
				printf("Start");
			}
			else if (Command == "2"){
				printf("Stop");
			}
			else if (Command == "3"){
				printf("Left");
			}
			else if (Command == "4"){
				printf("Right");
			}
			else if (Command == "5"){
				printf("Up");
			}
			else if (Command == "6"){
				printf("Down");
			}
			// Thread를 빠져나오는 부분
			else{
				break;
			}
		}
	}
}