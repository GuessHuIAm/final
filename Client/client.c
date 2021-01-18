#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include "network_framework.h"
#include "task_handler.h"

int main() {
	printf("Connecting to server...\n");
	connect_server();
	printf("Connection established\n");
	return 0;
}