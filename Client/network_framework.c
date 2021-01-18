#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <pthread.h> 
#include "communication_protocol.h"
#include "network_framework.h"
#include "packet_handler.h"

struct client *this_client;

void disconnect() {
	char **params = NULL;
	int num_params = 0;
	send_packet(DISCONNECT_REQUEST, params, num_params);

	send(this_client->socket, req, sizeof(req), 0);
	close(this_client->socket);
}

void *send_function(void *p) {
	char *ps = (char *)p;
	send(this_client->socket, ps, strlen(ps), 0);
	return NULL;
}

void async_send(char *data) {
	pthread_t sendt;
	pthread_create(&sendt, NULL, send_function, (void *)data);
}

void *receive_function(void *rclient) {
	struct client *curr_client = (struct client *)rclient;
	while (1) {
		read(curr_client->socket, curr_client->receive_buffer, sizeof(curr_client->receive_buffer));
	}
	return NULL;
}

void async_receive() {
	pthread_t receivet;
	pthread_create(&receivet, NULL, receive_function, (void *)this_client);
}

void connect_server() {
	struct client new_client;
	this_client = &new_client;
	clean_buffer(this_client);
	this_client->num_requests = 0;

	//Create the client socket
	this_client->socket = socket(AF_INET, SOCK_STREAM, 0);

	//Prevent an address and port reuse error from occurring
	int opt = 1;
	setsockopt(this_client->socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	//Connect to server address and port
	struct sockaddr_in binding_info; 
	binding_info.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &binding_info.sin_addr);
    binding_info.sin_port = htons(8000);
	connect(this_client->socket, (struct sockaddr *)&binding_info, sizeof(binding_info));
}

struct client *get_client() {
	return this_client;
}