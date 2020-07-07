/* g++ server.cpp -pthread -o server */

#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
#define mkp(x, y) make_pair(x, y)

using namespace std;

bool close_server = false;
int sockfd;

int write_rdt(sockaddr cliaddr, string message)
{
  char buffer[message.size()];
  for (int i=0;i<message.size(); ++i) {
    buffer[i] = message[i];
  }
  socklen_t len = sizeof(cliaddr);
  sendto(sockfd, buffer, message.size(), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
}

int read_rdt(sockaddr &cliaddr, string &message, int size)
{
  char buffer[size];
  socklen_t len = sizeof(cliaddr);
  memset(&cliaddr, 0, sizeof(cliaddr));
  int n = recvfrom(sockfd, (char *)buffer, size,
               MSG_WAITALL, (struct sockaddr *)&cliaddr,
               &len);
  for(int i=0; i < size; i++) {
    message += buffer[i];
  }
}
void write_number_client(sockaddr cliaddr, int number)
{
  string message;
  message += 'a' + (number / 10);
  message += 'a' + (number % 10);
  int n = write_rdt(cliaddr, message);
  if (n < 0)
    cout << "There was a problem writing a message to client";
}

void write_string_client(sockaddr cliaddr, string message)
{
  int size = message.size();
  string _message;
  _message += 'a' + (size / 10);
  _message += 'a' + (size % 10);
  int n = write_rdt(cliaddr, _message);
  n = write_rdt(cliaddr, message);
  if (n < 0)
    cout << "There was a problem writing a message to client";
}

void write_command_client(sockaddr cliaddr, string command)
{
  int n = write_rdt(cliaddr, command);
  if (n < 0)
    cout << "There was a problem writing a message to client";
}

int read_number_client(sockaddr &cliaddr)
{
  string _number;
  int n = read_rdt(cliaddr, _number, 2);
  if (n < 0)
    cout << "There was a problem reading message from client" << endl;
  int number = (_number[0] - 'a') * 10 + (_number[1] - 'a');
  return number;
}

string read_string_client(sockaddr &cliaddr)
{
  string _size;
  int n = read_rdt(cliaddr, _size, 2);
  if (n < 0)
    cout << "There was a problem reading message from client";
  int size = (_size[0] - 'a') * 10 + (_size[1] - 'a');
  string _message;
  n = read_rdt(cliaddr, _message, size);
  if (n < 0)
    cout << "There was a problem reading message from client";
  string message;
  for (int i = 0; i < size; i++)
  {
    message += _message[i];
  }
  return message;
}

string read_command_client(sockaddr &cliaddr)
{
  string _command;
  int n = read_rdt(cliaddr, _command, 1);
  if (n < 0)
  {
    cout << "There was a problem reading message from client" << endl;
  }
  string command;
  command += _command[0];
  return command;
}

void client_process()
{
  bool close = 0;
  while(!close) {
    sockaddr cliaddr;
    cout << read_string_client(cliaddr) << endl;
    write_string_client(cliaddr, "maomao");
  }
}

int main()
{
  struct sockaddr_in servaddr;

  // Creating socket file descriptor
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));

  // Filling server information
  servaddr.sin_family = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  // Bind the socket with the server address
  if (bind(sockfd, (const struct sockaddr *)&servaddr,
           sizeof(servaddr)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  client_process();
  return 0;
}