//g++ client.cpp -o client

#include "headers/rdt_extended.h"

rdt_extended_client client("127.0.0.1", 50004);

void client_process()
{
  while (1)
  {
    cout << "Ingresa el comando" << endl;
    string command;
    cin >> command;
    client.write_rdt(command);

    if (command == "CREATE_NODE")
    {
      cout << "Ingrese el nombre del nodo" << endl;
      string node_name;
      cin >> node_name;
      client.write_rdt(node_name);

      int num_attr;
      cout << "Ingrese el numero de atributos" << endl;
      cin >> num_attr;
      client.write_rdt_num(num_attr);

      for (int i = 0; i < num_attr; i++)
      {
        string name_attr;
        cout << "Ingrese el nombre del atributo" << endl;
        cin >> name_attr;
        client.write_rdt(name_attr);

        string value_attr;
        cout << "Ingrese el valor del atributo" << endl;
        cin >> value_attr;
        client.write_rdt(value_attr);
      }

      cout << client.read_rdt() << endl;
    }
    else if (command == "DELETE_NODE")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write_rdt(node_name);

      cout << client.read_rdt() << endl;
    }
    else if (command == "CREATE_CONEXION")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write_rdt(node_name);

      string node_to_name;
      cout << "Ingrese el nombre del otro nodo" << endl;
      cin >> node_to_name;
      client.write_rdt(node_to_name);

      cout << client.read_rdt() << endl;
    }
    else if (command == "DELETE_CONEXION")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write_rdt(node_name);

      string node_to_name;
      cout << "Ingrese el nombre del otro nodo" << endl;
      cin >> node_to_name;
      client.write_rdt(node_to_name);

      cout << client.read_rdt() << endl;
    }
    else if (command == "GET_CONEXIONS")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write_rdt(node_name);

      int num_con = client.read_rdt_num();
      for (int i = 0; i < num_con; i++)
      {
        string conexion = client.read_rdt();
        cout << "Conexion ";
        cout << conexion << endl;
      }
    }
    else if (command == "GET_ATTRIBUTES")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write_rdt(node_name);

      int num_attributes = client.read_rdt_num();
      for (int i = 0; i < num_attributes; i++)
      {
        cout << "Atributo ";
        string attribute = client.read_rdt();
        cout << attribute << endl;

        cout << "Valor ";
        string value = client.read_rdt();
        cout << value << endl;
      }
    }
    else if (command == "UPDATE_ATTRIBUTE")
    {
      string node_name;
      cout << "Ingrese el nombre del nodo" << endl;
      cin >> node_name;
      client.write_rdt(node_name);

      cout << "Ingrese el nombre del Atributo" << endl;
      string attribute;
      cin >> attribute;
      client.write_rdt(attribute);

      cout << "Ingrese el nombre del Valor" << endl;
      string value;
      cin >> value;
      client.write_rdt(value);

      cout << client.read_rdt() << endl;
    }
  }
}

int main()
{
  cout << "Commands" << endl;
  cout << "1. CREATE_NODE" << endl;
  cout << "2. DELETE_NODE" << endl;
  cout << "3. CREATE_CONEXION" << endl;
  cout << "4. DELETE_CONEXION" << endl;
  cout << "5. GET_CONEXIONS" << endl;
  cout << "6. GET_ATTRIBUTES" << endl;
  cout << "7. UPDATE_ATTRIBUTE" << endl;
  client_process();
}