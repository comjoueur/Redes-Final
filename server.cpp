/* g++ server.cpp -pthread -o server */

// Server side implementation of UDP client-server model

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
#define MAXLINE 2048

using namespace std;

bool close_server = false;
int sockfd;

vector<sockaddr> clients;
vector<deque<string> > wait_messages;

int write_rdt(int client_id, string message)
{
  char buffer[message.size()];
  for (int i = 0; i < message.size(); ++i)
  {
    buffer[i] = message[i];
  }
  sockaddr cliaddr = clients[client_id];
  socklen_t len = sizeof(cliaddr);
  sendto(sockfd, buffer, message.size(), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
  return 0;
}

int register_client(sockaddr cliaddr) {
  int id = clients.size();
  clients.push_back(cliaddr);
  deque<string>novo;
  wait_messages.push_back(novo);
  char char_id = 'a' + id;
  string rep;
  rep+=char_id;
  write_rdt(id, rep);
}

void listener()
{
  while (1)
  {
    char buffer[MAXLINE];
    sockaddr cliaddr;
    socklen_t len = sizeof(cliaddr);
    int n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *)&cliaddr,
                     &len);
    buffer[n]='\0';
    if (buffer[0] == 'R')
    {
      register_client(cliaddr);
    }
    else {
      string msg; 
      for (int i = 1; i < n; i++)
      {
        msg += buffer[i];
      }
      int client_id = buffer[0] - 'a';
      wait_messages[client_id].push_back(msg);
    }
  }
}

int read_rdt(int client_id, string &message, int size)
{
  while(wait_messages[client_id].size()==0) {}
  message = wait_messages[client_id][0];
  wait_messages[client_id].pop_front();
  return 0;
}

void write_number_client(int client_id, int number)
{
  string message;
  message += 'a' + (number / 10);
  message += 'a' + (number % 10);
  int n = write_rdt(client_id, message);
  if (n < 0)
    cout << "There was a problem writing a message to client";
}

void write_string_client(int client_id, string message)
{
  int size = message.size();
  string _message;
  _message += 'a' + (size / 10);
  _message += 'a' + (size % 10);
  int n = write_rdt(client_id, _message);
  n = write_rdt(client_id, message);
  if (n < 0)
    cout << "There was a problem writing a message to client";
}

void write_command_client(int client_id, string command)
{
  int n = write_rdt(client_id, command);
  if (n < 0)
    cout << "There was a problem writing a message to client";
}

int read_number_client(int client_id)
{
  string _number;
  int n = read_rdt(client_id, _number, 2);
  if (n < 0)
    cout << "There was a problem reading message from client" << endl;
  int number = (_number[0] - 'a') * 10 + (_number[1] - 'a');
  return number;
}

string read_string_client(int client_id)
{
  string _size;
  int n = read_rdt(client_id, _size, 2);
  if (n < 0)
    cout << "There was a problem reading message from client";
  int size = (_size[0] - 'a') * 10 + (_size[1] - 'a');
  string _message;
  n = read_rdt(client_id, _message, size);
  if (n < 0)
    cout << "There was a problem reading message from client";
  string message;
  for (int i = 0; i < size; i++)
  {
    message += _message[i];
  }
  return message;
}

string read_command_client(int client_id)
{
  string _command;
  int n = read_rdt(client_id, _command, 1);
  if (n < 0)
  {
    cout << "There was a problem reading message from client" << endl;
  }
  string command;
  command += _command[0];
  return command;
}

void client_process(int client_id)
{
  bool close = 0;
  while (!close)
  {
    string m = read_string_client(client_id);
    cout<<m<<endl;
    write_string_client(client_id, m+" readed");
  }
}

void clients_listener()
{
  int number_clients = 0;
  while (1)
  {
    if (clients.size() > number_clients)
    {
      thread(client_process, number_clients).detach();
      number_clients++;
    }
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

  thread(listener).detach();

  clients_listener();
  return 0;
}