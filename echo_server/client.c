#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int SlaveSocket = socket(
			AF_INET,
			SOCK_STREAM,
			IPPROTO_TCP);

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = 1234;
	SockAddr.sin_addr.s_addr = htons(INADDR_ANY);

	bind(SlaveSocket, (struct sockaddr*)(&SockAddr), sizeof(struct sockaddr_in));
	
	char buffer[255];
 	scanf("%s", buffer);
	buffer[254] = '\0';

	send(SlaveSocket, buffer, 4, MSG_NOSIGNAL);
	recv(SlaveSocket, buffer, 4, MSG_NOSIGNAL);

	buffer[4] = '\0';
	printf("%s\n", buffer);

	shutdown(SlaveSocket, SHUT_RDWR);
	close(SlaveSocket);
	
}





