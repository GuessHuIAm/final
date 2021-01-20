struct client {
	int socket;
	char receive_buffer[1500]; //TCP MTU size
	char requests[100][1024];
	int num_requests;
	int num_params;
	int incomplete;
	int last_index;
};
struct client *get_client();
void connect_server();
void disconnect();
void async_receive();
void async_send(char *data);