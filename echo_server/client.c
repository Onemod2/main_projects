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
	SockAddr.sin_port = htons(1234);
	SockAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	while (1) {	
		
		connect(SlaveSocket, (struct sockaddr*)(&SockAddr), sizeof(struct sockaddr_in));
		char buffer[512], answer[512];
		scanf("%s", buffer);

		send(SlaveSocket, buffer, 512, MSG_NOSIGNAL);
		recv(SlaveSocket, answer, 512, MSG_NOSIGNAL);

		printf("%s\n", answer);
		shutdown(SlaveSocket, SHUT_RDWR);
		close(SlaveSocket);
		
	}
	
}





