/***********************************************************
 * Trabalho Prático - Redes de Computadores
 * Professor: Renato Ishii
 * Aluno: Ian Haranaka | RGA: 2018.1904.009-7
 * Comando de compilação: g++ servidor.cpp -o servidor -Wall
 ***********************************************************/

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 7896
#define MSG_MAX_SIZE 1024

// Reference: https://www.bogotobogo.com/cplusplus/sockets_server_client.php
// https://www.geeksforgeeks.org/socket-programming-cc/
// https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/

class TCP_Server {
 private:
  int server_fd, client_fd;
  struct sockaddr_in server_addr, client_addr;
  char buffer[MSG_MAX_SIZE];
  std::string msg = "";

  FILE *fpipe;

  void SetAddr()
  {
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
  }

  void SetSockOpt()
  {
    int optval = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, sizeof(optval)))
    {
      perror("Error in setsockopt");
      exit(EXIT_FAILURE);
    }
  }

 public:
  TCP_Server()
  {
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd < 0)
    {
      perror("Error in socket");
      exit(EXIT_FAILURE);
    }
    std::cout << "Server socket created successfully\n";
    SetSockOpt();
    SetAddr();
  }

  ~TCP_Server()
  {
    close(server_fd);
    close(client_fd);
  }

  void Bind()
  {
    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
    {
      perror("Error in bind");
      exit(EXIT_FAILURE);
    }
    std::cout << "Binding successfull\n";
  }

  void Listen()
  {
    if (listen(server_fd, 3))
      std::cout << "Listening...\n";
  }

  void Accept()
  {
    socklen_t addr_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &addr_len);
    if (client_fd < 0) exit(EXIT_FAILURE);
    std::cout << "Connection established\n";
  }

  void Send(const std::string& message)
  {
    send(client_fd, message.data(), message.size(), 0);
  }

  // Salva a mensagem em uma string e retorna o tamanho da mensagem
  int Receive()
  {
    int msg_size = recv(client_fd, buffer, sizeof(buffer), 0);
    msg = buffer;
    memset(buffer, 0, sizeof(buffer));
    return msg_size;
  }

  std::string GetMessage()
  {
    return msg;
  }

  void MakeDir(const std::string &name)
  {
    std::string command = "mkdir ../servidor/";

    if (name.empty())
      Send("Incomplete command");
    else
    {
      fpipe = (FILE*) popen((command + name + " 2>&1").c_str(), "r");
      if (fpipe)
      {
        while (!feof(fpipe))
          fgets(buffer, sizeof(buffer), fpipe);

        if (strlen(buffer) == 0)
          Send("Directory successfully created");
        else
        {
          buffer[strlen(buffer) - 1] = '\0';
          std::string output(buffer);
          Send(output);
        }

        memset(buffer, 0, sizeof(buffer));
        pclose(fpipe);
      }
    }
  }

  void RemoveDir(const std::string &name)
  {
    std::string command = "rm -r ../servidor/";

    if (name.empty())
      Send("Incomplete command");
    else
    {
      fpipe = (FILE*) popen((command + name + " 2>&1").c_str(), "r");
      if (fpipe)
      {
        while (!feof(fpipe))
          fgets(buffer, sizeof(buffer), fpipe);

        if (strlen(buffer) == 0)
          Send("Directory successfully removed");
        else
        {
          buffer[strlen(buffer) - 1] = '\0';
          std::string output(buffer);
          Send(output);
        }

        memset(buffer, 0, sizeof(buffer));
        pclose(fpipe);
      }
    }
  }

  void List(const std::string &name)
  {
    char temp_buffer[100] = "";
    std::string command = "ls ../servidor/";

    if (name.empty())
      fpipe = (FILE*) popen(command.c_str(), "r");
    else
      fpipe = (FILE*) popen((command + name).c_str(), "r");

    if (fpipe)
    {
      while (!feof(fpipe))
      {
        strcat(buffer, temp_buffer);
        fgets(temp_buffer, sizeof(temp_buffer), fpipe);
        buffer[strlen(buffer) - 1] = ' ';
      }

      buffer[strlen(buffer) - 1] = '\0';
      std::string output(buffer);

      if (output == "")
        Send("Empty");
      else
        Send(output);

      memset(buffer, 0, sizeof(buffer));
      pclose(fpipe);
    }
  }

  void RemoveFile(const std::string &name)
  {
    std::string command = "rm ../servidor/";
    if (name.empty())
      Send("Incomplete command");
    else
    {
      fpipe = (FILE*) popen((command + name + " 2>&1").c_str(), "r");
      if (fpipe)
      {
        while (!feof(fpipe))
          fgets(buffer, sizeof(buffer), fpipe);

        if (strlen(buffer) == 0)
          Send(name + " successfully removed");
        else
        {
          buffer[strlen(buffer) - 1] = '\0';
          std::string output(buffer);
          Send(output);
        }

        memset(buffer, 0, sizeof(buffer));
        pclose(fpipe);
      }
    }
  }

  void ReceiveFile(const std::string& filename)
  {
    FILE *fp = fopen(("../servidor/" + filename).c_str(), "w");

    // Checa se deu erro ou se o arquivo já existe na pasta do servidor
    if (fp == NULL)
      Send("Error");
    else
    {
      Send("Ok");
      Receive();
      int size = stoi(msg);

      for (int i = 0; i < size; ++i)
      {
        recv(client_fd, buffer, sizeof(buffer), 0);
        fwrite(buffer, sizeof(char), 1024, fp);
        memset(buffer, 0, sizeof(buffer));
      }

      fclose(fp);
    }

    Send("File received successfully");
  }
};

int main()
{
  std::string in, command, name;

  TCP_Server server;
  server.Bind();
  server.Listen();
  server.Accept();

  // Enquanto o cliente estiver conectado
  while (server.Receive())
  {
    std::stringstream ss(server.GetMessage());
    ss >> command;

    if(command == "make_dir")
    {
      ss >> name;
      server.MakeDir(name);
    }
    else if (command == "remove_dir")
    {
      ss >> name;
      server.RemoveDir(name);
    }
    else if (command == "ls")
    {
      ss >> name;
      server.List(name);
    }
    else if (command == "send_file")
    {
      ss >> name;
      server.ReceiveFile(name);
    }
    else if (command == "remove_file")
    {
      ss >> name;
      server.RemoveFile(name);
    }
    else
      server.Send("Unknown command");

    name.clear();
  }

  std::cout << "Connection broken" << std::endl;

  return 0;
}
