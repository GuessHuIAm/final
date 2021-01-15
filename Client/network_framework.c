#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include "communication_protocol.h"

int client_socket;
char receive_buffer[1024];

void disconnect() {
	char req[5];
	sprintf(req, "%04d", DISCONNECT_REQUEST);

	send(client_socket, req, sizeof(req), 0);
	close(client_socket);
}

void connect_server() {
	//Create the client socket
	client_socket = socket(AF_INET, SOCK_STREAM, 0);

	//Prevent an address and port reuse error from occurring
	int opt = 1;
	setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	//Connect to server address and port
	struct sockaddr_in binding_info; 
	binding_info.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &binding_info.sin_addr);
    binding_info.sin_port = htons(8000);
	connect(client_socket, (struct sockaddr *)&binding_info, sizeof(binding_info));
}

int get_client_socket() {
	return client_socket;
}

char *get_client_buffer() {
	return receive_buffer;
}

/*void send_data() {

}

void receive_data() {
	
}*/