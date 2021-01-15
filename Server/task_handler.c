#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include "network_framework.h"
#include "communication_protocol.h"

void start_task_handler() {
	while (1) { 
		int curr_client;
		for (curr_client = 0; curr_client <= get_last_index(); curr_client++) {
			char *message = get_client_buffer(curr_client);
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
			reset_client_buffer(curr_client);
		}
	}
}