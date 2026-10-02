#include "server.h"

int main(int argc, char const *argv[])
{
    int server_fd, new_socket; // Initialize variables for server file descriptor and new socket file descriptor
    ssize_t valread; // Initialize variable for read() return value when reading the socket into the buffer
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = { 0 }; // Initialize buffer for reading the socket
    char *response; // Initialize variable for the server response

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET; // Use the IPv4 address family
    address.sin_addr.s_addr = INADDR_ANY; // Use any IPv4 address available (i.e. localhost, 127.0.0.1, 0.0.0.0, or any public/private device IP)
    address.sin_port = htons(PORT); // Use the defined port constant (change from little endian (local device convention) to big endian (networking convention))

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    for(int i=0; i<1; i++){
        // TODO: Add security tightening and error checking/handling below


        /*
         * The server will wait here until there is a connection request on the socket
         *
         * When there is a request, it will make a new socket for that connection so that the listening socket can continue listening
         *
         * On a real server, you would likely multithread this portion for concurrent request handling
         */
        new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

        /*
         * Read up to 1023 bytes of information from the socket into the buffer
         *
         * We subtract 1 for the null terminator at the end of the buffer
         *
         * read() returns the number of bytes it read, and we set valread to that number
         */
	    valread = read(new_socket, buffer, 1024 - 1);

        // Explicitly set the last character to the null terminator
        buffer[valread] = '\0';

        // Print the request to terminal
        printf("Request received:\n%s\n", buffer);

        // Get the response based on the request
        response = parseRequest(buffer);

        // Send server response on the socket to the requestor
        send(new_socket, response, strlen(response), 0);
	    
        // Print server send success message
        printf("Server Response Sent\n\n\n");

        // Free the memory that was allocated for the response in "parseRequest"
        free(response);

	    // Close the connected socket
	    close(new_socket);
    }


    // Close the listening socket
    close(server_fd);

    // Return Success code
    return EXIT_SUCCESS;
}


char *parseRequest(char *request){
    // TODO: Implement dynamic request parsing


    // TODO: Add security tightening and error checking/handling below
    /*
     * Define the HTTP header
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
     * Read into body (the character buffer where we will store the page to return to the requestor)
     * fsize items of data (this number is equal to the number of bytes in our file),
     * where the data is size 1 (1 byte),
     * from file (the page template)
     */
    fread(body, fsize, 1, file);

    fclose(file); // Close the file descriptor
    body[fsize] = '\0'; // Set the last character of the buffer to the null terminator

    /*
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
