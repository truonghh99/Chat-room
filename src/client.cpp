#include <cstdio> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <string>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <cstring>
#define SERVER_PORT 9699

std::string username;

/* This method read messgaes from server, check if they belong to this client, and print out if they don't */
void server_to_screen(int sock) {
  char buffer[1024] = {0};
  while (true) {
    int byte_read = read(sock, buffer, 1024);
    buffer[byte_read] = 0;    int i = 0;
    // Read username of the message's sender
    while (buffer[i] != ':') { 
      if (buffer[i] == username[i]) {
        ++i;
      } else {
        break;
      }
    }
    // Check if this username matches the client's name
    if (buffer[i] != ':') { 
      printf("%s\n", buffer);
    }
  }
}

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
  printf("What do you want to be called? (please use only characters and digits)\n");
  getline(std::cin, username);
  send(sock, username.c_str(), username.length(), 0);
  printf("-----You are connected!-----\n");

  // Communicate with other clients via server
  std::thread receive_from_server(server_to_screen, sock);

  // Read from keyboard and send to server
  while (true) {
    std::string temp = "";
    getline (std::cin, temp);
    if (!temp.empty()) {
      const char *temp_add= temp.c_str();
      send(sock, temp_add, temp.length(), 0);
    }
  }

  return 0;
}
