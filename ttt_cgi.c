#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void reprintInterface(char *address, char *port, char *username, char *password, char *gamename, char *square) {
  printf("%s%c%c\n", "Content-Type:text/html;charset=iso-8859-1",13,10);

  printf("<html>");
  printf("<body>");
  printf("<form action=\"./ttt.cgi\">");
  if (address == NULL) {
    printf("<b>Server Address: <input type=\"text\" name=\"address\" size=\"20\"><br />");
  } else {
    printf("<b>Server Address: <input type=\"text\" name=\"address\" value=\"%s\" size=\"20\"><br />", address);
  }
  if (port == NULL) {
    printf("<b>Server Port: <input type=\"text\" name=\"port\" size=\"20\"><br />");
  } else {
    printf("<b>Server Port: <input type=\"text\" name=\"port\" value=\"%s\" size=\"20\"><br />", port);
  }
  if (username == NULL) {
    printf("<b>Username: <input type=\"text\" name=\"username\" size=\"20\"><br />");
  } else {
    printf("<b>Username: <input type=\"text\" name=\"username\" value=\"%s\" size=\"20\"><br />", username);
  }
  if (password == NULL) {
    printf("<b>Password: <input type=\"text\" name=\"password\" size=\"20\"><br />");
  } else {
    printf("<b>Password: <input type=\"text\" name=\"password\" value=\"%s\" size=\"20\"><br />", password);
  }
  if (gamename == NULL) {
    printf("<b>Gamename: <input type=\"text\" name=\"gamename\" size=\"20\"><br />");
  } else {
    printf("<b>Gamename: <input type=\"text\" name=\"gamename\" value=\"%s\" size=\"20\"><br />", gamename);
  }
  if (square == NULL) {
    printf("<b>Square: <input type=\"text\" name=\"square\" size=\"20\"><br />");
  } else {
    printf("<b>Square: <input type=\"text\" name=\"square\" value=\"%s\" size=\"20\"><br />", square);
  }

  printf("<input type=\"submit\" value=\"LOGIN\" name=\"LOGIN\">");
  printf("<input type=\"submit\" value=\"CREATE\" name=\"CREATE\">");
  printf("<input type=\"submit\" value=\"JOIN\" name=\"JOIN\">");
  printf("<input type=\"submit\" value=\"MOVE\" name=\"MOVE\">");
  printf("<input type=\"submit\" value=\"LIST\" name=\"LIST\">");
  printf("<input type=\"submit\" value=\"SHOW\" name=\"SHOW\">");
  printf("</form>");
  printf("</body>");
  printf("</html>");
}

int main(void)
{
  char *rawData;
  char data[2000];
  char *address;
  char *port;
  char *username;
  char *password;
  char *gamename;
  char *square;
  char *option;

  rawData = getenv("QUERY_STRING");
  strcpy(data, rawData);

  address = strtok(data, "&");
  port = strtok(NULL, "&");
  username = strtok(NULL, "&");
  password = strtok(NULL, "&");
  gamename = strtok(NULL, "&");
  square = strtok(NULL, "&");
  option = strtok(NULL, "&");

  address = strtok(address, "=");
  address = strtok(NULL, "=");

  port = strtok(port, "=");
  port = strtok(NULL, "=");

  username = strtok(username, "=");
  username = strtok(NULL, "=");

  password = strtok(password, "=");
  password = strtok(NULL, "=");

  gamename = strtok(gamename, "=");
  gamename = strtok(NULL, "=");

  square = strtok(square, "=");
  square = strtok(NULL, "=");

  option = strtok(option, "=");
  option = strtok(NULL, "=");

  char messageToServer[200];
  if (strcmp(option, "LOGIN")==0) {
    if (username == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (password == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else {
      sprintf(messageToServer, "%s,%s,%s", option, username, password);
    }
  } else if (strcmp(option, "CREATE")==0) {
    if (username == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (password == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (gamename == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else {
      sprintf(messageToServer, "%s,%s,%s,%s", option, username, password, gamename);
    }
  } else if (strcmp(option, "JOIN")==0) {
    if (username == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (password == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (gamename == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (square) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else {
      sprintf(messageToServer, "%s,%s,%s,%s,%s", option, username, password, gamename, square);
    }
  } else if (strcmp(option, "MOVE")==0) {
    if (username == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (password == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (gamename == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (square) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else {
      sprintf(messageToServer, "%s,%s,%s,%s,%s", option, username, password, gamename, square);
    }
  } else if (strcmp(option, "LIST")==0) {
    if (username == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (password == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else {
     sprintf(messageToServer, "%s,%s,%s", option, username, password);
    }
  } else if (strcmp(option, "SHOW")==0) {
    if (username == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (password == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else if (gamename == NULL) {
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR INVALID INPUT");
      return 1;
    } else {
      sprintf(messageToServer, "%s,%s,%s,%s", option, username, password, gamename);
    }
  }

  struct sockaddr_in server;
  int sock;
  char convertedAddress[20];
  unsigned short int convertedPort;

  strcpy(convertedAddress, address);
  convertedPort = (unsigned short int)atoi(port);

  server.sin_addr.s_addr = inet_addr(convertedAddress);
  server.sin_family = AF_INET;
  server.sin_port = htons(convertedPort);

  //while(1) {
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    reprintInterface(address, port, username, password, gamename, square);
    printf("<br>ERROR COULD NOT CREATE SOCKET");
    return 1;
  }

  if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
    reprintInterface(address, port, username, password, gamename, square);
    printf("<br>ERROR CONNECTION FAILED");
    return 1;
  }

  if (send(sock, messageToServer, 200, 0) < 0) {
    reprintInterface(address, port, username, password, gamename, square);
    printf("<br>ERROR UNABLE TO SEND MESSAGE TO SERVER");
    return 1;
  }

  size_t read_size;
  int bytes_read = 0;
  char incoming_msg[2000];
  char temp[2000];
  while( bytes_read < 2000 ){
    read_size = recv(sock , temp, 2000, 0);
    if(read_size <= 0){
      reprintInterface(address, port, username, password, gamename, square);
      printf("<br>ERROR UNABLE TO SEND MESSAGE TO SERVER");x
      fflush(stdout);
      close(sock);
      return 0;
    }
    memcpy( incoming_msg+bytes_read, temp, read_size );
    bytes_read += read_size;
  }

  reprintInterface(address, port, username, password, gamename, square);
  char converted_msg[2000];
  converted_msg[0] = '\0';

  for (int i = 0; i < strlen(incoming_msg); i++) {
    if (incoming_msg[i] == '\n') {
      sprintf(converted_msg, "%s<br>", converted_msg);
    } else if (incoming_msg[i] == ' ') {
      sprintf(converted_msg, "%s&ensp;", converted_msg);
    } else if (incoming_msg[i] == '-') {
      sprintf(converted_msg, "%s&ndash;", converted_msg);
    } else {
      sprintf(converted_msg, "%s%c", converted_msg, incoming_msg[i]);
    }
  }
  printf("<br>%s", converted_msg);

  close(sock);
  return 0;
}
