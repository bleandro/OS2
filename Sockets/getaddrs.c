#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <ifaddrs.h>

int main() {
  struct ifaddrs* addrs;
  struct ifaddrs* tmp;

  getifaddrs(&addrs);
  tmp = addrs;

  while (tmp)
  {
      if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
      {
          struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
          printf("%s: %s\n", tmp->ifa_name, inet_ntoa(pAddr->sin_addr));
      }

      tmp = tmp->ifa_next;
  }

  freeifaddrs(addrs);
}
