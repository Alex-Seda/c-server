#include "parseRequest.h"

char *parseRequest(char* request){
    /*
     * Decode request fields into "HeaderFields" struct
     *
     * TODO:
     *
     * Middleware - logging, rate-limiting?, authentication?
     *  - Return errors as the HTTP Response if the request is invalid
     *  - Have a designated logging file(s)
     *
     * Routing - Get page requested
     *  - Routing table (file or struct) that would allow for adding/removing pages without altering server code
     *  - Return 404 if routing table returns nothing
     */

    struct HeaderFields inHeader = decodeHeader(request);


    /*
     * DEFINE THE HTTP HEADER
     *
     * The simplest HTTP header the HTTP version and the status code of the response
     *
     * 200 means OK (or a successful response)
     *
     * After the status, an empty new line is left to signify the start of the HTTP response body
     */
    char *outHeader = "HTTP/1.1 200\n\n";


    /*
     *
     * Get the HTML contents to return to the requestor
     *
     */


    /*
     * Find file name that corresponds to the requested page
     */
    printf("\n\ninHeader Page: %s\n\n", inHeader.page);
    int pageNameSize = strlen(inHeader.page);
    char *pageName = malloc(pageNameSize+1);
    strncpy(pageName,"public",6);

    // Append the page that was requested to the pageName string (direct "/" to "index.html")
    if(pageNameSize == 1) strncat(pageName,"/index.html",11); // TODO: Improve this matching logic to only take "/"
    else{
        strncat(pageName,inHeader.page,pageNameSize);
        strncat(pageName,".html",6);
    }
    //printf("\n\nRequesting Page with Filename: %s\n\n",pageName);


    /*
     * Read file at the parsed file path from above
     */
    // TODO: Return 404 on failed file open
    FILE *file = fopen(pageName, "rb"); // Open the index.html file for reading
    if(file == NULL){
        char* outHeader = "HTTP/1.1 404\n\n";
        char* body = "<!DOCTYPE html><html><main><h1>404 - Page Not Found</h1><p>We're sorry, we could not find the page you are looking for.</p><a href=\"/\">Go Home</a></main></html>";
        char* response = malloc(strlen(outHeader) + strlen(body) + 1);
        strcpy(response,outHeader);
        strncat(response,body,strlen(body));
        return response;
    }
    else{
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

        char *response = malloc(strlen(outHeader)+strlen(body)+1);

        // Copy the header into the beginning of the response
        strcpy(response, outHeader);

        /* Copy the body into the rest of the response, specifying to only copy bytes equal to the body's length
         * NOTES:
         *  - strncat() null terminates any space after the input string
         *  - We allocated enough space for one character more than the body length
         *  - Therefore, this function will null terminate our response string for us
         */
        strncat(response, body, strlen(body));

        free(body); // Since we have copied body now, we can free the memory we allocated

        return response; // Return the full response
    }
}

