#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

class TCP_Client {



};

int main()
{
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_addr;

  if (client_fd < 0)
  {
    perror("Error in socket");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
  {
    perror("Error in socket");
    exit(EXIT_FAILURE);
  }

  std::cout << "Connected to Server\n";

  close(client_fd);
  return 0;
}