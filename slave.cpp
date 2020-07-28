//g++ slave.cpp -o slave

#include "headers/rdt_extended.h"
#include "headers/structures.h"

rdt_extended_client client("127.0.0.1", 8080);

int file_key = 0;

void register_slave()
{
  client.write("SLAVE_REGISTER");
  file_key = client.read_num();
}

void process_slave(graph &subgraph)
{
  while (1)
  {
    string command = client.read();
    if (command == "CREATE_NODE")
    {
      string node_name = client.read();
      int num_attr = client.read_num();
      vector<pair<string, string>> attributes;
      for (int i = 0; i < num_attr; i++)
      {
        attributes.push_back(make_pair(client.read(), client.read()));
      }
      string response = subgraph.create_node(node_name, attributes);
      client.write(response);
    }
    else if (command == "DELETE_NODE")
    {
      string node_name = client.read();
      string response = subgraph.delete_node(node_name);
      client.write(response);
    }
    else if (command == "CREATE_CONEXION")
    {
      string node_name = client.read();
      string node_to_name = client.read();
      string response = subgraph.create_conexion(node_name, node_to_name);
      client.write(response);
    }
    else if (command == "DELETE_CONEXION")
    {
      string node_name = client.read();
      string node_to_name = client.read();
      string response = subgraph.delete_conexion(node_name, node_to_name);
      client.write(response);
    }
    else if (command == "GET_CONEXIONS")
    {
      string node_name = client.read();
      vector<string> conexions = subgraph.get_conexions(node_name);
      client.write_num(conexions.size());
      for (int i = 0; i < conexions.size(); i++)
      {
        client.write(conexions[i]);
      }
    }
    else if (command == "GET_ATTRIBUTES")
    {
      string node_name = client.read();
      vector<pair<string, string>> attributes = subgraph.get_attributes(node_name);
      client.write_num(attributes.size());
      for (int i = 0; i < attributes.size(); i++)
      {
        client.write(attributes[i].first);
        client.write(attributes[i].second);
      }
    }
    else if (command == "UPDATE_ATTRIBUTE")
    {
      string node_name = client.read();
      string attribute = client.read();
      string value = client.read();
      string response = subgraph.update_atributte(node_name, attribute, value);
      client.write(response);
    }
  }
}

int main()
{
  register_slave();
  graph subgraph(file_key);
  cout << "Slave Running..." << endl;
  process_slave(subgraph);
}
