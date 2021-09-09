#include <iostream>
#include <set>
#include <algorithm>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define MAX_EVENTS 32

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

	struct sockaddr_in SockAddr;
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(1234);
	SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(MasterSocket, (struct sockaddr *)(&SockAddr), sizeof(sockaddr_in));
	set_nonblock(MasterSocket);

	listen(MasterSocket, SOMAXCONN);

    int Epoll = epoll_create1(0);

    struct epoll_event Event;
    Event.data.fd = MasterSocket;
    Event.events = EPOLLIN;
    epoll_ctl(Epoll, EPOLL_CTL_ADD, MasterSocket, &Event);

	while (true) {
        struct epoll_event Events[MAX_EVENTS];
        unsigned int n = epoll_wait(Epoll, Events, MAX_EVENTS, -1);

        for (unsigned int i = 0; i < n; ++i) {
            if (Events[i].data.fd == MasterSocket) {
                 int SlaveSocket = accept(MasterSocket, 0, 0);
                 set_nonblock(SlaveSocket);

                 struct epoll_event Event;
                 Event.data.fd = SlaveSocket;
                 Event.events = EPOLLIN;

                 epoll_ctl(Epoll, EPOLL_CTL_ADD, SlaveSocket, &Event);
            }
            else {
                static char Buffer[1024];
                int recive_size = recv(Events[i].data.fd, Buffer, 1024, MSG_NOSIGNAL);
                if (recive_size == 0 && errno != EAGAIN) {
                    shutdown(Events[i].data.fd, SHUT_RDWR);
                    close(Events[i].data.fd);
                }
                else if (recive_size > 0) {
                    send(Events[i].data.fd, Buffer, recive_size, MSG_NOSIGNAL);
                }
            }
        }
	} 

	return 0;
}
