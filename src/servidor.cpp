#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

// Reference: https://www.bogotobogo.com/cplusplus/sockets_server_client.php
// https://www.geeksforgeeks.org/socket-programming-cc/
// https://idiotdeveloper.com/file-transfer-using-tcp-socket-in-c/

class TCP_Server {
 private:
  int server_fd, client_fd;
  struct sockaddr_in server_addr, client_addr;
  int optval = 1;

  void SetAddr()
  {
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
  }

  void SetSockOpt()
  {
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)))
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
};

int main()
{
  TCP_Server server;
  server.Bind();
  server.Listen();
  server.Accept();

  bool exit = false;
  while (!exit)
  {

  }

  return 0;
}
