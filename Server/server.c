#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include "network_framework.h"
#include "task_handler.h"

int main() {
	start_server();
	return 0;
}

void start_server() {
	//Start handling client connections in a separate thread
	pthread_t connt;
	pthread_create(&taskt, NULL, handle_connections, NULL);

	//Start the task handler in a separate thread
	pthread_t taskt;
	pthread_create(&taskt, NULL, start_task_handler, NULL);
}