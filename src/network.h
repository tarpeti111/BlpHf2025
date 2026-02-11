#ifndef NETWORK_H
#define NETWORK_H

#include <curl/curl.h>

typedef struct{
    CURL* curl;
    char server_url[256];
}Network;

void network_init(Network* net);
void network_cleanup(Network* net);
void network_send_score(Network* net, char* name, char* score);

#endif