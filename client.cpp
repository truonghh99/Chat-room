#include <cstdio> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <string>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#define SERVER_PORT 9699

int main(int arg, char const *argv[]) {
  int sock = 0, valread;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};
  int test;

  // Create client socket
  test = sock = socket(AF_INET, SOCK_STREAM, 0);
  if (test == -1) {
    perror("Socket failed");
    exit(-1); 
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERVER_PORT);

  // Convert address
  test = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
  if (test == -1) {
    perror("Invalid address");
    exit(-1);
  }
  test = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  if (test == -1) {
    perror("Connect failed");
    exit(-1);
  }

  // Read username and send to server
  printf("What do you want to be called?\n");
  std::string username;
  getline(std::cin, username);
  send(sock, username.c_str(), username.length(), 0);

  // Read from keyboard and send to server
  while (true) {
    std::string temp = "";
    getline (std::cin, temp);
    if (!temp.empty()) {
      const char *temp_add= temp.c_str();
      send(sock, temp_add, temp.length(), 0);
    }
  }

  // Read from server and print
  valread = read(sock, buffer, 1024);
  printf("%s\n", buffer);
  return 0;
}
