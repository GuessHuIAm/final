#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <pthread.h> 
#include "network_framework.h"
#include "task_handler.h"

void start_server() {
	//Start handling client connections in a separate thread
	/*pthread_t connt;
	pthread_create(&connt, NULL, handle_connections, (void *)&connt);*/
	handle_connections();

	//Start the task handler in a separate thread
	pthread_t taskt;
	pthread_create(&taskt, NULL, start_task_handler, (void *)&taskt);
}

int main() {
	start_server();
	return 0;
}