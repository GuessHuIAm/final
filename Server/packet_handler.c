#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include "network_framework.h"
#include "packet_handler.h"

void clean_buffer(struct client *curr_client) {
    printf("clean_buffer\n");
    char *buff = curr_client->receive_buffer;
    int i;
    for (i = 0; i < 1500; i++) {
        *(buff + i) = 0;
    }
    printf("clean_buffer O\n");
}

void append_request(char *buff, char *request, struct client *curr_client) {
    if (curr_client->num_requests <= 100) {
        char *i;
        int count = 0;
        char *curr_req = *(curr_client->requests + curr_client->num_requests - 1);
        for (i = request; i <= buff + 1499; i++) {
            *(curr_req + curr_client->last_index + count) = *i;
            count++;
        }
        curr_client->last_index += count;
    } else {
        curr_client->num_requests--;
    }
}

void handle_receive_packet(struct client *curr_client) {
    printf("handle_receive_packet\n");
    char *buff = curr_client->receive_buffer;
    char *request = buff;
    int found = 1;
    while (found || request != buff + 1500) {
        found = 0;
        char *i;
        for (i = request; i <= buff + 1499; i++) {
            if (*i == 15) { //End of packet delimeter
                *i = 0;
                if (!curr_client->incomplete) {
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
                    curr_client->incomplete = 0;
                    curr_client->last_index = 0;
                }
                if (*(i + 1) == 0) {
                    break;
                }
            }
        }
    }
    if (!found) {
        //EOP delimeter not found... must be an incomplete packet
        if (!curr_client->incomplete) {
            curr_client->num_requests++;
        }
        append_request(buff, request, curr_client);
        curr_client->incomplete = 1;
    }
    clean_buffer(curr_client);
    printf("handle_receive_packet O\n");
}

void send_packet(int client_ID, int REQUEST_ID, char **params, int num_params) {
    int buffer_size = 0;
    int i;
    for (i = 0; i < num_params; i++) {
        buffer_size += (strlen(*(params + i)) + 1);
    }
    char buffer[buffer_size + 6];

    char req_id[5];
	sprintf(req_id, "%04d", REQUEST_ID);
    strcat(buffer, req_id);

    for (i = 0; i < num_params; i++) {
        char del[2];
        del[0] = 14; //Parameter delimeter
        del[1] = 0;
        strcat(*(params + i), del);
        strcat(buffer, *(params + i));
    }

    buffer[buffer_size - 1] = 15; //EOP

    async_send(client_ID, buffer);
}

char **parse_packet(struct client *curr_client) {
    printf("parse_packet\n");
    int num_params = 0;
    char *buff = curr_client->receive_buffer + 4;
    int i;
    for (i = 0; i < 1500 - 4; i++) {
        if (*(buff + i) == 14) {
            num_params++;
        }
    }
    curr_client->num_params = num_params;
    char **params = malloc(sizeof(long) * num_params);
    char *param = buff;
    int count = 0;
    for (i = 0; i < 1500 - 4; i++) {
        if (*(buff + i) == 14) {
            *(buff + i) = 0;
            char curr_param[strlen(param) + 1];
            strcpy(curr_param, param);
            params[count] = curr_param;

            count++;
            param = buff + i + 1;
        }
    }
    printf("parse_packet O\n");
    return params;
}