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
#include "rdt.h"

#define PORT 8080

using namespace std;

bool close_server = false;
rdt_client rdt;

void write_number_server(int number)
{
  string message;
  message += 'a' + (number / 10);
  message += 'a' + (number % 10);
  int n = rdt.write_rdt(message);
  if (n < 0)
    cout << "There was a problem writing a message to server" << endl;
}

void write_string_server(string message)
{
  int size = message.size();
  string _message;
  _message += 'a' + (size / 10);
  _message += 'a' + (size % 10);
  int n = rdt.write_rdt(_message);
  n = rdt.write_rdt(message);
  if (n < 0)
    cout << "There was a problem writing a message to server" << endl;
}

void write_command_server(string command)
{
  int n = rdt.write_rdt(command);
  if (n < 0)
    cout << "There was a problem writing a message to server" << endl;
}

int read_number_server()
{
  string _number;
  int n = rdt.read_rdt(_number, 2);
  if (n < 0)
    cout << "There was a problem reading message from server" << endl;
  int number = (_number[0] - 'a') * 10 + (_number[1] - 'a');
  return number;
}

string read_string_server()
{
  string _size;
  int n = rdt.read_rdt(_size, 2);
  if (n < 0)
    cout << "There was a problem reading message from server" << endl;
  int size = (_size[0] - 'a') * 10 + (_size[1] - 'a');
  string _message;
  n = rdt.read_rdt(_message, size);
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
  int n = rdt.read_rdt(_command, 1);
  if (n < 0)
  {
    cout << "There was a problem reading message from server" << endl;
  }
  string command;
  command += _command[0];
  return command;
}

int register_client() {
  rdt.write_rdt("R");
  string message;
  rdt.read_rdt(message, 1);
  rdt.client_id = message[0]-'a';
  cout<<"Client register with client_id "<<rdt.client_id<<endl;
}

void server_process()
{
  cout<<"Client running"<<endl;
  register_client();
  bool close = 0;
  while (!close)
  {
    cout<<"Write your message"<<endl;
    string p;
    cin>> p;
    write_string_server(p);
    cout<<read_string_server()<<endl;
  }
}

int main()
{
  struct hostent *host;

  host = (struct hostent *)gethostbyname((char *)"127.0.0.1");

  // Creating socket file descriptor
  if ((rdt.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&rdt.servaddr, 0, sizeof(rdt.servaddr));

  // Filling server information
  rdt.servaddr.sin_family = AF_INET;
  rdt.servaddr.sin_port = htons(PORT);
  rdt.servaddr.sin_addr = *((struct in_addr *)host->h_addr);

  server_process();
}
