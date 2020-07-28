//g++ client.cpp -o client

#include "headers/rdt_extended.h"

rdt_extended_client client("127.0.0.1", 8080);

void client_process()
{
  while (1)
  {
    string command;
    cin >> command;
    client.write(command);

    if (command == "CREATE_NODE")
    {
      cout << "Ingrese el nombre del nodo" << endl;
      string node_name;
      cin >> node_name;
      client.write(node_name);

      int num_attr;
      cout << "Ingrese el numero de atributos" << endl;
      cin >> num_attr;
      client.write_num(num_attr);

      for (int i = 0; i < num_attr; i++)
      {
        string name_attr;
        cout << "Ingrese el nombre del atributo" << endl;
        cin >> name_attr;
        client.write(name_attr);

        string value_attr;
        cout << "Ingrese el valor del atributo" << endl;
        cin >> value_attr;
        client.write(value_attr);
      }

      cout << client.read() << endl;
    }
    else if (command == "DELETE_NODE")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write(node_name);

      cout << client.read() << endl;
    }
    else if (command == "CREATE_CONEXION")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write(node_name);

      string node_to_name;
      cout << "Ingrese el nombre del otro nodo" << endl;
      cin >> node_to_name;
      client.write(node_to_name);

      cout << client.read() << endl;
    }
    else if (command == "DELETE_CONEXION")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write(node_name);

      string node_to_name;
      cout << "Ingrese el nombre del otro nodo" << endl;
      cin >> node_to_name;
      client.write(node_to_name);

      cout << client.read() << endl;
    }
    else if (command == "GET_CONEXIONS")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write(node_name);

      int num_con = client.read_num();
      for (int i = 0; i < num_con; i++)
      {
        string conexion = client.read();
        cout << "Conexion ";
        cout << conexion << endl;
      }
    }
    else if (command == "GET_ATTRIBUTES")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write(node_name);

      int num_attributes = client.read_num();
      for (int i = 0; i < num_attributes; i++)
      {
        cout << "Atributo ";
        string attribute = client.read();
        cout << attribute << endl;

        cout << "Valor ";
        string value = client.read();
        cout << value << endl;
      }
    }
    else if (command == "UPDATE_ATTRIBUTE")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write(node_name);

      cout << "Ingrese el nombre del Atributo" << endl;
      string attribute;
      cin >> attribute;
      client.write(attribute);

      cout << "Ingrese el nombre del Valor" << endl;
      string value;
      cin >> value;
      client.write(value);

      cout << client.read() << endl;
    }
  }
}

int main()
{
  client_process();
}