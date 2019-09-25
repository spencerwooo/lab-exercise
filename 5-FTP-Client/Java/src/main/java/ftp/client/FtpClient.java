package ftp.client;

import org.apache.commons.net.PrintCommandListener;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPFile;
import org.apache.commons.net.ftp.FTPReply;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.*;
import java.util.Arrays;
import java.util.Scanner;
import java.util.stream.Collectors;

public class FtpClient {

    // FTP server configurations
    private static String server;
    private static int port;

    // FTP username & password
    private static String user;
    private static String password;

    // Upload/download file path
    private static String uploadFile;
    private static String uploadFilePath;
    private static String downloadFile;
    private static String downloadFilePath;

    // List all files under path
    private static String listPath;
    private FTPClient ftp;

    FtpClient() {
    }

    public static void main(String[] args) throws IOException, ParserConfigurationException {
        FtpClient client = new FtpClient();
        File configFile = new File(FtpClient.class.getResource("/config.xml").getFile());

        // Parse configuration file
        try {
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newDefaultInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();

            Document doc = db.parse(configFile);

            NodeList ftpList = doc.getElementsByTagName("ftp");
            for (int i = 0; i < ftpList.getLength(); i++) {
                Element n = (Element) ftpList.item(i);
                server = n.getElementsByTagName("host").item(0).getFirstChild().getNodeValue();
                port = Integer.parseInt(n.getElementsByTagName("port").item(0).getFirstChild().getNodeValue());
                user = n.getElementsByTagName("username").item(0).getFirstChild().getNodeValue();
                password = n.getElementsByTagName("password").item(0).getFirstChild().getNodeValue();
            }

            NodeList fileList = doc.getElementsByTagName("file");
            for (int i = 0; i < fileList.getLength(); i++) {
                Element n = (Element) fileList.item(i);
                uploadFile = n.getElementsByTagName("uploadfile").item(0).getFirstChild().getNodeValue();
                uploadFilePath = n.getElementsByTagName("uploadfilepath").item(0).getFirstChild().getNodeValue();
                downloadFile = n.getElementsByTagName("downloadfile").item(0).getFirstChild().getNodeValue();
                downloadFilePath = n.getElementsByTagName("downloadfilepath").item(0).getFirstChild().getNodeValue();
                listPath = n.getElementsByTagName("listpath").item(0).getFirstChild().getNodeValue();
            }
        } catch (SAXException e) {
            e.printStackTrace();
        }

        System.out.println(client.getGreeting() + "\n");
        client.open();
        System.out.println();

        String command = "";
        Scanner scanner = new Scanner(System.in);

        // Deal with user input
        while (!command.equals("q")) {
            command = scanner.nextLine();
            switch (command) {
                case "upload":
                    File fileToUpload = new File(uploadFile);
                    client.putFileToPath(fileToUpload, uploadFilePath);
                    System.out.println();
                    break;
                case "download":
                    client.downloadFile(downloadFile, downloadFilePath);
                    System.out.println();
                    break;
                case "list":
                    client.listFiles(listPath);
                    System.out.println();
                    break;
                case "info":
                    System.out.println("list - Lists files under path " + listPath);
                    System.out.println("upload - Upload file: " + uploadFile + "; Upload file path: " + uploadFilePath);
                    System.out.println("download - Download file: " + downloadFile + "; Download file path: " + downloadFilePath);
                    System.out.println();
                    break;
                case "help":
                    System.out.println("list - Lists all files under desired path.");
                    System.out.println("upload - Upload desired file to remote server.");
                    System.out.println("download - Download desired file to local machine.");
                    System.out.println("info - Look information about FTP client.");
                    System.out.println("q - Exit FTP Client.\n");
                    break;
                case "q":
                    System.out.println("Bye!\n");
                    break;
                default:
                    System.out.println("Illegal command. Type \"help\" to see all commands available.\n");
            }
        }

        client.close();
    }

    /**
     * FTP connection open port
     *
     * @throws IOException
     */
    public void open() throws IOException {
        ftp = new FTPClient();

        ftp.addProtocolCommandListener(new PrintCommandListener(new PrintWriter(System.out)));

        ftp.connect(server, port);
        int reply = ftp.getReplyCode();
        if (!FTPReply.isPositiveCompletion(reply)) {
            ftp.disconnect();
            throw new IOException("Exception in connecting to FTP Server");
        }

        ftp.login(user, password);
    }

    /**
     * FTP connection close
     * @throws IOException
     */
    public void close() throws IOException {
        ftp.disconnect();
    }

    /**
     * List all files under desired path
     *
     * @param path
     * @throws IOException
     */
    public void listFiles(String path) throws IOException {
        FTPFile[] files = ftp.listFiles(path);
        System.out.println(Arrays.stream(files)
                .map(FTPFile::getName)
                .collect(Collectors.toList()));
    }

    /**
     * Store file onto FIP server
     * @param file
     * @param path
     * @throws IOException
     */
    public void putFileToPath(File file, String path) throws IOException {
        ftp.storeFile(path, new FileInputStream(file));
    }

    /**
     * Download file from FTP server
     * @param source
     * @param destination
     * @throws IOException
     */
    public void downloadFile(String source, String destination) throws IOException {
        FileOutputStream out = new FileOutputStream(destination);
        ftp.retrieveFile(source, out);
        out.close();
    }

    public String getGreeting() {
        return "Hello My FTP Client.";
    }
}
