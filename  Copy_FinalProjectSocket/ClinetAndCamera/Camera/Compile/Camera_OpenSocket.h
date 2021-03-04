// Camera
// Camera와 Server를 연결해주는 코드
// 완료

#define IP_Num "127.0.0.1"
#define PORT_Num "8080"

// Server의 IP와 PORT 받아와서 소켓 열기
int OpenSocket()
{
	int sock;
	struct sockaddr_in serv_addr;
	
	sock=socket(PF_INET, SOCK_STREAM, 0);
	
	// 주소 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(IP_Num);
	serv_addr.sin_port=htons(atoi(PORT_Num));

	while(1){
		// 소켓을 이용해 서버의 정보를 지닌 구조체를 가지고 접속 요청을 한다
		if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1){
			sleep(30);
		}
		// 접속이 되었다면 탈출
		else{
			break;
		}
	}
	
	return sock;
}