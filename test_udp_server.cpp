// g++ test_udp_server.cpp -pthread -o test_udp_server

#include "lib.h"
#include "udp.h"

udp_server S;

void client_process(int client_id)
{
  string p = S.read_udp(client_id);
  S.write_udp(client_id, p);
}

void clients_listener()
{
  cout<<"Server running...."<<endl;
  int number_clients = 0;
  while (1)
  {
    if (S.clients.size() > number_clients)
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
  if ((S.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
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
  if (bind(S.sockfd, (const struct sockaddr *)&servaddr,
           sizeof(servaddr)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  S.listen();
  clients_listener();
  return 0;
}