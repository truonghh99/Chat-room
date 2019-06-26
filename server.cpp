#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 9699

int main(int argc, char const *argv[]) {
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int opt = 1;
	char buffer[1024] = {0};
	char *hello = "Hello from server";
	int test;

	// creating socket
	test = server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (test == -1) {
		perror("socket failed");
		exit(-1);
	}	

	// set socket option
	test = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (test == -1) {
		perror("set socket option failed");
		exit(-1);
	} 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(PORT);

	// attach socket to port
	test = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
	if (test == -1) {
		perror("bind failed");
		exit(-1);
	}

	// listen
	test = listen(server_fd, 3);
	if (test == -1) {
		perror("listen failed");
		exit(-1);
	}

	// accept
	test = new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
	if (test == -1) {
		perror("accept failed");
		exit(-1);
	}

	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	send(new_socket, hello, strlen(hello), 0);
	printf("Hello message sent");
	return 0;
}