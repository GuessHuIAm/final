#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <pthread.h> 
#include <errno.h>
#include "communication_protocol.h"
#include "network_framework.h"
#include "packet_handler.h"

struct client *this_client;

void disconnect() {
	char **params = NULL;
	int num_params = 0;
	send_packet(DISCONNECT_REQUEST, params, num_params);

	close(this_client->socket);
}

void *send_function(void *p) {
	char *ps = (char *)p;
	printf("SENT DATA: ");
	int i = 0;
	while (*(ps + i) != 15) {
		printf("%c", *(ps + i));
		i++;
	}
	printf("\n");
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
		printf("READ DATA: ");
		int i = 0;
		while (*(curr_client->receive_buffer + i) != 15) {
			printf("%c", *(curr_client->receive_buffer + i));
			i++;
		}
		printf("\n");
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
	this_client->last_index = 0;
	this_client->num_params = 0;

	//Create the client socket
	if ((this_client->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("FAILED TO CREATE SOCKET: %s\n", strerror(errno));
		return;
	}

	//Prevent an address and port reuse error from occurring
	int opt = 1;
	if ((setsockopt(this_client->socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))) {
		printf("SETSOCKOPT FAILED: %s\n", strerror(errno));
		return;
	}

	//Connect to server address and port
	struct sockaddr_in binding_info; 
	binding_info.sin_family = AF_INET;
	if (inet_pton(AF_INET, "127.0.0.1", &binding_info.sin_addr) <= 0) {
		printf("INET_PTON FAILED: %s\n", strerror(errno));
		return;
	}
    binding_info.sin_port = htons(8000);
	if (connect(this_client->socket, (struct sockaddr *)&binding_info, sizeof(binding_info)) < 0) {
		printf("CONNECT FAILED: %s\n", strerror(errno));
		return;
	}
}

struct client *get_client() {
	return this_client;
}