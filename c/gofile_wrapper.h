#ifndef GOFILE_WRAPPER_H
#define GOFILE_WRAPPER_H

#include <stdbool.h>

// Structure to hold GoFile client context
typedef struct {
    char* base_url;
    void* curl;  // CURL handle
} GoFileClient;

// Initialize a new GoFile client
GoFileClient* gofile_init(void);

// Clean up the GoFile client
void gofile_cleanup(GoFileClient* client);

// Upload a file
char* gofile_upload_file(GoFileClient* client, const char* file_path);

// Get server status
char* gofile_get_server_status(GoFileClient* client);

// Get file info
char* gofile_get_file_info(GoFileClient* client, const char* file_id);

// Delete a file
bool gofile_delete_file(GoFileClient* client, const char* file_id);

// Free memory allocated by the wrapper functions
void gofile_free_response(char* response);

#endif // GOFILE_WRAPPER_H 