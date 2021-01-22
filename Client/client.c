#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <signal.h> 
#include <sys/types.h>
#include <unistd.h>
#include "network_framework.h"
#include "task_handler.h"

void handle_signal(int sig) { 
	printf("In signal handler\n");
    disconnect(); 
} 

int main() {
	signal(SIGINT, handle_signal);
	printf("Connecting to server...\n");
	connect_server();
	printf("Connection established\n");
	sleep(5);
	kill(getpid(), SIGINT);
	return 0;
}