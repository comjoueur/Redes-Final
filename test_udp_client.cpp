//g++ test_udp_client.cpp -pthread -o test_udp_client

#include "lib.h"
#include "udp.h"

udp_client S;

int main()
{
  struct hostent *host;

  host = (struct hostent *)gethostbyname((char *)"127.0.0.1");

  // Creating socket file descriptor
  if ((S.sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&S.servaddr, 0, sizeof(S.servaddr));

  // Filling server information
  S.servaddr.sin_family = AF_INET;
  S.servaddr.sin_port = htons(PORT);
  S.servaddr.sin_addr = *((struct in_addr *)host->h_addr);
  S.register_client();
  string p;
  cin>>p;
  S.write_udp(p);
  cout<<S.read_udp()<<endl;
}
