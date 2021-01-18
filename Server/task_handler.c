#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include "network_framework.h"
#include "packet_handler.h"
#include "communication_protocol.h"

void *start_task_handler(void *vargp) {
	while (1) { 
		int curr_client;
		for (curr_client = 0; curr_client <= get_last_index(); curr_client++) {
			struct client *this_client = get_client(curr_client);
			if (this_client->receive_buffer[0] != 0) {
				handle_receive_packet(this_client);
			}
			if (this_client->num_requests > 0) {
				char *message = *(this_client->requests + this_client->num_requests - 1);

				char request_string[5];
				strncpy((char *)&request_string, message, 5);
				char *ptr;
				int request = strtol(request_string, &ptr, 10);
				switch(request) {
					case REGISTER_REQUEST :
						break;

					case LOGIN_REQUEST :
						break;

					case VIEW_REQUEST :
						break;

					case SOLUTION_REQUEST :
						break;

					case SUBMISSION_REQUEST :
						break;

					case DISCONNECT_REQUEST :
						disconnect(curr_client);
						break;
				}

				this_client->num_requests--;
			}
		}
	}
}