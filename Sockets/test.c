#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>

int main() {
  struct hostent* hostinfo;


  char socket_host[255];
  gethostname(socket_host, sizeof(socket_host));


  hostinfo = gethostbyname(socket_host);
  if (hostinfo == NULL)
    return 1;


  printf("%s\n", socket_host);
}
