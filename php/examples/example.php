<?php

require_once __DIR__ . '/../GoFileWrapper.php';

try {
    $gofile = new GoFileWrapper();

    // Get server status
    echo "Getting server status...\n";
    $serverStatus = $gofile->getServerStatus();
    echo "Server status: " . json_encode($serverStatus) . "\n";

    // Upload a file
    echo "\nUploading file...\n";
    $uploadResult = $gofile->uploadFile("path/to/your/file.txt");
    $fileId = $uploadResult['data']['fileId'];
    echo "File uploaded successfully. File ID: " . $fileId . "\n";

    // Get file info
    echo "\nGetting file info...\n";
    $fileInfo = $gofile->getFileInfo($fileId);
    echo "File name: " . $fileInfo['data']['name'] . "\n";
    echo "File size: " . $fileInfo['data']['size'] . " bytes\n";

    // Delete the file
    echo "\nDeleting file...\n";
    $deleteResult = $gofile->deleteFile($fileId);
    echo "File deletion " . ($deleteResult ? "successful" : "failed") . "\n";
} catch (Exception $e) {
    echo "Error: " . $e->getMessage() . "\n";
    exit(1);
} 