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

/* Read text from the socket and print it out.  Continue until the
   socket closes.  Return non-zero if the client sent a "quit"
   message, zero otherwise.  */

int server (int client_socket)
{
  while (1) {
    int length;
    char* text;

    /* First, read the length of the text message from the socket.  If
       read returns zero, the client closed the connection.  */
    if (read (client_socket, &length, sizeof (length)) == 0)
      return 0;
    /* Allocate a buffer to hold the text.  */
    text = (char*) malloc (length);
    /* Read the text itself, and print it.  */
    read (client_socket, text, length);
    printf ("%s\n", text);

    /* If the client sent the message "quit", we're all done.  */
    if (strcmp (text, "quit") == 0) {
      /* Free the buffer.  */
      free (text);
      return 1;
    } else
        free(text);
  }
}

int main (int argc, char* const argv[])
{
  const char* const socket_host = "192.168.11.244";

  const int port = atoi(argv[1]);
  struct hostent* hostinfo;
  int socket_fd;
  struct sockaddr_in name;
  int client_sent_quit_message;

  /* Create the socket.  */
  socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  /* Indicate this is a server.  */
  name.sin_family = AF_INET;

  /* Convert from strings to numbers.  */
  hostinfo = gethostbyname(socket_host);

  if (hostinfo == NULL)
    return 1;
  else
    name.sin_addr = *((struct in_addr *) hostinfo->h_addr);
  name.sin_port = htons(port);

  if (bind (socket_fd, (struct sockaddr*) &name, sizeof (struct sockaddr_in)) < 0)
     printf("ERRO NO BIND \n");
  /* Listen for connections.  */
  listen (socket_fd, 5);

  /* Repeatedly accept connections, spinning off one server() to deal
     with each client.  Continue until a client sends a "quit" message.  */
  do {
    struct sockaddr_in client_name;
    socklen_t client_name_len;
    int client_socket_fd;

    /* Accept a connection.  */
    client_socket_fd = accept(socket_fd, (struct sockaddr*) &client_name, &client_name_len);
    /* Handle the connection.  */
    client_sent_quit_message = server (client_socket_fd);
    /* Close our end of the connection.  */
    close (client_socket_fd);
  }
  while (!client_sent_quit_message);

  /* Remove the socket file.  */
  close(socket_fd);
  unlink(socket_host);

  return 0;
}
