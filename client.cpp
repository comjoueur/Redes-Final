//g++ client.cpp -pthread -o client

#include "headers/rdt_extended.h"

rdt_extended_client client("127.0.0.1", 8080);

int main()
{
  while (1)
  {
    string p;
    cin >> p;
    client.write(p);
    cout << client.read() << endl;
  }
}