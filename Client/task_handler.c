#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include "network_framework.h"
#include "packet_handler.h"
#include "communication_protocol.h"

void handle_response() { 
	struct client *this_client = get_client();
	if (this_client->receive_buffer[0] != 0) {
		handle_receive_packet(this_client);
	}
	if (this_client->num_requests > 100) {
		//connection is too unstable
		disconnect();
	}
	int i;
	for (i = 0; i < this_client->num_requests; i++) {
		if (this_client->incomplete && i == this_client->num_requests - 1) {
			break;
		}
		char *message = *(this_client->requests + i);

		char request_string[5];
		request_string[4] = 0;
		strncpy((char *)&request_string, message, 5);
		char *ptr;
		int request = strtol(request_string, &ptr, 10);

		char **params = parse_packet(this_client);
		int num_params = this_client->num_params;
		switch(request) {
			case REGISTER_RESPONSE :
				break;

			case LOGIN_RESPONSE :
				break;

			case VIEW_RESPONSE :
				break;

			case SOLUTION_RESPONSE :
				break;

			case SUBMISSION_RESPONSE :
				break;
		}
		free(params);
	}
	this_client->num_requests = 0;
}