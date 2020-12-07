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



 public:
  TCP_Server()
  {

  }

  ~TCP_Server()
  {

  }

};

int main()
{
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len;

  if (server_fd < 0)
  {
    perror("Error in socket");
    exit(EXIT_FAILURE);
  }

  std::cout << "Server socket created successfully\n";

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
  {
    perror("Error in bind");
    exit(EXIT_FAILURE);
  }

  std::cout << "Binding successfull\n";

  if (listen(server_fd, 3))
    std::cout << "Listening...\n";

  addr_len = sizeof(client_addr);
  int new_socket = accept(server_fd, (struct sockaddr *) &client_addr, &addr_len);

  if (new_socket < 0) exit(EXIT_FAILURE);

  close(server_fd);
  close(new_socket);
  return 0;
}
