import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.Socket;

/**
 * Client: TCP Client
 */
public class Client {

  private Socket client = null;
  private DataInputStream userInputStream = null;
  private DataInputStream inputStream = null;
  private DataOutputStream outputStream = null;

  private int serverPort;
  private String serverAddress;

  public Client(String address, int port) {
    serverAddress = address;
    serverPort = port;
  }

  public void startClient() {
    System.out.println("[CLIENT] Starting client...");

    // Trying to establish connection
    try {
      // Establish connection
      client = new Socket(serverAddress, serverPort);
      System.out.println("[CLIENT] Connected to server at: " + client.getRemoteSocketAddress());

      userInputStream = new DataInputStream(System.in);
      inputStream = new DataInputStream(client.getInputStream());
      outputStream = new DataOutputStream(client.getOutputStream());
    } catch (Exception e) {
      e.printStackTrace();
    }

    String message = "";
    while (!message.equals("BYE")) {
      try {
        message = userInputStream.readLine();
        outputStream.writeUTF(message);

        System.out.println("[CLIENT] Server responded: " + inputStream.readUTF());
      } catch (Exception e) {
        e.printStackTrace();
      }
    }

    // Close connection
    try {
      userInputStream.close();
      inputStream.close();
      outputStream.close();
      client.close();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
