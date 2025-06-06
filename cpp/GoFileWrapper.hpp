#ifndef GOFILE_WRAPPER_HPP
#define GOFILE_WRAPPER_HPP

#include <string>
#include <vector>
#include <map>
#include <curl/curl.h>

class GoFileWrapper {
public:
    GoFileWrapper();
    ~GoFileWrapper();

    // Upload a file
    std::string uploadFile(const std::string& filePath);
    
    // Get server status
    std::string getServerStatus();
    
    // Get file info
    std::string getFileInfo(const std::string& fileId);
    
    // Delete a file
    bool deleteFile(const std::string& fileId);

private:
    CURL* curl;
    std::string baseUrl;
    
    // Helper methods
    std::string performRequest(const std::string& endpoint, const std::map<std::string, std::string>& params = {});
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);
};

#endif // GOFILE_WRAPPER_HPP 