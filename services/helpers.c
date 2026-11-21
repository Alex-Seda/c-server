#include "helpers.h"

void outputFields(struct HeaderFields *header){
    /*
     * Turn Boolean into String Output
     */
    char isSecure[6];

    if(header->secure==true) strncpy(isSecure,"True\0",5);
    else strncpy(isSecure,"False\0",6);
    

    /*
     * Output all fields of the header struct
     */
    printf("--- HEADER PARSING OUTPUT ---\n\n");
    printf("Request Type: %s\n", header->requestType);
    printf("Page Requested: %s\n", header->page);
    printf("Secure Connection: %s\n", isSecure);
    printf("\n-----------------------------\n\n\n\n");
}
