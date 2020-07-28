#include "headers/structures.h"

int main()
{
  int key = 1;
  graph nodes(key);
  vector<pair<string, string>> attributes;
  attributes.push_back(make_pair("att_1", "val_1"));
  attributes.push_back(make_pair("att_2", "val_2"));
  cout << nodes.create_node("node_1", attributes) << endl;
  vector<pair<string, string>> attributes1;
  attributes1.push_back(make_pair("att_name_1", "att_val_1"));
  attributes1.push_back(make_pair("att_name_2", "att_val_2"));
  cout << nodes.create_node("node_2", attributes1) << endl;
  cout << nodes.create_conexion("node_1", "node_2") << endl;
  cout << nodes.update_atributte("node_1", "att_1", "valor_diferente") << endl;
  nodes.nodes[0].print();
  nodes.nodes[1].print();
}