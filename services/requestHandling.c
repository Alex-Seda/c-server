#include "requestHandling.h"

char *parseRequest(char *request){
    // TODO: Implement dynamic request parsing
    /*
     * Decode request fields - possibly make a struct for this
     *
     * Middleware - logging, rate-limiting?, authentication?
     *  - Return errors as the HTTP Response if the request is invalid
     *  - Have a designated logging file(s)
     *
     * Routing - Get page requested
     *  - Routing table (file or struct) that would allow for adding/removing pages without altering server code
     *  - Return 404 if routing table returns nothing
     */


    // TODO: Add security tightening and error checking/handling below
    /*
     * DEFINE THE HTTP HEADER
     *
     * The simplest HTTP header the HTTP version and the status code of the response
     *
     * 200 means OK (or a successful response)
     *
     * After the status, an empty new line is left to signify the start of the HTTP response body
     */
    char *header = "HTTP/1.1 200\n\n";


    /*
     * Get the HTML contents to return to the requestor
     */
    FILE *file = fopen("public/index.html", "rb"); // Open the index.html file for reading
    fseek(file, 0, SEEK_END); // Set the file pointer position to the end of the file with an initial offset of 0
    long fsize = ftell(file); // Get the current file position relative to the end of the file (this gets the file length)
    fseek(file, 0, SEEK_SET); // Set the file pointer position to the beginning of the file to prepare for reading (also with an initial offset of 0)

    char *body = malloc(fsize + 1); // Allocate enough memory for the file size plus the null terminator

    /*
     * READ HTML TEMPLATE
     *
     * Read into body (the character buffer where we will store the page to return to the requestor)
     * fsize items of data (this number is equal to the number of bytes in our file),
     * where the data is size 1 (1 byte),
     * from file (the page template)
     */
    fread(body, fsize, 1, file);

    fclose(file); // Close the file descriptor
    body[fsize] = '\0'; // Set the last character of the buffer to the null terminator

    /*
     * COMBINE HEADER AND BODY FOR HTTP RESPONSE
     *
     * Now we combine the header and body into one single response
     * To do this, we must reserve a larger chunk of memory:
     *   strlen(header): Length of the header string
     *   strlen(body): Length of the body string
     *   1: Space for the null terminating character
     */
    char *response = malloc(strlen(header)+strlen(body)+1);

    strcpy(response, header); // Copy the header into the beginning of the response
    strncat(response, body, strlen(body)); // Copy the body into the rest of the response, specifying to only copy bytes equal to the body's length

    free(body); // Since we have copied body now, we can free the memory we allocated

    return response; // Return the full response
}

struct HeaderFields decodeHeader(char* request){
    struct HeaderFields header; // Initialize a struct to return

}
