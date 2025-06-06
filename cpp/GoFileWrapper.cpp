#include "GoFileWrapper.hpp"
#include <sstream>
#include <fstream>
#include <iostream>

GoFileWrapper::GoFileWrapper() {
    curl = curl_easy_init();
    baseUrl = "https://api.gofile.io";
}

GoFileWrapper::~GoFileWrapper() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
}

std::string GoFileWrapper::uploadFile(const std::string& filePath) {
    if (!curl) return "Error: CURL not initialized";

    std::string server = getServerStatus();
    if (server.empty()) return "Error: Could not get server";

    CURLcode res;
    struct curl_httppost* formpost = NULL;
    struct curl_httppost* lastptr = NULL;

    // Add file to form
    curl_formadd(&formpost, &lastptr,
        CURLFORM_COPYNAME, "file",
        CURLFORM_FILE, filePath.c_str(),
        CURLFORM_END);

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, (baseUrl + "/uploadFile").c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    curl_formfree(formpost);

    if (res != CURLE_OK) {
        return "Error: " + std::string(curl_easy_strerror(res));
    }

    return response;
}

std::string GoFileWrapper::getServerStatus() {
    return performRequest("/getServer");
}

std::string GoFileWrapper::getFileInfo(const std::string& fileId) {
    std::map<std::string, std::string> params;
    params["fileId"] = fileId;
    return performRequest("/getFile", params);
}

bool GoFileWrapper::deleteFile(const std::string& fileId) {
    std::map<std::string, std::string> params;
    params["fileId"] = fileId;
    std::string response = performRequest("/deleteFile", params);
    return response.find("success") != std::string::npos;
}

std::string GoFileWrapper::performRequest(const std::string& endpoint, const std::map<std::string, std::string>& params) {
    if (!curl) return "Error: CURL not initialized";

    std::string url = baseUrl + endpoint;
    if (!params.empty()) {
        url += "?";
        for (const auto& param : params) {
            url += param.first + "=" + param.second + "&";
        }
        url.pop_back(); // Remove last &
    }

    std::string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        return "Error: " + std::string(curl_easy_strerror(res));
    }

    return response;
}

size_t GoFileWrapper::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t realsize = size * nmemb;
    userp->append((char*)contents, realsize);
    return realsize;
} 