#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <vector>
#include <thread>

#include "message_queue.cpp"

#define PORT 9699

MessageQueue message_queue; 
std::vector<int> clients;

void read_and_enqueue_messages (int socket) {
  char username[1024] = {0};
  read(socket, username, 1024);
  while (true) {
    char buffer[1024] = {0};
    read(socket, buffer, 1024);
    string tempMessage = username + string(": ") + buffer;
    message_queue.push(tempMessage);
  }
}

void forward_messages () {
  while (true) {
    if (message_queue.empty()) continue;
    string message = message_queue.top();
    std::cout << "Forwarding " + message;
    const char *messageAdd = message.c_str();
    for (int i = 0; i < clients.size(); ++i) {
      send(clients[i], messageAdd, message.length(), 0);
    }
    message_queue.pop();
  }
}

int main(int argc, char const *argv[]) {   
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int addrlen = sizeof(address);
  int opt = 1;
  int test;

  // Creating socket
  test = server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (test == -1) {
    perror("socket failed");
    exit(-1);
  }   

  // Set socket option
  test = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (test == -1) {
    perror("set socket option failed");
    exit(-1);
  } 
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; 
  address.sin_port = htons(PORT);

  // Attach socket to port
  test = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
  if (test == -1) {
    perror("bind failed");
    exit(-1);
  }

  // Waiting for clients to connect
  test = listen(server_fd, 3);
  if (test == -1) {
    perror("listen failed");
    exit(-1);
  }

  // Accept and print message from client
  thread forward(forward_messages);
  std::vector<std::thread> threads;
  while (true) {
    test = new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    clients.push_back(new_socket);
    if (test == -1) {
      perror("accept failed");
      exit(-1);
    }
    threads.emplace_back(std::thread(read_and_enqueue_messages, new_socket));
  }

  return 0;
}
