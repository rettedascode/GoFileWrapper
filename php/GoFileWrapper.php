<?php

class GoFileWrapper {
    private $baseUrl;
    private $curl;

    public function __construct() {
        $this->baseUrl = 'https://api.gofile.io';
        $this->curl = curl_init();
    }

    public function __destruct() {
        if ($this->curl) {
            curl_close($this->curl);
        }
    }

    /**
     * Upload a file to GoFile
     * @param string $filePath Path to the file to upload
     * @return array Response from the server
     */
    public function uploadFile($filePath) {
        if (!file_exists($filePath)) {
            throw new Exception("File not found: $filePath");
        }

        $server = $this->getServerStatus();
        if (empty($server)) {
            throw new Exception("Could not get server status");
        }

        $file = new CURLFile($filePath);
        
        curl_setopt_array($this->curl, [
            CURLOPT_URL => $this->baseUrl . '/uploadFile',
            CURLOPT_POST => true,
            CURLOPT_POSTFIELDS => ['file' => $file],
            CURLOPT_RETURNTRANSFER => true
        ]);

        $response = curl_exec($this->curl);
        if ($response === false) {
            throw new Exception('Curl error: ' . curl_error($this->curl));
        }

        return json_decode($response, true);
    }

    /**
     * Get server status
     * @return array Server status information
     */
    public function getServerStatus() {
        curl_setopt_array($this->curl, [
            CURLOPT_URL => $this->baseUrl . '/getServer',
            CURLOPT_RETURNTRANSFER => true
        ]);

        $response = curl_exec($this->curl);
        if ($response === false) {
            throw new Exception('Curl error: ' . curl_error($this->curl));
        }

        return json_decode($response, true);
    }

    /**
     * Get file information
     * @param string $fileId The ID of the file
     * @return array File information
     */
    public function getFileInfo($fileId) {
        curl_setopt_array($this->curl, [
            CURLOPT_URL => $this->baseUrl . '/getFile?fileId=' . urlencode($fileId),
            CURLOPT_RETURNTRANSFER => true
        ]);

        $response = curl_exec($this->curl);
        if ($response === false) {
            throw new Exception('Curl error: ' . curl_error($this->curl));
        }

        return json_decode($response, true);
    }

    /**
     * Delete a file
     * @param string $fileId The ID of the file to delete
     * @return bool True if deletion was successful
     */
    public function deleteFile($fileId) {
        curl_setopt_array($this->curl, [
            CURLOPT_URL => $this->baseUrl . '/deleteFile?fileId=' . urlencode($fileId),
            CURLOPT_RETURNTRANSFER => true
        ]);

        $response = curl_exec($this->curl);
        if ($response === false) {
            throw new Exception('Curl error: ' . curl_error($this->curl));
        }

        $result = json_decode($response, true);
        return isset($result['status']) && $result['status'] === 'success';
    }
} 