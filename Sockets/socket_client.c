/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

/* Write TEXT to the socket given by file descriptor SOCKET_FD.  */

void write_text (int socket_fd, const char* text)
{
  /* Write the number of bytes in the string, including
     NUL-termination.  */
  int length = strlen (text) + 1;
  write (socket_fd, &length, sizeof (length));
  /* Write the string.  */
  write (socket_fd, text, length);
}

int main (int argc, char* const argv[])
{
  struct hostent* hostinfo;
  struct sockaddr_in name;
  int socket_fd;

  /* Create the socket.  */
  socket_fd = socket (PF_INET, SOCK_STREAM, 0);

  //Ip string to number 32 bits
  hostinfo = gethostbyname (argv[1]);
  //Get number port
  name.sin_addr = *((struct in_addr *) hostinfo->h_addr);
  name.sin_port = htons(atoi(argv[2]));
  const char* const message = argv[3];

  /* Store the server's name in the socket address.  */
  name.sin_family = AF_INET;

  /* Connect the socket. */
  if (connect (socket_fd, (struct sockaddr *) &name, sizeof(struct sockaddr_in)) == -1) {
    perror("connect");
    return 1;
  }

  /* Write the text on the command line to the socket.  */
  write_text (socket_fd, message);
  close (socket_fd);
  return 0;
}
