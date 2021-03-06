#include "./ReadJsonAndInsertData.h"
#include "../DB/DB_MakeCsv.h"

#define _CRT_SECURE_NO_WARNINGS

#define BUF_SIZE 100
#define MAX_CLNT 3
#define PORT "8080"

#define DB_HOST "127.0.0.1"
#define DB_USER "bit"
#define DB_PASS "1234"
#define DB_NAME "Bit"

// Server
void * handle_clnt(void * arg);
void CommandSend_To_Raspberry(char * msg, int len);
void FileSend_To_Client(char * msg, int socket);
int RcvFlie(int socket, char * FileName);
int SendFile(int socket, char *FileName);

int Client = 0;	// 0
int Raspberry_pi = 0;	// 1
int Jetson = 0;	// 2

char * ImagePath;

// 서버에 접속한 클라이언트와 라즈베리의 idx를 관리하는 변수
int clnt_cnt = 0;

// 서버에 접속한 클라이언트와 라즈베리들의 소켓번호를 저장하는 배열
int clnt_socks[MAX_CLNT];

pthread_mutex_t mutx;	// 쓰레드를 안정적으로 관리할 mutex

MYSQL * connection = NULL;


int ServerOpen(MYSQL * connec)
{
	connection = connec;
	// server open에 이용될 변수들
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	soclen_t optlen;
	int option;
  
	pthread_mutex_init(&mutx, NULL);	// mutex 초기화
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);	// 서버는 IPv4(PF_INET) 프로토콜을 사용할거고 연결지향형(SOCK_STREAM) 소켓을 사용할거야

	optlen = sizeof(option);
	option = 1;
	setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);

	// 넘겨받은 PORT번호로 서버 OPEN
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(PORT));
	
	// bind와 listen을 통해 client받을준비 완료
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		return 0;
	if(listen(serv_sock, 5)==-1)
		return 0;

	while(1){
		// 접속을 요청한 Client, Rasberry 접속 허용 및 소켓 지정
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);

		pthread_mutex_lock(&mutx);

		clnt_socks[clnt_cnt] = clnt_sock;

		if (clnt_cnt == 0){Client = clnt_sock;}
		else if (clnt_cnt == 1){Raspberry_pi = clnt_sock;}
		else{Jetson = clnt_sock;}

		clnt_cnt++;

		pthread_mutex_unlock(&mutx);
	
		// 추가된 클라이언트에게 서비스를 제공하기 위한 쓰레드를 생성
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		// pthread_detach 함수호출을 통해서 종료된 쓰레드가 메모리에서 완전히 소멸되도록 하고 있다.
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}

	close(serv_sock);
	return 0;
}

void * handle_clnt(void * arg)
{
	int clnt_sock=*((int*)arg);
	int str_len=0, i;
	char msg[BUF_SIZE];
	
	if (clnt_sock == Client){	// Client (즉, GUI에서 받은거 Raspvberry에게)
		while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
		CommandSend_To_Raspberry(msg, str_len);
	}
	else{	// Jetson
		while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)
		FileSend_To_Client(msg, clnt_sock);
	}
	
	close(clnt_sock);
	return NULL;
}

void CommandSend_To_Raspberry(char * msg, int len)	// Raspberry에게 메세지 전달
{
	pthread_mutex_lock(&mutx);
	write(clnt_socks[1], msg, len);
	pthread_mutex_unlock(&mutx);
}

void FileSend_To_Client(char * msg, int socket)   // 클라이언트에게 파일을 전송
{
	// 파일을 받고 DB에 저장하고 Client에게 보내기
	pthread_mutex_lock(&mutx);
	if (msg[0] == '1'){
		RcvFlie(socket, "./SaveFile/Json/Recv.json");
	}
	else if (msg[0] == '2'){
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);

		char name[100] = 0;
		sprintf(name, "./SaveFile/Image/%d.%d.%d.%d.%d.%d_Recv.png",
			tm.tm_year - 100, tm.tm_mon+1, tm.tm_mday,
			tm.tm_hour, tm.tm_min, tm.tm_sec);

		RcvFlie(socket, name);
		ImagePath = name;
	}
	else{	// 파일을 모두 받았다
		ReadJson(connection, ImagePath);
		MakeCsv(connection);
		SendFile(clnt_socks[0], "/var/lib/mysql-files/orders.csv");

		remove("./SaveFile/Json/Recv.json");
		remove("/var/lib/mysql-files/orders.csv");
	}
	
	pthread_mutex_unlock(&mutx);
}

int RcvFlie(int socket, char * FileName)
{
	int clnt_sock = socket;	// 클라이언트 소셋
	char buf[256];	// 받을 메세지

	int nbyte = 256;
    size_t filesize = 0, bufsize = 0;
    FILE *file = NULL;

    file = fopen(FileName, "wb");

    bufsize = 256;

    while(nbyte!=0) {
        nbyte = recv(clnt_sock, buf, bufsize, 0);
        fwrite(buf, sizeof(char), nbyte, file);		
    }

	// 파일과 소켓을 닫아준다
	fclose(file);
	return 0;
}

void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}

int SendFile(int socket, char *FileName){
    // 소켓의 정보를 받아온다.
    int sock= socket;

    FILE* file = NULL;
    char buf[BUFSIZ];

    size_t fsize, nsize = 0;
    
	file = fopen(FileName, "rb");
	
    /* 파일 크기 계산 */
    // move file pointer to end
	fseek(file, 0, SEEK_END);
	// calculate file size
	fsize=ftell(file);
	// move file pointer to first
	fseek(file, 0, SEEK_SET);

	// send file contents
	while (nsize!=fsize) {
		int fpsize = fread(buf, 1, 256, file);

		nsize += fpsize;
		send(sock, buf, fpsize, 0);
	}	

	// 파일 닫아주기
	fclose(file);
    
	return 0;
}