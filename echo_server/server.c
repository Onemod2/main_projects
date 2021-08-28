#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int MainSocket = socket(
			AF_INET,
			SOCK_STREAM,
			IPPROTO_TCP);

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(1234);
	SockAddr.sin_addr.s_addr = htons(INADDR_ANY);
	
	bind(MainSocket, (struct sockaddr*)(&SockAddr), sizeof(struct sockaddr_in));

	listen(MainSocket, SOMAXCONN);
	printf("Start\n");

	struct sockaddr_in SockAddrClient;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	int SlaveSocket = accept(MainSocket, (struct sockaddr*)(&SockAddrClient), &addrlen);

	while (1) {
		printf("new_ip=%u\n", SockAddrClient.sin_addr.s_addr);
		char buffer[512];

		if (0 == recv(SlaveSocket, buffer, 512, MSG_NOSIGNAL)) break;
		if (-1 == send(SlaveSocket, buffer, 512, MSG_NOSIGNAL)) break;
	}	
	shutdown(SlaveSocket, SHUT_RDWR);
	close(SlaveSocket);
	
	return 0;
}





