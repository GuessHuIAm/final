#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include "network_framework.h"
#include "packet_handler.h"

int incomplete = 0;
int last_index = 0;

void clean_buffer(struct client *curr_client) {
    char *buff = curr_client->receive_buffer;
    int i;
    for (i = 0; i < 1500; i++) {
        *(buff + i) = 0;
    }
}

void append_request(char *buff, char *request, struct client *curr_client) {
    if (curr_client->num_requests <= 100) {
        char *i;
        int count = 0;
        char *curr_req = *(curr_client->requests + curr_client->num_requests - 1);
        for (i = request; i <= buff + 1499; i++) {
            *(curr_req + last_index + count) = *i;
            count++;
        }
        last_index += count;
    } else {
        curr_client->num_requests--;
    }
}

void handle_receive_packet(struct client *curr_client) {
    char *buff = curr_client->receive_buffer;
    char *request = buff;
    int found = 1;
    while (found || request != buff + 1500) {
        found = 0;
        char *i;
        for (i = request; i <= buff + 1499; i++) {
            if (*i == 15) { //End of packet delimeter
                *i = 0;
                if (!incomplete) {
                    curr_client->num_requests++;
                    if (curr_client->num_requests <= 100) {
                        char *curr_req = *(curr_client->requests + curr_client->num_requests - 1);
                        strcpy(curr_req, request);
                    } else {
                        curr_client->num_requests--;
                    }
                    request = i + 1;
                    found = 1;
                } else {
                    append_request(buff, request, curr_client);
                    incomplete = 0;
                    last_index = 0;
                }
                if (*(i + 1) == 0) {
                    break;
                }
            }
        }
    }
    if (!found) {
        //EOP delimeter not found... must be an incomplete packet
        if (!incomplete) {
            curr_client->num_requests++;
        }
        append_request(buff, request, curr_client);
        incomplete = 1;
    }
    clean_buffer(curr_client);
}

void send_packet(int REQUEST_ID, char **params, int num_params) {
    int buffer_size = 0;
    for (int i = 0; i < num_params; i++) {
        buffer_size += strlen(*(params + i));
    }
    char buffer[buffer_size + 6];

    char req_id[5];
	sprintf(req_id, "%04d", REQUEST_ID);
    strcat(buffer, req_id);

    char eop[1];
    eop[0] = 15; //End of packet delimeter
    strcat(buffer, eop);

    async_send(buffer);
}