// Client
// 아직 완성되지 않은 코드입니다.

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

// void * send_msg(void * arg);
// void * recv_msg(void * arg);
void * send_Flie(void * arg)
void error_handling(char * msg);

char name[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread, snd_file;
	void * thread_return;

	// IP와 PORT 안알려주면 error 메세지
	if(argc!=3) {
		printf("Usage : %s <IP> <port> \n", argv[0]);
		exit(1);
	 }
	// serv_sock을 IPv4와 연결지향형으로 만들겠다 (소켓생성)
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	// 주소 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	
	// 소켓을 이용해 서버의 정보를 지닌 구조체를 가지고 접속 요청을 한다
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
		error_handling("connect() error");
	}
	
	// pthread_create(&snd_thread, NULL, send_msg, (void*)&sock);
	// pthread_create(&rcv_thread, NULL, recv_msg, (void*)&sock);
	// int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine)(void*), void *arg)
	// 1. thread : 성공적으로 함수가 호출되면 이곳에 thread ID가 저장됩니다. 이 인자로 넘어온 값을 통해서 pthread_join과 같은 함수를 사용할 수 있습니다.
	// 2. attr : 스레드의 특성을 정의합니다. 기본적으로 NULL을 지정합니다. 만약 스레드의 속성을 지정하려고 한다면 pthread_attr_init등의 함수로 초기화해야합니다.
	// 3. start_routine : 어떤 로직을 할지 함수 포인터를 매개변수로 받습니다. 
	// 4. arg : start_routine에 전달될 인자를 말합니다. start_routine에서 이 인자를 변환하여 사용합니다.
	pthread_create(&snd_file, NULL, send_Flie, (void*)&sock);

	// pthread_join(snd_thread, &thread_return);
	// pthread_join(rcv_thread, &thread_return);
	// int pthread_join(pthread_t th, void **thread_return);
	// 특정 쓰레드가 종료하기를 기다렸다가, 쓰레그가 종료된 이후 다음 진행
	// 1. pthread_t th : 쓰레드의 ID를 받아 그 ID가 종료될때까지 실행을 지연시킨다
	// 2. void **thread_return : 쓰레드 종료시 반환값을 받는다.
	// 3. 반환값 : 성공할경우 쓰레드식별자인 thread에 쓰레드 식별번호를 저장하고, 0을 리턴한다. 실패했을경우 0 이 아닌 에러코드 값을 리턴한다.
	pthread_join(snd_file, &thread_return);

	close(sock);  
	return 0;
}

void * send_msg(void * arg)   // send thread main
{
	int sock=*((int*)arg);
	char name_msg[NAME_SIZE+BUF_SIZE];
	while(1) 
	{
		fgets(msg, BUF_SIZE, stdin);
		if(!strcmp(msg,"q\n")||!strcmp(msg,"Q\n")) 
		{
			close(sock);
			exit(0);
		}
		sprintf(name_msg,"%s %s", name, msg);
		write(sock, name_msg, strlen(name_msg));
	}
	return NULL;
}

// void * recv_msg(void * arg)   // read thread main
// {
// 	int sock=*((int*)arg);
// 	char name_msg[NAME_SIZE+BUF_SIZE];
// 	int str_len;
// 	while(1)
// 	{
// 		str_len=read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
// 		if(str_len==-1) 
// 			return (void*)-1;
// 		name_msg[str_len]=0;
// 		fputs(name_msg, stdout);
// 	}
// 	return NULL;
// }

void error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
// 파일을 보내는 함수
void * send_Flie(void * arg){
	int sock=*((int*)arg);
	
	// jpg 
	size_t fsize, nsize = 0;
	FILE* file = NULL;
	char buf[BUFSIZ];
    
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

	// 소켓과 파일 닫아주기
	fclose(file);

	return NULL;
}