#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <netinet/in.h>
#include <string.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>


int set_nonblock(int fd) {
	int flags;
#if defined(O_NONBLOCK)
	if (-1 == (flags = fcntl(fd, F_GETFL, 0))) {
		flags = 0;
	}
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(fd, FIOBIO, &flags);
#endif
}

int main() {
	int SlaveSocket = socket(
			AF_INET,
			SOCK_STREAM,
			IPPROTO_TCP);

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(0);
	SockAddr.sin_addr.s_addr = htonl(2130706452);

	bind(SlaveSocket, (struct sockaddr*)(&SockAddr),
		sizeof(struct sockaddr_in));

	struct sockaddr_in SockAddrIn;	
	SockAddrIn.sin_family = AF_INET;
	SockAddrIn.sin_port = htons(1234);
	SockAddrIn.sin_addr.s_addr = inet_addr("195.208.1.107");

	connect(SlaveSocket, (struct sockaddr*)(&SockAddrIn),
		 sizeof(struct sockaddr_in));

	unsigned long long v = 0;
	
	while (1) {	
		char answer[512];

		char buffer[20];
		sprintf(buffer, "%llu", v);		

		send(SlaveSocket, buffer, strlen(buffer), MSG_NOSIGNAL);
		
		//int current_size = recv(SlaveSocket, answer,
	//			511, MSG_NOSIGNAL);

		//answer[current_size] = '\0';

		printf("%s\n", answer);

		v++;
	}

	shutdown(SlaveSocket, SHUT_RDWR);
	close(SlaveSocket);
	
}





