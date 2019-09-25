package ftp.client;

import org.apache.commons.net.PrintCommandListener;
import org.apache.commons.net.ftp.FTPClient;
import org.apache.commons.net.ftp.FTPFile;
import org.apache.commons.net.ftp.FTPReply;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.*;
import java.util.Arrays;
import java.util.Collection;
import java.util.Scanner;
import java.util.stream.Collectors;

public class FtpClient {

    private static String server;
    private static int port;
    private static String user;
    private static String password;
    private static String uploadfile;
    private static String uploadfilepath;
    private static String downloadfile;
    private static String downloadfilepath;
    private static String listpath;
    private FTPClient ftp;

    FtpClient() {
    }

    public static void main(String[] args) throws IOException, ParserConfigurationException {
        FtpClient myclient = new FtpClient();
        File configfile = new File(FtpClient.class.getResource("/config.xml").getFile());
        try {
            DocumentBuilderFactory dbf = DocumentBuilderFactory.newDefaultInstance();
            DocumentBuilder db = dbf.newDocumentBuilder();
            Document doc = db.parse(configfile);
            NodeList ftpList = doc.getElementsByTagName("ftp");
            for (int i = 0; i < ftpList.getLength(); i++) {
                Element n = (Element) ftpList.item(i);
                NamedNodeMap node = n.getAttributes();
                server = n.getElementsByTagName("host").item(0).getFirstChild().getNodeValue();
                port = Integer.parseInt(n.getElementsByTagName("port").item(0).getFirstChild().getNodeValue());
                user = n.getElementsByTagName("username").item(0).getFirstChild().getNodeValue();
                password = n.getElementsByTagName("password").item(0).getFirstChild().getNodeValue();
            }
            NodeList fileList = doc.getElementsByTagName("file");
            for (int i = 0; i < fileList.getLength(); i++) {
                Element n = (Element) fileList.item(i);
                NamedNodeMap node = n.getAttributes();
                uploadfile = n.getElementsByTagName("uploadfile").item(0).getFirstChild().getNodeValue();
                uploadfilepath = n.getElementsByTagName("uploadfilepath").item(0).getFirstChild().getNodeValue();
                downloadfile = n.getElementsByTagName("downloadfile").item(0).getFirstChild().getNodeValue();
                downloadfilepath = n.getElementsByTagName("downloadfilepath").item(0).getFirstChild().getNodeValue();
                listpath = n.getElementsByTagName("listpath").item(0).getFirstChild().getNodeValue();
            }
        } catch (SAXException e) {
            e.printStackTrace();
        }

        System.out.println(myclient.getGreeting() + "\n");
        myclient.open();
        System.out.println();

        String command = "";
        Scanner scanner = new Scanner(System.in);

        while (!command.equals("q")) {
            command = scanner.nextLine();
            switch (command) {
                case "upload":
                    File myfile = new File(uploadfile);
                    myclient.putFileToPath(myfile, uploadfilepath);
                    System.out.println();
                    break;
                case "download":
                    myclient.downloadFile(downloadfile, downloadfilepath);
                    System.out.println();
                    break;
                case "list":
                    myclient.listFiles(listpath);
                    System.out.println();
                    break;
                case "info":
                    System.out.println("list - Lists files under path " + listpath);
                    System.out.println("upload - Upload file: " + uploadfile + "; Upload file path: " + uploadfilepath);
                    System.out.println("download - Download file: " + downloadfile + "; Download file path: " + downloadfilepath);
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

        myclient.close();
    }

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

    public void close() throws IOException {
        ftp.disconnect();
    }

    public Collection<String> listFiles(String path) throws IOException {
        FTPFile[] files = ftp.listFiles(path);
        System.out.println(Arrays.stream(files)
                .map(FTPFile::getName)
                .collect(Collectors.toList()));
        return Arrays.stream(files)
                .map(FTPFile::getName)
                .collect(Collectors.toList());
    }

    public void putFileToPath(File file, String path) throws IOException {
        ftp.storeFile(path, new FileInputStream(file));
    }

    public void downloadFile(String source, String destination) throws IOException {
        FileOutputStream out = new FileOutputStream(destination);
        ftp.retrieveFile(source, out);
        out.close();
    }

    public String getGreeting() {
        return "Hello My FTP Client.";
    }
}
