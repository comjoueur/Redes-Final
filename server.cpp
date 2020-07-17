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
#include "rdt.h"

#define PORT 8080

using namespace std;

bool close_server = false;
rdt_server rdt;

void write_string_client(int client_id, string message)
{
  int size = message.size();
  string _message;
  _message += 'a' + (size / 10);
  _message += 'a' + (size % 10);
  int n = rdt.write_rdt(client_id, _message);
  n = rdt.write_rdt(client_id, message);
  if (n < 0)
    cout << "There was a problem writing a message to client";
}

void write_command_client(int client_id, string command)
{
  int n = rdt.write_rdt(client_id, command);
  if (n < 0)
    cout << "There was a problem writing a message to client";
}

int read_number_client(int client_id)
{
  string _number;
  int n = rdt.read_rdt(client_id, _number, 2);
  if (n < 0)
    cout << "There was a problem reading message from client" << endl;
  int number = (_number[0] - 'a') * 10 + (_number[1] - 'a');
  return number;
}

string read_string_client(int client_id)
{
  string _size;
  int n = rdt.read_rdt(client_id, _size, 2);
  if (n < 0)
    cout << "There was a problem reading message from client";
  int size = (_size[0] - 'a') * 10 + (_size[1] - 'a');
  string _message;
  n = rdt.read_rdt(client_id, _message, size);
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
  int n = rdt.read_rdt(client_id, _command, 1);
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
    write_string_client(client_id, m+" readed");
  }
}

void clients_listener()
{
  cout<<"Server running...."<<endl;
  int number_clients = 0;
  while (1)
  {
    if (rdt.clients.size() > number_clients)
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
  if ((rdt.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
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
  if (bind(rdt.sockfd, (const struct sockaddr *)&servaddr,
           sizeof(servaddr)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }

  listener_rdt(&rdt);

  clients_listener();
  return 0;
}