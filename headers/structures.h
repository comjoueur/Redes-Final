#include "lib.h"
#include <fstream>

string error_operation = "ERROR_OPERATION";
string success_operation = "SUCCESS_OPERATION";

vector<string> split(const string str, const string delim)
{
  vector<string> tokens;
  size_t prev = 0, pos = 0;
  do
  {
    pos = str.find(delim, prev);
    if (pos == string::npos)
      pos = str.length();
    string token = str.substr(prev, pos - prev);
    if (!token.empty())
      tokens.push_back(token);
    prev = pos + delim.length();
  } while (pos < str.length() && prev < str.length());
  return tokens;
}

struct node
{
public:
  string name;
  vector<pair<string, string>> attributes;
  vector<string> conexions;

  node() {}

  node(string data)
  {
    vector<string> novo = split(data, "$/%/$");
    if (novo.size() > 0 && novo[0] != "")
    {
      name = novo[0];
    }
    if (novo.size() > 1 && novo[1] != "")
    {
      vector<string> attributes_pair = split(novo[1], "#/&/#");
      for (int j = 0; j < attributes_pair.size(); j++)
      {
        vector<string> attrs = split(attributes_pair[j], "&%&%&");
        attributes.push_back(make_pair(attrs[0], attrs[1]));
      }
    }
    if (novo.size() > 2 && novo[2] != "")
    {
      conexions = split(novo[2], "&/%()");
    }
  }

  string update_attribute(string name_attr, string value_attr)
  {
    int k = 0;
    for (int i = 0; i < attributes.size(); i++)
    {
      if (attributes[i].first == name_attr)
      {
        k = i;
      }
    }
    if (k < attributes.size())
    {
      attributes[k].second = value_attr;
      return success_operation;
    }
    return error_operation;
  }

  string create_conexion(string name_node)
  {
    int k = 0;
    for (int i = 0; i < conexions.size(); i++)
    {
      if (conexions[i] == name_node)
      {
        k = i;
      }
      k++;
    }
    if (k == conexions.size())
    {
      conexions.push_back(name_node);
      return success_operation;
    }
    return error_operation;
  }

  string delete_conexion(string name_node)
  {
    int k = 0;
    for (int i = 0; i < conexions.size(); i++)
    {
      if (conexions[i] == name_node)
      {
        k = i;
      }
    }
    if (k != conexions.size())
    {
      conexions.erase(conexions.begin() + k);
      return success_operation;
    }
    return error_operation;
  }

  string to_string()
  {
    string data;
    data += name;
    data += "$/%/$";
    for (int i = 0; i < attributes.size(); i++)
    {
      data += attributes[i].first;
      data += "&%&%&";
      data += attributes[i].second;
      data += "#/&/#";
    }
    data += "$/%/$";
    for (int i = 0; i < conexions.size(); i++)
    {
      data += conexions[i];
      data += "&/%()";
    }
    return data;
  }

  void print()
  {
    cout << endl
         << "----START NODE----" << endl;
    cout << "Name " << name << endl;
    cout << "Atributes:" << endl;
    for (int i = 0; i < attributes.size(); i++)
    {
      cout << "   Name: " << attributes[i].first << "   Value: " << attributes[i].second << endl;
    }
    cout << endl;
    cout << "Conexions:" << endl;
    for (int i = 0; i < conexions.size(); i++)
    {
      cout << "   Conexion Node Name: " << conexions[i] << endl;
    }
    cout << endl
         << "----END NODE----" << endl;
  }
};

class graph
{
public:
  vector<node> nodes;
  ifstream read_file;
  string file = "data/node_file_";

  graph(int key_file)
  {
    file += ('0' + key_file);
    file += ".txt";
    read_file.open(file);
    load_data();
    persist();
  }

  int find_index(string name)
  {
    for (int i = 0; i < nodes.size(); i++)
    {
      if (nodes[i].name == name)
      {
        return i;
      }
    }
    return -1;
  }

  void load_data()
  {
    string data;
    read_file >> data;
    nodes = string_to_nodes(data);
  }

  void persist()
  {
    ofstream write_file(file);
    write_file << nodes_to_string(nodes);
  }

  string create_node(string name, vector<pair<string, string>> attributes)
  {
    if (find_index(name) == -1)
    {
      node n;
      n.name = name;
      n.attributes = attributes;
      nodes.push_back(n);
      persist();
      return success_operation;
    }
    return error_operation;
  }

  string delete_node(string name)
  {
    int node_index = find_index(name);
    if (node_index > -1)
    {
      nodes.erase(nodes.begin() + node_index);
      persist();
      return success_operation;
    }
    return error_operation;
  }

  string create_conexion(string name_node, string name_to_node)
  {
    int index_node = find_index(name_node);
    if (index_node > -1)
    {
      string rsp = nodes[index_node].create_conexion(name_to_node);
      persist();
      return rsp;
    }
    return error_operation;
  }

  string delete_conexion(string name_node, string name_to_node)
  {
    int index_node = find_index(name_node);
    if (index_node > -1)
    {
      string rsp = nodes[index_node].delete_conexion(name_to_node);
      persist();
      return rsp;
    }
    return error_operation;
  }

  string update_atributte(string name_node, string name_attribute, string value_attribute)
  {
    int index_node = find_index(name_node);
    if (index_node > -1)
    {
      string rsp = nodes[index_node].update_attribute(name_attribute, value_attribute);
      persist();
      return rsp;
    }
    return error_operation;
  }

  string nodes_to_string(vector<node> n)
  {
    string rsp;
    for (int i = 0; i < n.size(); i++)
    {
      rsp += n[i].to_string();
      rsp += "$&$/()";
    }
    return rsp;
  }

  vector<node> string_to_nodes(string data)
  {
    vector<node> _nodes;
    vector<string> novo = split(data, "$&$/()");
    for (int i = 0; i < novo.size(); i++)
    {
      if (novo[i] != "")
      {
        _nodes.push_back(node(novo[i]));
      }
    }
    return _nodes;
  }

  vector<string> get_conexions(string name)
  {
    int node_index = find_index(name);
    if (node_index > -1)
    {
      return nodes[node_index].conexions;
    }
    vector<string>novo;
    return novo;
  }

  vector<pair<string, string> > get_attributes(string name)
  {
    int node_index = find_index(name);
    if (node_index > -1)
    {
      return nodes[node_index].attributes;
    }
    vector<pair<string, string> >novo;
    return novo;
  }

  node get_node(string name)
  {
    int node_index = find_index(name);
    if (node_index > -1)
    {
      return nodes[find_index(name)];
    }
    node n;
    return n; 
  }
};
