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

	// create client socket
	test = sock = socket(AF_INET, SOCK_STREAM, 0);
	if (test == -1) {
		perror("socket failed");
		exit(-1); 
	}
	 serv_addr.sin_family = AF_INET;
	 serv_addr.sin_port = htons(SERVER_PORT);

	// convert address
	test = inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
	if (test == -1) {
		perror("invalid address");
		exit(-1);
	}
	test = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	if (test == -1) {
		perror("connect failed");
		exit(-1);
	}
	while (true) {
		std::string temp = "";
		getline (std::cin, temp);
		if (!temp.empty()) {
			const char *tempAdd= temp.c_str();
			send(sock, tempAdd, temp.length(), 0);
		}
	}
	valread = read(sock, buffer, 1024);
	printf("%s\n", buffer);
	return 0;
}
