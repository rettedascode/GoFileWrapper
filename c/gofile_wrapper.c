#include "gofile_wrapper.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

struct MemoryStruct {
    char* memory;
    size_t size;
};

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct* mem = (struct MemoryStruct*)userp;

    char* ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) {
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

GoFileClient* gofile_init(void) {
    GoFileClient* client = (GoFileClient*)malloc(sizeof(GoFileClient));
    if (!client) return NULL;

    client->curl = curl_easy_init();
    if (!client->curl) {
        free(client);
        return NULL;
    }

    client->base_url = strdup("https://api.gofile.io");
    return client;
}

void gofile_cleanup(GoFileClient* client) {
    if (client) {
        if (client->curl) {
            curl_easy_cleanup(client->curl);
        }
        free(client->base_url);
        free(client);
    }
}

static char* perform_request(GoFileClient* client, const char* endpoint, const char* params) {
    if (!client || !client->curl) return NULL;

    char url[1024];
    if (params) {
        snprintf(url, sizeof(url), "%s%s?%s", client->base_url, endpoint, params);
    } else {
        snprintf(url, sizeof(url), "%s%s", client->base_url, endpoint);
    }

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_easy_setopt(client->curl, CURLOPT_URL, url);
    curl_easy_setopt(client->curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(client->curl, CURLOPT_WRITEDATA, (void*)&chunk);

    CURLcode res = curl_easy_perform(client->curl);
    if (res != CURLE_OK) {
        free(chunk.memory);
        return NULL;
    }

    return chunk.memory;
}

char* gofile_upload_file(GoFileClient* client, const char* file_path) {
    if (!client || !client->curl || !file_path) return NULL;

    struct curl_httppost* formpost = NULL;
    struct curl_httppost* lastptr = NULL;

    curl_formadd(&formpost, &lastptr,
        CURLFORM_COPYNAME, "file",
        CURLFORM_FILE, file_path,
        CURLFORM_END);

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    char url[1024];
    snprintf(url, sizeof(url), "%s/uploadFile", client->base_url);

    curl_easy_setopt(client->curl, CURLOPT_URL, url);
    curl_easy_setopt(client->curl, CURLOPT_HTTPPOST, formpost);
    curl_easy_setopt(client->curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(client->curl, CURLOPT_WRITEDATA, (void*)&chunk);

    CURLcode res = curl_easy_perform(client->curl);
    curl_formfree(formpost);

    if (res != CURLE_OK) {
        free(chunk.memory);
        return NULL;
    }

    return chunk.memory;
}

char* gofile_get_server_status(GoFileClient* client) {
    return perform_request(client, "/getServer", NULL);
}

char* gofile_get_file_info(GoFileClient* client, const char* file_id) {
    if (!file_id) return NULL;
    char params[256];
    snprintf(params, sizeof(params), "fileId=%s", file_id);
    return perform_request(client, "/getFile", params);
}

bool gofile_delete_file(GoFileClient* client, const char* file_id) {
    if (!file_id) return false;
    char params[256];
    snprintf(params, sizeof(params), "fileId=%s", file_id);
    char* response = perform_request(client, "/deleteFile", params);
    if (!response) return false;

    bool success = (strstr(response, "success") != NULL);
    free(response);
    return success;
}

void gofile_free_response(char* response) {
    free(response);
} 