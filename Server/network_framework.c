#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <pthread.h> 
#include "communication_protocol.h"
#include "network_framework.h"
#include "packet_handler.h"

int curr_capacity = 100;
int server_socket;
int curr_index = -1;
struct client **clients;
struct send_struct {int id; char *sdata;};

//protect shared memory (mutex locks)
//add alive packets
//client 0 disconnecting wastes some memory
//need actual async sends/receives

struct client *get_client(int client_ID) {
	return clients[client_ID];
}

int get_last_index() {
	return curr_index;
}

void resize_clients() {
	clients = realloc(clients, sizeof(long) * curr_capacity * 2);
	curr_capacity *= 2;
}

void disconnect(int client_ID) {
	printf("SUPPP\n");
	close(clients[client_ID]->socket);
	free(clients[client_ID]);
	clients[curr_index]->socket = -1 * client_ID;
	curr_index++;
	if (curr_index == curr_capacity - 1) {
		resize_clients();
	}
	printf("Client %d disconnected\n", client_ID);
}

void *send_function(void *p) {
	struct send_struct *ps = (struct send_struct *)p;
	send(clients[ps->id]->socket, ps->sdata, strlen(ps->sdata), 0);
	return NULL;
}

void async_send(int client_ID, char *data) {
	struct send_struct *p;
	p->id = client_ID;
	p->sdata = data;
	pthread_t sendt;
	pthread_create(&sendt, NULL, send_function, (void *)p);
}

void *receive_function(void *rclient) {
	struct client *curr_client = (struct client *)rclient;
	while (1) {
		read(curr_client->socket, curr_client->receive_buffer, sizeof(curr_client->receive_buffer));
	}
	return NULL;
}

void async_receive(int client_ID) {
	pthread_t receivet;
	pthread_create(&receivet, NULL, receive_function, (void *)clients[client_ID]);
}

void *handle_connections(void *vargp) {
	clients = malloc(sizeof(long) * curr_capacity);

	//Create the server socket
	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	//Prevent an address and port reuse error from occurring
	int opt = 1;
	setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	//Bind socket to address and port
	struct sockaddr_in binding_info; 
	binding_info.sin_family = AF_INET;
	binding_info.sin_addr.s_addr = INADDR_ANY; 
    binding_info.sin_port = htons(8000);
	bind(server_socket, (struct sockaddr *)&binding_info, sizeof(binding_info));
	
	//Start accepting incoming client connections
	listen(server_socket, 1000);

	int info_length = sizeof(binding_info);
	int new_socket;
	int client_ID;
	while ((new_socket = accept(server_socket, (struct sockaddr *)&binding_info, (socklen_t *)&info_length))) {
		if (curr_index != -1 && clients[curr_index]->socket < 0) {
			struct client new_client;
			clients[-1 * clients[curr_index]->socket] = &new_client;
			clients[-1 * clients[curr_index]->socket]->socket = new_socket;
			client_ID = -1 * clients[curr_index]->socket;
			clean_buffer(clients[-1 * clients[curr_index]->socket]);
			clients[-1 * clients[curr_index]->socket]->num_requests = 0;
			clients[-1 * clients[curr_index]->socket]->last_index = 0;
			clients[-1 * clients[curr_index]->socket]->num_params = 0;
			clients[curr_index]->socket = 0;
			if (clients[curr_index - 1]->socket < 0) {
				curr_index--;
			}
		} else {
			struct client new_client;
			curr_index++;
			clients[curr_index] = &new_client;
			clients[curr_index]->socket = new_socket;
			client_ID = curr_index;
			clean_buffer(clients[curr_index]);
			clients[curr_index]->num_requests = 0;
			clients[curr_index]->last_index = 0;
			clients[curr_index]->num_params = 0;
			if (curr_index == curr_capacity - 1) {
				resize_clients();
			}
		}

		printf("Client %d connected\n", client_ID);

		async_receive(client_ID);
	}
	return NULL;
}