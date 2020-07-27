//g++ server.cpp -pthread -o server

#include "headers/rdt_extended.h"

rdt_extended_server serv(8080);

void client_process(int client_id)
{
  while (1)
  {
    string p = serv.read(client_id);
    serv.write(client_id, p);
  }
}

void clients_listener()
{
  while (1)
  {
    if (serv.request_client())
    {
      thread(client_process, serv.accept_client()).detach();
    }
  }
}

int main()
{
  clients_listener();
}
