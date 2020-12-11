/*********************************************************
 * Trabalho Prático - Redes de Computadores
 * Professor: Renato Ishii
 * Aluno: Ian Haranaka | RGA: 2018.1904.009-7
 * Comando de compilação: g++ cliente.cpp -o cliente -Wall
 *********************************************************/

#include <iostream>
#include <string>
#include <sstream>

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

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

  int GetSocket()
  {
    return client_fd;
  }

  void SendCommand(const std::string& msg)
  {
    send(client_fd, msg.data(), msg.size(), 0);
  }
};

int main()
{
  TCP_Client client;
  client.Connect();

  bool exit = false;
  std::string in, command;
  while (!exit)
  {
    std::getline(std::cin, in);
    std::stringstream ss(in);
    ss >> command;

    client.SendCommand(in);
    if (command == "quit")
      exit = true;
  }

  return 0;
}
