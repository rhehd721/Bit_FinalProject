// Server
// 완료

#include <sys/socket.h>
int Server_Close(int Socket)
{
	int serv_sock = Socket;
	close(serv_sock);
	return 0;
}