#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MY_SOCK_PATH "./local"
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


struct sock {
	int socket;
};

void *thread_entry(void *args) {
	while (1) {
		struct sock *sock_ptr = (struct sock*) args;
		char input[50];
		int ret;
		ret = read(sock_ptr->socket, input, 50);
		if (ret == -1) handle_error("read");
		write(stdout, input, ret);
	}
}

int main(int argc, char *argv[]) {
	int clientSocket;
	char buffer[50];
	struct sockaddr_un serverAddr;
	socklen_t addr_size = sizeof(struct sockaddr);
	
	
	
	
	//	struct struct_socket variable;
	
	clientSocket = socket(AF_UNIX, SOCK_STREAM, 0);   //1
	if (clientSocket == -1)
		handle_error("socket");

	memset(&serverAddr, 0, sizeof(struct sockaddr));

	strncpy(serverAddr.sun_path, MY_SOCK_PATH, sizeof(serverAddr.sun_path) - 1);
	serverAddr.sun_family = AF_UNIX;

//	addr_size = sizeof serverAddr;
	
	if(connect(clientSocket, (struct sockaddr*) &serverAddr, addr_size) == -1)
		handle_error("connect");

	//similary to thread but read STDIN_FILENO and write to socket.

	pthread_t sock_t;
	struct sock sock_conn;
	sock_conn.socket = clientSocket;
	
	pthread_create(&sock_t, NULL, thread_entry, &sock_conn);

	int ret;
	ret = read(stdin, buffer, 50);
	write(clientSocket, buffer, ret);

	//instructions for client
	//recieve connection
	//read from socket
	//write to socket and send data to server
	while (strncmp(buffer, "quit\n", 5)) {
		printf("Send message to server: ");
		fgets(buffer, 1024, stdin);
		write(clientSocket, (void*)buffer, 50);
	}

	return 0;
}
