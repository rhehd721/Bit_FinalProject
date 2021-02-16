// Client
// 완료

// 소켓을 받아와서 종료
int EndSocket(int socket)
{
	fputs("END SOCKT\n", stderr);
	close(socket);
	fputs("ENDEND\n", stderr);
	return 0;
}