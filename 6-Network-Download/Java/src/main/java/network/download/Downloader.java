package network.download;

import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URL;
import java.net.URLConnection;
import java.nio.channels.Channels;
import java.nio.channels.FileChannel;
import java.nio.channels.ReadableByteChannel;
import java.nio.file.Path;
import java.nio.file.Paths;

class Downloader {
    /**
     * Download file from remote server
     *
     * @param downloadUrl:         URL to be downloaded
     * @param downloadDestination: Download file target folder
     * @param downloadedFileName:  Downloaded file name
     * @throws IOException : File not found exception
     */
    void downloadFile(String downloadUrl, String downloadDestination, String downloadedFileName)
            throws IOException {
        // Print help information
        System.out.println("[DOWNLOADER] Starting download...");
        Path downloadFilePath = Paths.get(System.getProperty("user.home"),
                downloadDestination, downloadedFileName);
        System.out.println("[DOWNLOADER] Downloading file to " + downloadFilePath);

        // Start download
        URL url = new URL(downloadUrl);
        URLConnection conn = url.openConnection();
        int fileSize = conn.getContentLength();

        if (fileSize < 0) {
            System.out.print("[DOWNLOADER] File is empty. Aborting");
        } else {
            // Loading indicator
            String botName = "DOWNLOADER";
            String greetings = "Downloading";
            LoadingIndicator loadingIndicator = new LoadingIndicator(botName, greetings);
            loadingIndicator.start();

            ReadableByteChannel readableByteChannel = Channels.newChannel(url.openStream());
            FileOutputStream fileOutputStream = new FileOutputStream(String.valueOf(downloadFilePath));
            FileChannel fileChannel = fileOutputStream.getChannel();
            fileChannel.transferFrom(readableByteChannel, 0, Long.MAX_VALUE);
            fileOutputStream.close();
            readableByteChannel.close();

            // Stop loading
            loadingIndicator.loading = false;
            System.out.println("\n[DOWNLOADER] Finished downloading.");
        }

    }
}
