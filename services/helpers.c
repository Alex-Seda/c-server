#include "helpers.h"
struct HeaderFields decodeHeader(char* request){
    struct HeaderFields header = {false, "", "/", false};

    int i=0, current=0;
    char line[50];

    // --- Get Request Type ---
    while(i<7){
        if(request[current] == ' '){
            current++; 
            break;
        }
        else{
            line[i] = request[current];
            i++;
            current++;
        }
    }
    line[i] = '\0'; // Set null terminator
    strncpy(header.requestType, line, i+1); // Copy temp string into header struct field
   
    // --- Get Page Requested ---
    i=0; // Reset loop counter, but keep "current" the same to track the current "cursor position
    while(i<39){
        if(request[current] == ' '){
            current++;
            break;
        }
        else{
            line[i] = request[current];
            i++;
            current++;
        }
    }
    line[i] = '\0'; // Set null terminator
    strncpy(header.page, line, i+1); // Copy temp string into header struct field

    // Remove this later. This is for ensuring correct parsing during development
    outputFields(&header);

    // TODO: ERROR CHECKING FOR PARSING (Set isValid to true for now)
    header.isValid = true;
    return header;
}



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
