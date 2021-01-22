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
				printf("Something happened, num requests is %d\n", this_client->num_requests);
				handle_receive_packet(this_client);
			}
			if (this_client->num_requests > 100) {
				//client may be trying to overwhelm server maliciously or connection is too unstable
				disconnect(curr_client);
			}
			int i;
			for (i = 0; i < this_client->num_requests; i++) {
				printf("Hol up...\n");
				if (this_client->incomplete && i == this_client->num_requests - 1) {
					break;
				}
				char *message = *(this_client->requests + i);

				char request_string[5];
				request_string[4] = 0;
				strncpy((char *)&request_string, message, 4);
				char *ptr;
				int request = strtol(request_string, &ptr, 10);

				char **params = parse_packet(this_client);
				int num_params = this_client->num_params;
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
						printf("Aight we here in the switch\n");
						disconnect(curr_client);
						break;
				}
				free(params);
			}
			this_client->num_requests = 0;
		}
	}
}