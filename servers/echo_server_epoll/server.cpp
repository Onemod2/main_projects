#include <iostream>
#include <set>
#include <algorithm>
#include <functional>
#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

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

struct User {
    struct sockaddr_in sock_addr;
    int fd;
    User(int fd) : fd(fd) {}
    User(sockaddr_in sock_addr, int fd) : sock_addr(sock_addr), fd(fd) {}
};

using cmp = decltype([](User f, User s) {
return f.fd < s.fd;
});

void notify_all(std::set<User, cmp>& users, int SlaveSocket, int MasterSocket, bool is_connect) {
    std::string s = "";
    if (is_connect) {
        s += "Connected ";
    }
    else {
        s += "Disconnected ";
    }
    auto slave_el = users.find(User(SlaveSocket));

    char IPv4Addr[32];
    std::memset(&IPv4Addr, 0, 32);
    inet_ntop(slave_el->sock_addr.sin_family, &slave_el->sock_addr.sin_addr, IPv4Addr,
              sizeof(IPv4Addr));

    s += std::string(IPv4Addr) + "\n";

    for (const User& el : users) {
        if (el.fd != SlaveSocket) {
            send(SlaveSocket, s.c_str(), s.size(), MSG_NOSIGNAL);
        }
    }
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

    std::set<User, cmp> users;

    while (true) {
        struct epoll_event Events[MAX_EVENTS];
        unsigned int n = epoll_wait(Epoll, Events, MAX_EVENTS, -1);

        for (unsigned int i = 0; i < n; ++i) {
            if (Events[i].data.fd == MasterSocket) {
                struct sockaddr_in SockAddr;
                socklen_t size = sizeof(sockaddr_in);
                int SlaveSocket = accept(MasterSocket, (sockaddr*)&SockAddr, &size);

                users.insert({SockAddr, SlaveSocket});
                notify_all(users, SlaveSocket, MasterSocket, true);

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
                    notify_all(users, Events[i].data.fd, MasterSocket, false);
                    users.erase(User(Events[i].data.fd));

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
