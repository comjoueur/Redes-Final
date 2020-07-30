//g++ server.cpp -pthread_rdt -o server

#include "headers/rdt_extended.h"

rdt_extended_server serv(8080);

vector<int> slaves;

int get_slave_id(string node_name)
{
  return slaves[get_hash_file_key(node_name, 2)];
}

void client_process(int client_id)
{
  while (1)
  {
    string command = serv.read_rdt(client_id);

    cout << "START RUNNING COMMAND " << command << endl;

    // SLAVES COMMANDS

    if (command == "SLAVE_REGISTER")
    {
      serv.write_rdt_num(client_id, slaves.size());
      slaves.push_back(client_id);
      cout << "END RUNNING COMMAND " << command << endl;
      return;
    }

    // CLIENTS COMMANDS

    else if (command == "CREATE_NODE")
    {
      string node_name = serv.read_rdt(client_id);
      int slave_id = get_slave_id(node_name);

      int num_attr = serv.read_rdt_num(client_id);
      cout << "CREATE_NODE" << endl;
      serv.write_rdt(slave_id, "CREATE_NODE");
      cout << node_name << endl;
      serv.write_rdt(slave_id, node_name);
      cout << num_attr << endl;
      serv.write_rdt_num(slave_id, num_attr);

      vector<pair<string, string>> attributes;

      for (int i = 0; i < num_attr; i++)
      {
        string name_attr = serv.read_rdt(client_id);
        string value_attr = serv.read_rdt(client_id);
        attributes.push_back(make_pair(name_attr, value_attr));
        cout << attributes[i].first << endl;
        serv.write_rdt(slave_id, attributes[i].first);
        cout << attributes[i].second << endl;
        serv.write_rdt(slave_id, attributes[i].second);
      }

      string response = serv.read_rdt(slave_id);
      cout << response << endl;
      serv.write_rdt(client_id, response);
    }
    else if (command == "DELETE_NODE")
    {
      string node_name = serv.read_rdt(client_id);

      int slave_id = get_slave_id(node_name);
      cout << "DELETE NODE" << endl;
      serv.write_rdt(slave_id, "DELETE_NODE");
      cout << node_name << endl;
      serv.write_rdt(slave_id, node_name);

      string response = serv.read_rdt(slave_id);
      cout << response << endl;
      serv.write_rdt(client_id, response);
    }
    else if (command == "CREATE_CONEXION")
    {
      string node_name = serv.read_rdt(client_id);
      int slave_id_1 = get_slave_id(node_name);
      string node_to_name = serv.read_rdt(client_id);
      int slave_id_2 = get_slave_id(node_to_name);

      cout << "CREATE_CONEXION" << endl;
      serv.write_rdt(slave_id_1, "CREATE_CONEXION");
      cout << node_name << endl;
      serv.write_rdt(slave_id_1, node_name);
      cout << node_to_name << endl;
      serv.write_rdt(slave_id_1, node_to_name);

      string response_1 = serv.read_rdt(slave_id_1);
      cout << "CREATE_CONEXION" << endl;
      serv.write_rdt(slave_id_2, "CREATE_CONEXION");
      cout << node_to_name << endl;
      serv.write_rdt(slave_id_2, node_to_name);
      cout << node_name << endl;
      serv.write_rdt(slave_id_2, node_name);

      string response_2 = serv.read_rdt(slave_id_2);
      cout << response_1 << endl;
      serv.write_rdt(client_id, response_1);
    }
    else if (command == "DELETE_CONEXION")
    {
      string node_name = serv.read_rdt(client_id);
      int slave_id_1 = get_slave_id(node_name);

      string node_to_name = serv.read_rdt(client_id);
      cout << "DELETE_CONEXION" << endl;
      serv.write_rdt(slave_id_1, "DELETE_CONEXION");
      cout << node_name << endl;
      serv.write_rdt(slave_id_1, node_name);
      cout << node_to_name << endl;
      serv.write_rdt(slave_id_1, node_to_name);

      string response_1 = serv.read_rdt(slave_id_1);

      int slave_id_2 = get_slave_id(node_to_name);
      cout << "DELETE_CONEXION" << endl;
      serv.write_rdt(slave_id_2, "DELETE_CONEXION");
      cout << node_to_name << endl;
      serv.write_rdt(slave_id_2, node_to_name);
      cout << node_name << endl;
      serv.write_rdt(slave_id_2, node_name);

      string response_2 = serv.read_rdt(slave_id_2);

      serv.write_rdt(client_id, response_1);
    }
    else if (command == "GET_CONEXIONS")
    {
      string node_name = serv.read_rdt(client_id);
      int slave_id = get_slave_id(node_name);
      cout << "GET_CONEXIONS" << endl;
      serv.write_rdt(slave_id, "GET_CONEXIONS");
      cout << node_name << endl;
      serv.write_rdt(slave_id, node_name);

      int num_con = serv.read_rdt_num(slave_id);
      cout << num_con << endl;
      serv.write_rdt_num(client_id, num_con);

      for (int i = 0; i < num_con; i++)
      {
        cout << client_id << endl;
        serv.write_rdt(client_id, serv.read_rdt(slave_id));
      }
    }
    else if (command == "GET_ATTRIBUTES")
    {
      string node_name = serv.read_rdt(client_id);
      int slave_id = get_slave_id(node_name);
      cout << "GET_ATTRIBUTES" << endl;
      serv.write_rdt(slave_id, "GET_ATTRIBUTES");
      cout << "node_name" << endl;
      serv.write_rdt(slave_id, node_name);

      int num_attr = serv.read_rdt_num(slave_id);
      serv.write_rdt_num(client_id, num_attr);

      for (int i = 0; i < num_attr; i++)
      {
        serv.write_rdt(client_id, serv.read_rdt(slave_id));
        serv.write_rdt(client_id, serv.read_rdt(slave_id));
      }
    }
    else if (command == "UPDATE_ATTRIBUTE")
    {
      string node_name = serv.read_rdt(client_id);
      int slave_id = get_slave_id(node_name);

      cout << "UPDATE_ATTRIBUTE" << endl;
      serv.write_rdt(slave_id, "UPDATE_ATTRIBUTE");
      cout << node_name << endl;
      serv.write_rdt(slave_id, node_name);

      string attribute = serv.read_rdt(client_id);
      cout << attribute << endl;
      serv.write_rdt(slave_id, attribute);

      string value = serv.read_rdt(client_id);
      cout << value << endl;
      serv.write_rdt(slave_id, value);

      string response = serv.read_rdt(slave_id);
      cout << response << endl;
      serv.write_rdt(client_id, response);
    }
    cout << "END RUNNING COMMAND " << command << endl;
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