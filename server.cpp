//g++ server.cpp -pthread -o server

#include "headers/rdt_extended.h"

rdt_extended_server serv(8080);

vector<int> slaves;

int get_slave_id(string node_name)
{
  return slaves[get_hash_file_key(node_name, slaves.size())];
}

void client_process(int client_id)
{
  while (1)
  {
    string command = serv.read(client_id);

    // SLAVES COMMANDS

    if (command == "SLAVE_REGISTER")
    {
      serv.write_num(client_id, slaves.size());
      slaves.push_back(client_id);
    }

    // CLIENTS COMMANDS

    else if (command == "CREATE_NODE")
    {
      string node_name = serv.read(client_id);
      int slave_id = get_slave_id(node_name);

      serv.write(slave_id, "CREATE_NODE");
      serv.write(slave_id, node_name);

      int num_attr = serv.read_num(client_id);
      serv.write_num(slave_id, num_attr);

      for (int i = 0; i < num_attr; i++)
      {
        serv.write(slave_id, serv.read(client_id));
        serv.write(slave_id, serv.read(client_id));
      }

      serv.write(client_id, serv.read(slave_id));
    }
    else if (command == "DELETE_NODE")
    {
      string node_name = serv.read(client_id);
      int slave_id = get_slave_id(node_name);

      serv.write(slave_id, "DELETE_NODE");
      serv.write(slave_id, node_name);

      serv.write(client_id, serv.read(slave_id));
    }
    else if (command == "CREATE_CONEXION")
    {
      string node_name = serv.read(client_id);
      int slave_id_1 = get_slave_id(node_name);

      string node_to_name = serv.read(client_id);
      serv.write(slave_id_1, "CREATE_CONEXION");
      serv.write(slave_id_1, node_name);
      serv.write(slave_id_1, node_to_name);

      string response_1 = serv.read(slave_id_1);

      int slave_id_2 = get_slave_id(node_to_name);
      serv.write(slave_id_2, "CREATE_CONEXION");
      serv.write(slave_id_2, node_to_name);
      serv.write(slave_id_2, node_name);

      string response_2 = serv.read(slave_id_2);

      serv.write(client_id, response_1);
    }
    else if (command == "DELETE_CONEXION")
    {
      string node_name = serv.read(client_id);
      int slave_id_1 = get_slave_id(node_name);

      string node_to_name = serv.read(client_id);
      serv.write(slave_id_1, "DELETE_CONEXION");
      serv.write(slave_id_1, node_name);
      serv.write(slave_id_1, node_to_name);

      string response_1 = serv.read(slave_id_1);

      int slave_id_2 = get_slave_id(node_to_name);
      serv.write(slave_id_2, "DELETE_CONEXION");
      serv.write(slave_id_2, node_to_name);
      serv.write(slave_id_2, node_name);

      string response_2 = serv.read(slave_id_2);

      serv.write(client_id, response_1);
    }
    else if (command == "GET_CONEXIONS")
    {
      string node_name = serv.read(client_id);
      int slave_id = get_slave_id(node_name);

      serv.write(slave_id, "GET_CONEXIONS");
      serv.write(slave_id, node_name);

      int num_con = serv.read_num(slave_id);
      serv.write_num(client_id, num_con);

      for (int i = 0; i < num_con; i++)
      {
        serv.write(client_id, serv.read(slave_id));
      }
    }
    else if (command == "GET_ATTRIBUTES")
    {
      string node_name = serv.read(client_id);
      int slave_id = get_slave_id(node_name);

      serv.write(slave_id, "GET_ATTRIBUTES");
      serv.write(slave_id, node_name);

      int num_attr = serv.read_num(slave_id);
      serv.write_num(client_id, num_attr);

      for (int i = 0; i < num_attr; i++)
      {
        serv.write(client_id, serv.read(slave_id));
        serv.write(client_id, serv.read(slave_id));
      }
    }
    else if (command == "UPDATE_ATTRIBUTE")
    {
      string node_name = serv.read(client_id);
      int slave_id = get_slave_id(node_name);

      serv.write(slave_id, "GET_ATTRIBUTES");
      serv.write(slave_id, node_name);

      string attribute = serv.read(client_id);
      serv.write(slave_id, attribute);

      string value = serv.read(client_id);
      serv.write(slave_id, value);

      serv.write(client_id, serv.read(slave_id));
    }
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
  cout << "Server running..." << endl;
  clients_listener();
}
