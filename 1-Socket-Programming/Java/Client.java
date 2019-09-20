import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.Socket;

/**
 * Client: TCP Client
 */
public class Client {

  private Socket client = null;
  private BufferedReader userInput = null;
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

      // Read user input (System.in) stream
      userInput = new BufferedReader(new InputStreamReader(System.in));
      // Read client input stream
      inputStream = new DataInputStream(client.getInputStream());
      // Read client output stream
      outputStream = new DataOutputStream(client.getOutputStream());
    } catch (Exception e) {
      e.printStackTrace();
    }

    String message = "";
    while (!message.equals("BYE")) {
      try {
        // Read user input
        message = userInput.readLine();

        // Write output stream, send request to server
        outputStream.writeUTF(message);

        // Read from server's response
        System.out.println("[CLIENT] Server responded: " + inputStream.readUTF());
      } catch (Exception e) {
        e.printStackTrace();
      }
    }

    // Close connection
    try {
      inputStream.close();
      outputStream.close();
      client.close();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
