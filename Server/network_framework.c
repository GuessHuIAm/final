#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <pthread.h> 
#include "communication_protocol.h"

int curr_capacity = 100;
int server_socket;
int curr_index = 0;
struct client {
	int socket;
	char receive_buffer[1024];
};
struct client **clients;

//accept each client in queue?
//listen/accept async problem

int get_client_socket(int client_ID) {
	return clients[client_ID]->socket;
}

char *get_client_buffer(int client_ID) {
	return clients[client_ID]->receive_buffer;
}

int get_last_index() {
	return curr_index;
}

void resize_clients() {
	struct client **new_clients = malloc(sizeof(long) * curr_capacity * 2);
	int i;
	for (i = 0; i <= curr_index; i++) {
		new_clients[i] = clients[i];
	}
	free(clients);

	clients = new_clients;
	curr_capacity *= 2;
}

void disconnect(int client_ID) {
	close(clients[client_ID]->socket);
	free(clients[client_ID]);
	clients[curr_index]->socket = -1 * client_ID;
	curr_index++;
	if (curr_index == curr_capacity - 1) {
		resize_clients();
	}
}

struct send_struct {int id; char *sdata;};

void send_function(struct send_struct p) {
	send(clients[p.id]->socket, p.sdata, strlen(p.sdata), 0);
}

void async_send(int client_ID, char *data) {
	struct send_struct p;
	p.id = client_ID;
	p.sdata = data;
	pthread_t sendt;
	pthread_create(&sendt, NULL, send_function, send_struct);
}

void receive_function(int client_ID) {
	while (1) {
		read(clients[client_ID]->socket, clients[client_ID]->receive_buffer, sizeof(clients[client_ID]->receive_buffer));
	}
}

void async_receive(int client_ID) {
	pthread_t receivet;
	pthread_create(&receivet, NULL, receive_function, client_ID);
}

void handle_connections() {
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
	while (1)  {
		listen(server_socket, 1000);

		int info_length = sizeof(binding_info);
		int new_client = accept(server_socket, (struct sockaddr *)&binding_info, (socklen_t *)&info_length);

		async_receive(new_client);

		if (clients[curr_index]->socket < 0) {
			clients[-1 * clients[curr_index]->socket]->socket = new_client;
			clients[curr_index]->socket = 0;
			if (clients[curr_index - 1]->socket < 0) {
				curr_index--;
			}
		} else {
			clients[curr_index]->socket = new_client;
			curr_index++;
			if (curr_index == curr_capacity - 1) {
				resize_clients();
			}
		}
	}
}

void reset_client_buffer(int curr_client) {
	char *buff = clients[curr_client]->receive_buffer;
	int i;
	for (i = 0; i <= 3; i++) {
		*(buff + i) = 0;
	}
}