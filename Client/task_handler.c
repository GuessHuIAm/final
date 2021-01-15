#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include "network_framework.h"
#include "communication_protocol.h"

void handle_response() { 
	char request_string[5];
	strncpy((char *)&request_string, get_client_buffer(), 5);
	char *ptr;
	int request = strtol(request_string, &ptr, 10);
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
}