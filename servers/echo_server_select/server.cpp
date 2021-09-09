#include <iostream>
#include <set>
#include <algorithm>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
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

int main(int argc, char **argv) {
	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	std::set<int> SlaveSockets;

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(1234);
	SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(MasterSocket, (struct sockaddr *)(&SockAddr), sizeof(sockaddr_in));

	set_nonblock(MasterSocket);

	listen(MasterSocket, SOMAXCONN);

	while (true) {
		fd_set Set;
		FD_ZERO(&Set);
		FD_SET(MasterSocket, &Set);
		
		for (auto current_socket : SlaveSockets) {
			FD_SET(current_socket, &Set);
		}
	
		int current_max = MasterSocket;
		if (SlaveSockets.size() > 0) {
			current_max = std::max(MasterSocket, *SlaveSockets.rbegin());	
		}
		
		select(current_max+1, &Set, NULL, NULL, NULL);

		for (auto current_fd : SlaveSockets) {
			if (FD_ISSET(current_fd, &Set)) {
				static char buffer[1024];

				int current_size = recv(current_fd, buffer,
					       	1024, MSG_NOSIGNAL);

				if (current_size == 0 && (errno != EAGAIN)) {
					shutdown(current_fd, SHUT_RDWR);
					close(current_fd);
					SlaveSockets.erase(current_fd);
				}
				else if (current_size > 0) {
					send(current_fd, buffer, 
							current_size, MSG_NOSIGNAL);
				}
			}
		}
		
		if (FD_ISSET(MasterSocket, &Set)) {
			int tmp_fd = accept(MasterSocket, 0, 0);
			set_nonblock(tmp_fd);
			SlaveSockets.insert(tmp_fd);
		}	
	} 

	return 0;
}
