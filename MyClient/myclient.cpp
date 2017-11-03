/* myclient.c */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#define BUF 1024

void receive(int fd, char *buf);

bool connectSocket(int&, char*, u_int16_t, char*);



int main (int argc, char **argv) {
    //ToDo: Put this in Config file
    const char SUCCESS[4] = "OK\n";
    const char FAILURE[5] = "ERR\n";
    u_int16_t PORT;
    char* SERVERADRESS;
  int create_socket;
  char buffer[BUF];


  if( argc < 3 || argc > 3){
     printf("Usage: %s <Port-number> <ServerAdresse>\n", argv[0]);
     exit(EXIT_FAILURE);
  }
    else{
      PORT = (uint16_t)atoi(argv[1]);
      SERVERADRESS = argv[2];
  }

  if(!connectSocket(create_socket, SERVERADRESS, PORT, buffer)){
      return EXIT_FAILURE;
  }

    //connected, do something till user wishes to quit:
  do {
      //send command
      fgets (buffer, BUF, stdin);
      send(create_socket, buffer, strlen (buffer), 0);

      //receive response
      receive(create_socket, buffer);

      //start parameter sending
      while (strcmp (buffer, SUCCESS) != 0 && strcmp (buffer, FAILURE) != 0){ //As long as the end wasn't reached (natural or error)
          //print received instructions (status message)
          printf ("%s ", buffer);
          //get user input and send
          if(strncasecmp(buffer, "password:", 9) == 0){
              termios tty;
              tcgetattr(STDIN_FILENO, &tty);
              tty.c_lflag &= ~ECHO;
              tcsetattr(STDIN_FILENO, TCSANOW, &tty);
              fgets (buffer, BUF, stdin);
              tty.c_lflag |= ECHO;
              tcsetattr(STDIN_FILENO, TCSANOW, &tty);
          }
          else{
              fgets (buffer, BUF, stdin);
          }
          send(create_socket, buffer, strlen (buffer), 0);
          //receive response
          receive(create_socket, buffer);
      }

      //override received status message with result:
      send(create_socket, SUCCESS, strlen(SUCCESS), 0); //send confirmation
      strcpy(buffer, "");
      receive(create_socket, buffer);

      //quit if command is quit
      if(strcmp (buffer, "quit\n") == 0){
          break;
      }

      //Print only result
      printf ("%s ", buffer);
      //request next command
      printf ("Please enter your command:\n");

  } while (true);
  close (create_socket);
  return EXIT_SUCCESS;
}




void receive(int fd, char *buf){
    long size=recv(fd,buf, BUF-1, 0);
    if (size>0)
    {
        buf[size]= '\0';
    }
}




bool connectSocket(int &create_socket, char* serveraddress, u_int16_t port, char *buf){
    struct sockaddr_in address;

    if ((create_socket = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Socket error");
        return false;
    }

    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons (port);
    inet_aton (serveraddress, &address.sin_addr);

    if (connect ( create_socket, (struct sockaddr *) &address, sizeof (address)) == 0)
    {
        printf ("Connection with server (%s) established\n", inet_ntoa (address.sin_addr));
        //expect a response;
        receive(create_socket, buf);
        printf ("%s ", buf);
    }
    else
    {
        perror("Connect error - no server available");
        return false;
    }
    return true;
}