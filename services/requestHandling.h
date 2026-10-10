#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* parseRequest(char *request);

struct HeaderFields {
    bool isValid; // For now, this will act as a safety net. If the parsing function encounters any issues or unexpected formatting, declare the request invalid as a security measure. This should be basic security for now
    char* requestType[8]; // The longest HTTP request type is 7 characters, so we allocate that plus the 1 null terminator
    char* page[40]; // Defined arbitrarily as a 40 character array for now. can be adjusted later to be dynamic based on routes?
    bool secure; // This can be used later when https is implemented
    // version; // Reassess later if HTTP version will be used for anything
    // host; // This can be used later if multiple applications will use this server so the server can act as a reverse proxy
} HeaderFields_defaultSettings = {false,"","/",false};

