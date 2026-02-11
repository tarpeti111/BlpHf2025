#include "network.h"
#include <string.h>

void network_init(Network* net){
    net->curl = curl_easy_init();
    strcpy(net->server_url, "localhost:8080");
    return;
}

void network_cleanup(Network* net){
    if (net->curl){
        curl_easy_cleanup(net->curl);
    }
}

void network_send_score(Network* net, char* name, char* score){
    CURL* curl = net->curl;
    char* url = net->server_url;

    if(!curl){
        perror("Curl not initialized\n");
        return;
    }

    if(!url){
        perror("No url was provided\n");
        return;
    }

    char postfields[256];
    snprintf(postfields, sizeof(postfields), "name=%s&score=%s", name, score);

    FILE* devnull = fopen("/dev/null", "w+");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postfields);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, devnull);

    CURLcode res = curl_easy_perform(curl);
    if(res != CURLE_OK) {
        fprintf(stderr, "curl error: %s\n", curl_easy_strerror(res));
    }
    fclose(devnull);
}