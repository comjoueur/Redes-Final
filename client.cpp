/* g++ client.cpp -pthread -o client  */

// Client side implementation of UDP client-server model

#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>

#define PORT 8080

using namespace std;

bool close_server = false;
struct sockaddr_in servaddr;
int sockfd;

int write_rdt(string message)
{
  char buffer[message.size()];
  for (int i=0;i<message.size(); ++i) {
    buffer[i] = message[i];
  }
  socklen_t len = sizeof(servaddr);
  sendto(sockfd, buffer, message.size(), MSG_CONFIRM, (const struct sockaddr *)&servaddr, len);
  return 0;
}

int read_rdt(string &message, int size)
{
  char buffer[size];
  socklen_t len = sizeof(servaddr);
  memset(&servaddr, 0, sizeof(servaddr));
  int n = recvfrom(sockfd, (char *)buffer, size,
               MSG_WAITALL, (struct sockaddr *)&servaddr,
               &len);
  for(int i=0; i < size; i++) {
    message += buffer[i];
  }
  return n;
}
void write_number_server(int number)
{
  string message;
  message += 'a' + (number / 10);
  message += 'a' + (number % 10);
  int n = write_rdt(message);
  if (n < 0)
    cout << "There was a problem writing a message to server" << endl;
}

void write_string_server(string message)
{
  int size = message.size();
  string _message;
  _message += 'a' + (size / 10);
  _message += 'a' + (size % 10);
  int n = write_rdt(_message);
  n = write_rdt(message);
  if (n < 0)
    cout << "There was a problem writing a message to server" << endl;
}

void write_command_server(string command)
{
  int n = write_rdt(command);
  if (n < 0)
    cout << "There was a problem writing a message to server" << endl;
}

int read_number_server()
{
  string _number;
  int n = read_rdt(_number, 2);
  if (n < 0)
    cout << "There was a problem reading message from server" << endl;
  int number = (_number[0] - 'a') * 10 + (_number[1] - 'a');
  return number;
}

string read_string_server()
{
  string _size;
  int n = read_rdt(_size, 2);
  if (n < 0)
    cout << "There was a problem reading message from server" << endl;
  int size = (_size[0] - 'a') * 10 + (_size[1] - 'a');
  string _message;
  n = read_rdt(_message, size);
  if (n < 0)
    cout << "There was a problem reading message from server" << endl;
  string message;
  for (int i = 0; i < size; i++)
  {
    message += _message[i];
  }
  return message;
}

string read_command_server()
{
  string _command;
  int n = read_rdt(_command, 1);
  if (n < 0)
  {
    cout << "There was a problem reading message from server" << endl;
  }
  string command;
  command += _command[0];
  return command;
}
void server_process()
{
  bool close = 0;
  while(!close) {
    write_string_server("Moamoa");
    cout<<read_string_server()<<endl;
    close = true;
  }
}


int main()
{
  struct hostent *host;

  host = (struct hostent *)gethostbyname((char *)"127.0.0.1");

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr = *((struct in_addr *)host->h_addr);

  server_process();
}
