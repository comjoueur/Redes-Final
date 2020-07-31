//g++ slave.cpp -o slave

#include "headers/rdt_extended.h"
#include "headers/structures.h"

rdt_extended_client client("127.0.0.1", 50004);

int file_key = 0;

void register_slave()
{
  cout << "SLAVE_REGISTER" << endl;
  client.write_rdt("SLAVE_REGISTER");
  file_key = client.read_rdt_num();
  cout << file_key << endl;
}

void process_slave(graph &subgraph)
{
  while (1)
  {
    string command = client.read_rdt();
    cout << "START RUNNING COMMAND " << command << endl;
    if (command == "CREATE_NODE")
    {
      string node_name = client.read_rdt();
      cout << node_name << endl;
      int num_attr = client.read_rdt_num();
      cout << num_attr << endl;
      vector<pair<string, string>> attributes;
      for (int i = 0; i < num_attr; i++)
      {
        cout << "iteration" << i << endl;
        string name_attr = client.read_rdt();
        string value_attr = client.read_rdt();
        cout << name_attr << " " << value_attr << endl;
        attributes.push_back(make_pair(name_attr, value_attr));
      }
      string response = subgraph.create_node(node_name, attributes);
      cout << response << endl;
      client.write_rdt(response);
    }
    else if (command == "DELETE_NODE")
    {
      string node_name = client.read_rdt();
      string response = subgraph.delete_node(node_name);
      cout << response << endl;
      client.write_rdt(response);
    }
    else if (command == "CREATE_CONEXION")
    {
      string node_name = client.read_rdt();
      string node_to_name = client.read_rdt();
      string response = subgraph.create_conexion(node_name, node_to_name);
      cout << response << endl;
      client.write_rdt(response);
    }
    else if (command == "DELETE_CONEXION")
    {
      string node_name = client.read_rdt();
      string node_to_name = client.read_rdt();
      string response = subgraph.delete_conexion(node_name, node_to_name);
      cout << response << endl;
      client.write_rdt(response);
    }
    else if (command == "GET_CONEXIONS")
    {
      string node_name = client.read_rdt();
      vector<string> conexions = subgraph.get_conexions(node_name);
      int num_conexions = conexions.size();
      cout << num_conexions << endl;
      client.write_rdt_num(num_conexions);
      for (int i = 0; i < conexions.size(); i++)
      {
        cout << conexions[i] << endl;
        client.write_rdt(conexions[i]);
      }
    }
    else if (command == "GET_ATTRIBUTES")
    {
      string node_name = client.read_rdt();
      vector<pair<string, string>> attributes = subgraph.get_attributes(node_name);
      cout << attributes.size() << endl;
      client.write_rdt_num(attributes.size());
      for (int i = 0; i < attributes.size(); i++)
      {
        cout << attributes[i].first << endl;
        client.write_rdt(attributes[i].first);
        cout << attributes[i].second << endl;
        client.write_rdt(attributes[i].second);
      }
    }
    else if (command == "UPDATE_ATTRIBUTE")
    {
      string node_name = client.read_rdt();
      string attribute = client.read_rdt();
      string value = client.read_rdt();
      string response = subgraph.update_atributte(node_name, attribute, value);
      cout << response << endl;
      client.write_rdt(response);
    }
    cout << "END RUNNING COMMAND " << command << endl;
  }
}

int main()
{
  register_slave();
  cout<<file_key<<endl;
  graph subgraph(file_key);
  cout<<file_key<<endl;
  cout << "Slave Running with id " << file_key << " ..." << endl;
  process_slave(subgraph);
}
