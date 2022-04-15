#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

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
  int SlaveSocket;

  while (SlaveSocket = accept(MainSocket, (struct sockaddr*)(&SockAddrClient), &addrlen)) {

    printf("new_ip=%u\n", SockAddrClient.sin_addr.s_addr);

    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    setsockopt(SlaveSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    while (1) {
      char buffer[512] = {};
      int result = recv(SlaveSocket, buffer, 512, MSG_NOSIGNAL);
      printf("%s\n", buffer);

      if (result == -1 && errno == EAGAIN) { printf("timeout\n"); }
      else if (!result) break;
      else if (-1 == send(SlaveSocket, buffer, 512, MSG_NOSIGNAL)) break;
    }
  }
  shutdown(SlaveSocket, SHUT_RDWR);
  close(SlaveSocket);

  return 0;
}



