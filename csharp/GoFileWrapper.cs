using System;
using System.IO;
using System.Net.Http;
using System.Threading.Tasks;
using System.Text.Json;
using System.Collections.Generic;

namespace GoFile
{
    public class GoFileWrapper : IDisposable
    {
        private readonly HttpClient _httpClient;
        private readonly string _baseUrl;
        private bool _disposed;

        public GoFileWrapper()
        {
            _httpClient = new HttpClient();
            _baseUrl = "https://api.gofile.io";
        }

        /// <summary>
        /// Uploads a file to GoFile
        /// </summary>
        /// <param name="filePath">Path to the file to upload</param>
        /// <returns>Response from the server as a dictionary</returns>
        public async Task<Dictionary<string, JsonElement>> UploadFileAsync(string filePath)
        {
            if (!File.Exists(filePath))
            {
                throw new FileNotFoundException($"File not found: {filePath}");
            }

            var server = await GetServerStatusAsync();
            if (server == null)
            {
                throw new Exception("Could not get server status");
            }

            using var form = new MultipartFormDataContent();
            using var fileStream = File.OpenRead(filePath);
            using var streamContent = new StreamContent(fileStream);
            form.Add(streamContent, "file", Path.GetFileName(filePath));

            var response = await _httpClient.PostAsync($"{_baseUrl}/uploadFile", form);
            response.EnsureSuccessStatusCode();

            var content = await response.Content.ReadAsStringAsync();
            return JsonSerializer.Deserialize<Dictionary<string, JsonElement>>(content);
        }

        /// <summary>
        /// Gets the server status
        /// </summary>
        /// <returns>Server status information as a dictionary</returns>
        public async Task<Dictionary<string, JsonElement>> GetServerStatusAsync()
        {
            var response = await _httpClient.GetAsync($"{_baseUrl}/getServer");
            response.EnsureSuccessStatusCode();

            var content = await response.Content.ReadAsStringAsync();
            return JsonSerializer.Deserialize<Dictionary<string, JsonElement>>(content);
        }

        /// <summary>
        /// Gets information about a file
        /// </summary>
        /// <param name="fileId">The ID of the file</param>
        /// <returns>File information as a dictionary</returns>
        public async Task<Dictionary<string, JsonElement>> GetFileInfoAsync(string fileId)
        {
            if (string.IsNullOrEmpty(fileId))
            {
                throw new ArgumentNullException(nameof(fileId));
            }

            var response = await _httpClient.GetAsync($"{_baseUrl}/getFile?fileId={Uri.EscapeDataString(fileId)}");
            response.EnsureSuccessStatusCode();

            var content = await response.Content.ReadAsStringAsync();
            return JsonSerializer.Deserialize<Dictionary<string, JsonElement>>(content);
        }

        /// <summary>
        /// Deletes a file
        /// </summary>
        /// <param name="fileId">The ID of the file to delete</param>
        /// <returns>True if deletion was successful</returns>
        public async Task<bool> DeleteFileAsync(string fileId)
        {
            if (string.IsNullOrEmpty(fileId))
            {
                throw new ArgumentNullException(nameof(fileId));
            }

            var response = await _httpClient.GetAsync($"{_baseUrl}/deleteFile?fileId={Uri.EscapeDataString(fileId)}");
            response.EnsureSuccessStatusCode();

            var content = await response.Content.ReadAsStringAsync();
            var result = JsonSerializer.Deserialize<Dictionary<string, JsonElement>>(content);
            
            return result.TryGetValue("status", out var status) && 
                   status.GetString() == "success";
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!_disposed)
            {
                if (disposing)
                {
                    _httpClient?.Dispose();
                }
                _disposed = true;
            }
        }

        ~GoFileWrapper()
        {
            Dispose(false);
        }
    }
} 