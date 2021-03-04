// Camera CloseSocket
// 카메라의 Socket을 닫아준다.
// 완료

// 소켓을 받아와서 종료
int EndSocket(int sock)
{
	int socket=sock;
	close(socket);  
	return 0;
}