#include <iostream>
#include "../GoFileWrapper.hpp"

int main() {
    try {
        GoFileWrapper wrapper;

        // Get server status
        std::cout << "Getting server status..." << std::endl;
        std::string serverStatus = wrapper.getServerStatus();
        std::cout << "Server status: " << serverStatus << std::endl;

        // Upload a file
        std::cout << "\nUploading file..." << std::endl;
        std::string uploadResult = wrapper.uploadFile("path/to/your/file.txt");
        std::cout << "Upload result: " << uploadResult << std::endl;

        // Get file info (replace with actual file ID from upload)
        std::cout << "\nGetting file info..." << std::endl;
        std::string fileInfo = wrapper.getFileInfo("your_file_id_here");
        std::cout << "File info: " << fileInfo << std::endl;

        // Delete the file
        std::cout << "\nDeleting file..." << std::endl;
        bool deleteResult = wrapper.deleteFile("your_file_id_here");
        std::cout << "File deletion " << (deleteResult ? "successful" : "failed") << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 