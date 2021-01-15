int get_client_socket(int client_ID);
char *get_client_buffer(int client_ID);
int get_last_index();
void handle_connections();
void disconnect(int client_ID);
void reset_client_buffer(int curr_client);