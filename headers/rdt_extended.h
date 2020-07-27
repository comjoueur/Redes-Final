#include "rdt.h"

int max_packages = 10;
int max_msg_size = 2000;

string number_to_msg(int n)
{
  string response = "";
  response += ('a' + (n / 10));
  response += ('a' + (n % 10));
  return response;
}

int msg_to_number(string msg)
{
  return (msg[0] - 'a') * 10 + (msg[1] - 'a');
}

string extract_seq(string msg, string &t_msg)
{
  t_msg = "";
  for (int i = 0; i < msg.size() - 2; i++)
  {
    t_msg += msg[i];
  }
  string seq;
  seq += msg[msg.size()-2];
  seq += msg[msg.size()-1];
  return seq;
}

class rdt_extended_server : public rdt_server
{
public:
  rdt_extended_server(int port)
      : rdt_server(port)
  {
  }

  void seq_write(int client_id, string message, string seq_cli)
  {
    write_rdt(client_id, message + seq_cli);
    string seq_cli_m = serv.read_udp(client_id);
    if (seq_cli_m != seq_cli)
    {
      serv.write_udp(client_id, ack_error);
      seq_write(client_id, message, seq_cli);
    }
    else
    {
      serv.write_udp(client_id, ack_ok);
    }
  }

  void bulk_write(int client_id, vector<string> messages)
  {
    string p;
    for (int i = 0; i < messages.size(); i++)
    {
      seq_write(client_id, messages[i], number_to_msg(i));
    }
  }

  void write(int client_id, string message)
  {
    vector<string> messages;
    string acum = "";
    for (int i = 0; i < message.size(); i++)
    {
      if (i % max_msg_size == 0 && i != 0)
      {
        messages.push_back(acum);
        acum = "";
        acum += message[i];
      }
      else
      {
        acum += message[i];
      }
    }
    if (acum != "")
    {
      messages.push_back(acum);
    }
    vector<string> acum_st;
    for (int i = 0; i < messages.size(); i++)
    {
      if (i % max_packages == 0 && i != 0)
      {
        bulk_write(client_id, acum_st);
        vector<string> novo;
        acum_st = novo;
        acum_st.push_back(messages[i]);
      }
      else {
        acum_st.push_back(messages[i]);
      }
    }
    acum_st.push_back(ack_end);
    if (acum_st.size() != 0)
    {
      bulk_write(client_id, acum_st);
    }
  }

  string bulk_read(int client_id)
  {
    string msg = read_rdt(client_id);
    string message;
    string cli_seq = extract_seq(msg, message);
    serv.write_udp(client_id, cli_seq);
    if (serv.read_udp(client_id) == ack_ok)
    {
      return message;
    }
    else
    {
      return bulk_read(client_id);
    }
  }

  string read(int client_id)
  {
    string message = "";
    string msg = "";
    while (msg != ack_end)
    {
      message += msg;
      msg = bulk_read(client_id);
    }
    return message;
  }

  void write_num(int client_id, int number) {
    string num = to_string(number);
    this->write(client_id, num);
  }

  int read_num(int client_id) {
    string number = this->read(client_id);
    return stoi(number);
  }
};

class rdt_extended_client : public rdt_client
{
public:
  rdt_extended_client(string direction, int port)
      : rdt_client(direction, port)
  {
  }

  void seq_write(string message, string seq_cli)
  {
    write_rdt(message + seq_cli);
    string seq_cli_m = client.read_udp();
    if (seq_cli_m != seq_cli)
    {
      client.write_udp(ack_error);
      seq_write(message, seq_cli);
    }
    else
    {
      client.write_udp(ack_ok);
    }
  }

  void bulk_write(vector<string> messages)
  {
    string p;
    for (int i = 0; i < messages.size(); i++)
    {
      seq_write(messages[i], number_to_msg(i));
    }
  }

  void write(string message)
  {
    vector<string> messages;
    string acum = "";
    for (int i = 0; i < message.size(); i++)
    {
      if (i % max_msg_size == 0 && i != 0)
      {
        messages.push_back(acum);
        acum = "";
        acum += message[i];
      }
      else
      {
        acum += message[i];
      }
    }
    if (acum != "")
    {
      messages.push_back(acum);
    }
    vector<string> acum_st;
    for (int i = 0; i < messages.size(); i++)
    {
      if (i % max_packages == 0 && i != 0)
      {
        bulk_write(acum_st);
        vector<string> novo;
        acum_st = novo;
        acum_st.push_back(messages[i]);
      }
      else {
        acum_st.push_back(messages[i]);
      }
    }
    acum_st.push_back(ack_end);
    if (acum_st.size() != 0)
    {
      bulk_write(acum_st);
    }
  }

  string bulk_read()
  {
    string msg = read_rdt();
    string message;
    string cli_seq = extract_seq(msg, message);
    client.write_udp(cli_seq);
    if (client.read_udp() == ack_ok)
    {
      return message;
    }
    else
    {
      return bulk_read();
    }
  }

  string read()
  {
    string message = "";
    string msg = "";
    while (msg != ack_end)
    {
      message += msg;
      msg = bulk_read();
    }
    return message;
  }

  void write_num(int number) {
    string num = to_string(number);
    this->write(num);
  }

  int read_num() {
    string number = this->read();
    return stoi(number);
  }
};
