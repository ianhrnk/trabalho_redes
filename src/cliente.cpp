#include <iostream>
#include <map>
#include <string>

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

enum Command {
  make_dir, remove_dir, ls, send_file, remove_file
};

class TCP_Client {
 private:
  int client_fd;
  struct sockaddr_in server_addr;

  void SetServerAddr()
  {
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
  }

 public:
  TCP_Client()
  {
    client_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_fd < 0)
    {
      perror("Error in socket");
      exit(EXIT_FAILURE);
    }
    std::cout << "Client socket created successfully\n";
    SetServerAddr();
  }

  ~TCP_Client()
  {
    close(client_fd);
  }

  void Connect()
  {
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
      perror("Error in connection");
      exit(EXIT_FAILURE);
    }
    std::cout << "Connected to Server\n";
  }
};

int main()
{
  Command command;
  std::string _command;
  std::map<std::string, Command> str_to_command = {
    {"make_dir", make_dir},
    {"remove_dir", remove_dir},
    {"ls", ls},
    {"send_file", send_file},
    {"remove_file", remove_file}
  };

  TCP_Client client;
  client.Connect();

  bool exit = false;
  std::string in, name;
  while (!exit)
  {
    std::getline(std::cin, in);
    std::stringstream ss(in);
    ss >> _command;
    command = str_to_command[_command];

    switch(command)
    {
      case make_dir:
        ss >> name;
        break;

      case remove_dir:
        ss >> name;
        break;

      case ls:
        break;

      case send_file:
        break;

      case remove_file:
        ss >> name;
        break;

      case exit:
        exit = true;
        break;

      default:
        break;
    }
  }

  return 0;
}
