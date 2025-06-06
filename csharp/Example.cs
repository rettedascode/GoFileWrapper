using System;
using System.Threading.Tasks;
using GoFile;

class Example
{
    static async Task Main(string[] args)
    {
        try
        {
            using var gofile = new GoFileWrapper();

            // Get server status
            Console.WriteLine("Getting server status...");
            var serverStatus = await gofile.GetServerStatusAsync();
            Console.WriteLine($"Server status: {serverStatus["status"]}");

            // Upload a file
            Console.WriteLine("\nUploading file...");
            var uploadResult = await gofile.UploadFileAsync("path/to/your/file.txt");
            var fileId = uploadResult["data"]["fileId"].GetString();
            Console.WriteLine($"File uploaded successfully. File ID: {fileId}");

            // Get file info
            Console.WriteLine("\nGetting file info...");
            var fileInfo = await gofile.GetFileInfoAsync(fileId);
            Console.WriteLine($"File name: {fileInfo["data"]["name"]}");
            Console.WriteLine($"File size: {fileInfo["data"]["size"]} bytes");

            // Delete the file
            Console.WriteLine("\nDeleting file...");
            var deleteResult = await gofile.DeleteFileAsync(fileId);
            Console.WriteLine($"File deletion {(deleteResult ? "successful" : "failed")}");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Error: {ex.Message}");
        }
    }
} 