// Client
// 완료

// 소켓을 받아와서 종료
int EndSocket(int socket)
{
	close(socket);
	return 0;
}