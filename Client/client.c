#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <signal.h> 
#include "network_framework.h"
#include "task_handler.h"

void handle_signal(int sig) { 
    disconnect(); 
} 

int main() {
	signal(SIGINT, handle_signal);
	printf("Connecting to server...\n");
	connect_server();
	printf("Connection established\n");
	int i;
	while (1) {
		i++;
	}
	return 0;
}