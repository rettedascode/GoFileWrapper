#include <stdio.h>
#include <stdlib.h>
#include "../gofile_wrapper.h"

int main() {
    GoFileClient* client = gofile_init();
    if (!client) {
        fprintf(stderr, "Failed to initialize GoFile client\n");
        return 1;
    }

    // Get server status
    printf("Getting server status...\n");
    char* serverStatus = gofile_get_server_status(client);
    if (serverStatus) {
        printf("Server status: %s\n", serverStatus);
        gofile_free_response(serverStatus);
    }

    // Upload a file
    printf("\nUploading file...\n");
    char* uploadResult = gofile_upload_file(client, "path/to/your/file.txt");
    if (uploadResult) {
        printf("Upload result: %s\n", uploadResult);
        gofile_free_response(uploadResult);
    }

    // Get file info (replace with actual file ID from upload)
    printf("\nGetting file info...\n");
    char* fileInfo = gofile_get_file_info(client, "your_file_id_here");
    if (fileInfo) {
        printf("File info: %s\n", fileInfo);
        gofile_free_response(fileInfo);
    }

    // Delete the file
    printf("\nDeleting file...\n");
    bool deleteResult = gofile_delete_file(client, "your_file_id_here");
    printf("File deletion %s\n", deleteResult ? "successful" : "failed");

    // Cleanup
    gofile_cleanup(client);
    return 0;
} 