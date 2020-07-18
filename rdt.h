#include <bits/stdc++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

string get_hash(string p) {
  int hash = 0;
  for(int i=0;i<p.size();i++) {
    hash += int(p[i]);
  }
  hash = hash % 100;
  string rsp;
  rsp += ('a' + hash/10);
  rsp += ('a' + hash%10);
  return rsp;
}

class rdt_server
{
public:
  vector<sockaddr> clients;
  vector<deque<string>> wait_messages;
  vector<int> sequence;
  int sockfd;
  int MAXLINE = 2048;  

  int write_rdt(int client_id, string _message)
  {
    int seq = sequence[client_id];
    string seq_client;
    seq_client += ('a' + (seq/10));
    seq_client += ('a' + (seq%10));
    string message = _message + seq_client;
    message = message + get_hash(_message);
    char buffer[message.size()];
    for (int i = 0; i < message.size(); ++i)
    {
      buffer[i] = message[i];
    }
    buffer[message.size()] = '\0';
    sockaddr cliaddr = clients[client_id];
    socklen_t len = sizeof(cliaddr);
    sendto(sockfd, buffer, message.size(), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    return 0;
  }

  int register_client(sockaddr cliaddr)
  {
    int id = clients.size();
    clients.push_back(cliaddr);
    deque<string> novo;
    wait_messages.push_back(novo);
    sequence.push_back(0);
    char char_id = 'a' + id;
    string rep;
    rep += char_id;
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
      buffer[n] = '\0';
      if (buffer[0] == 'R')
      {
        register_client(cliaddr);
      }
      else
      {
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
    while (wait_messages[client_id].size() == 0)
    {
    }
    message = wait_messages[client_id][0];
    string _message;
    for (int i=0;i<message.size()-4;i++) {
      _message += message[i];
    }
    string hash;
    hash += message[message.size()-2];
    hash += message[message.size()-1];
    string seq;
    seq += message[message.size()-4];
    seq += message[message.size()-3];
    if(hash!=get_hash(_message)) {
      cout<<"checksum incorrect"<<endl;
    }
    message = _message;
    wait_messages[client_id].pop_front();
    return 0;
  }
};


void _listener_rdt(rdt_server *rdt) {
  (*rdt).listener();
}

void listener_rdt(rdt_server *rdt) {
  thread(_listener_rdt, rdt).detach();
}


class rdt_client
{
public:
  struct sockaddr_in servaddr;
  int sockfd;
  int client_id = -1;
  int MAXLINE = 2048;
  int sequence;

  int write_rdt(string message)
  {
    string _message = message;
    int seq = sequence;
    string seq_client;
    seq_client += ('a' + (seq/10));
    seq_client += ('a' + (seq%10));
    message += seq_client;
    message += get_hash(_message);
    if (client_id != -1)
    {
      string id_str;
      id_str += 'a' + client_id;
      message = id_str + message;
    }
    char buffer[message.size()];
    for (int i = 0; i < message.size(); ++i)
    {
      buffer[i] = message[i];
    }
    socklen_t len = sizeof(servaddr);
    sendto(sockfd, buffer, message.size(), MSG_CONFIRM, (const struct sockaddr *)&servaddr, len);
    return 0;
  }

  int read_rdt(string &message, int size)
  {
    char buffer[MAXLINE];
    socklen_t len = sizeof(servaddr);
    memset(&servaddr, 0, sizeof(servaddr));
    int n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                     MSG_WAITALL, (struct sockaddr *)&servaddr,
                     &len);
    for (int i = 0; i < n - 4; i++)
    {
      message += buffer[i];
    }
    string hash;
    hash += buffer[n-2];
    hash += buffer[n-1];
    string seq;
    seq += message[message.size()-4];
    seq += message[message.size()-3];

    if(hash != get_hash(message)) {
      cout<<"checksum incorrect"<<endl;
    }

    return 0;
  }
};
