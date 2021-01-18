void handle_receive_packet(struct client *curr_client);
void clean_buffer(struct client *curr_client);
void send_packet(int client_ID, int REQUEST_ID, char **params, int num_params);