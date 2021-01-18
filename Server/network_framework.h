struct client {
	int socket;
	char receive_buffer[1500]; //TCP MTU size
	char requests[100][1024];
	int num_requests;
};
struct client *get_client(int client_ID);
int get_last_index();
void *handle_connections();
void disconnect(int client_ID);
void async_send(int client_ID, char *data);
void async_receive(int client_ID);