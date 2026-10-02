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


    /*
     * CREATE THE LISTENING SOCKET
     *
     * socket() returns the file descriptor of the new socket on success
     * On failure, it returns -1
     *
     * The domain is AF_INET, which means that this socket will transmit with IPv4 addresses
     *
     * The type is set to SOCK_STREAM, which means it will be used to transmit sequenced, reliable, two-way byte streams
     *
     * The protocol is set to 0
     * There is usually only one protocol per communication domain, but if there are multiple, then this must be specified
     * 0 is the default
     */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { // If socket() returns -1, then the socket creation failed
        perror("socket failed"); // Print 'socket failed' and then the error in errno from the failed socket() call
        exit(EXIT_FAILURE); // Exit program execution with a failure code
    }

    /*
     * SET SOCKET OPTIONS
     *
     * setsockopt() returns 0 on success and -1 on failure
     *
     * The socket to be changed is server_fd, the one we just created
     *
     * The level is SOL_SOCKET, which sets these options at the socket level
     *
     * The options we are going to change are:
     *   - SO_REUSEADDR
     *     - When TCP connections handshake, the OS may not consider the connection closed after the last ACK packet
     *     - Usually there is a TIME WAIT until it is considered closed, and the OS will not normally allow multiple connections to the same IP address
     *     - In this situation, further connections to the address may be blocked until the current connection is considered closed
     *     - When we set this option to true, it tells the OS to let multiple connections to have the same address, avoiding the denial of service
     *   - SO_REUSEPORT
     *     - This option is similar to reuseaddr, but more restrictive
     *     - For a deeper explanation, see the direct documentation or https://stackoverflow.com/questions/14388706/how-do-so-reuseaddr-and-so-reuseport-differ
     *
     *  The option is set to the value stored at the memory address for opt, which is 1 in our case
     *  This sets them to True
     *
     *  The last argument is the size of the memory stored at the location passed in the previous argument "sizeof(opt)"
     *  This lets the function know what size to expect the data to be
     */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) < 0) { // If setsocketopt() returns -1, then it failed
        perror("setsockopt"); // Print 'setsockopt' and then the error in errno from the failed setsocketopt() call
        exit(EXIT_FAILURE); // Exit program execution with a failure code
    }


    /*
     * SET ADDRESS SPECIFICATIONS
     *
     * This prepares the address struct to be attached to the socket from earlier
     * This is how other programs will find the socket
     */
    address.sin_family = AF_INET; // Use the IPv4 address family
    address.sin_addr.s_addr = INADDR_ANY; // Use any host IPv4 address available (i.e. localhost, 127.0.0.1, 0.0.0.0, or any public/private device IP)
    address.sin_port = htons(PORT); // Use the defined port constant (change from little endian (local device convention) to big endian (networking convention))

    /*
     * BIND THE ADDRESS TO THE SOCKET
     *
     * When a socket is created, it exists with the specification that it is intended for a certain namespace (AF_INET)
     * However, it does not actually have an address assigned to it until we bind an address to the socket
     *
     * bind() returns 0 on success and -1 on failure
     *
     * The socket to be bound to is server_fd, the socket we opened earlier
     *
     * The address that we are assigning to that socket is the address stored at our address variable's memory location
     *
     * Finally, we tell bind() what size of an sockaddr struct to expect so it knows how much information to expect
     */
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { // If bind() returns -1, then it failed
        perror("bind failed"); // Print 'bind failed' and then the error in errno from the failed bind() call
        exit(EXIT_FAILURE); // Exit program execution with a failure code
    }

    /*
     * DESIGNATE SOCKET AS A LISTENING SOCKET
     *
     * listen() returns 0 on success and -1 on failure
     *
     * This marks the socket server_fd as a listening socket with an available backlog of 3
     * The backlog argument defines the maximum length that the queue of pending connections may reach before the server refuses connections
     */
    if (listen(server_fd, 3) < 0) { // If listen() returns -1, then it failed
        perror("listen"); // Print 'listen' and then the error in errno from the failed listen() call
        exit(EXIT_FAILURE); // Exit program execution with a failure code
    }


    /*
     * CONNECTION LOOP
     *
     * This loop waits for a connection request, makes a new socket for it, handles the request, responds to the requestor, then closes the connection
     * At the moment, there is no concurrency, and the server only accepts one request then exits its loop
     */
    for(int i=0; i<1; i++){
        // TODO: Add security tightening and error checking/handling below


        /*
         * CREATE NEW SOCKET FOR COMMUNICATION
         *
         * The server will wait here until there is a connection request on the socket
         *
         * When there is a request, it will make a new socket for that connection so that the listening socket can continue listening
         *
         * On a real server, you would likely multithread this portion for concurrent request handling
         */
        new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

        /*
         * READ BYTE STREAM
         *
         * Read up to 1023 bytes of information from the socket into the buffer
         *
         * We subtract 1 for the null terminator at the end of the buffer
         *
         * read() returns the number of bytes it read, and we set valread to that number
         */
	    valread = read(new_socket, buffer, 1024 - 1);


        /*
         * This optional line prints the number of bytes read
         * This is useful when debugging or if you would like to tailor the buffer to take up minimal space while being large enough to take requests
         */
        // printf("%zd bytes read\n",valread);


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
