/*********************************************************
 * Trabalho Prático - Redes de Computadores
 * Professor: Renato Ishii
 * Aluno: Ian Haranaka | RGA: 2018.1904.009-7
 * Comando de compilação: g++ cliente.cpp -o cliente -Wall
 *********************************************************/

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 7896
#define MSG_MAX_SIZE 1024

class TCP_Client {
 private:
  int client_fd;
  struct sockaddr_in server_addr;
  char buffer[MSG_MAX_SIZE];

  void SetServerAddr()
  {
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
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

  void SendMessage(const std::string& msg)
  {
    send(client_fd, msg.data(), msg.size(), 0);
  }

  bool SendFile(const std::string& filename)
  {
    FILE *fp = fopen(("../cliente/" + filename).c_str(), "r");
    if (fp == NULL)
    {
      perror("Error in send file");
      return false;
    }

    // Envia a requisição de transferência do arquivo
    SendMessage("send_file " + filename);

    // Verifica se tá tudo ok para poder enviar
    if (Receive() == "Error")
      return false;

    // Envia a quantidade de blocos de 1024 bytes do arquivo
    long size;
    fseek(fp , 0 , SEEK_END);
    size = ftell(fp);
    size = (size + 1023) / 1024;
    rewind(fp);
    SendMessage(std::to_string(size));

    // Envia blocos de 1024 bytes
    for (int i = 0; i < size; ++i)
    {
      fread(buffer, sizeof(char), 1024, fp);
      send(client_fd, buffer, sizeof(buffer), 0);
      memset(buffer, 0, sizeof(buffer));
    }

    fclose(fp);
    return true;
  }

  std::string Receive()
  {
    recv(client_fd, buffer, sizeof(buffer), 0);
    std::string msg(buffer);
    memset(buffer, 0, sizeof(buffer));
    return msg;
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

    if (command != "quit" && command != "send_file")
    {
      client.SendMessage(in);
      std::cout << client.Receive() << std::endl;
    }
    else if (command == "send_file")
    {
      std::string filename;
      ss >> filename;
      if (client.SendFile(filename))
        std::cout << client.Receive() << std::endl;
    }
    else
      exit = true;
  }

  return 0;
}
