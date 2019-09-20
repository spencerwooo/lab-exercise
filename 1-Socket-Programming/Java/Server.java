import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Server: TCP Server
 */
public class Server {

  private Socket serverSocket = null;
  private ServerSocket server = null;
  private DataInputStream inputStream = null;
  private DataOutputStream outputStream = null;

  private int serverPort;

  public Server(int port) {
    serverPort = port;
  }

  public void startServer() {
    System.out.println("[SERVER] Starting server, listening at port: " + serverPort);

    try {
      // Start server
      server = new ServerSocket(serverPort);
      System.out.println("[SERVER] Server started. Waiting for connection...");

      // Accept connection from client
      serverSocket = server.accept();
      System.out.println("[SERVER] Accepted connection from client.");

      // Client request
      inputStream = new DataInputStream(serverSocket.getInputStream());
      // Server response
      outputStream = new DataOutputStream(serverSocket.getOutputStream());

      String request = "";
      String response = "";

      while (!request.equals("BYE")) {
        try {
          request = inputStream.readUTF();

          // Handle requests
          switch (request) {
          case "TEMPERATURE":
            response = "TEMPERATURE = 60";
            break;
          case "HUMIDITY":
            response = "HUMIDITY = 0";
            break;
          case "LIGHT":
            response = "LIGHT = 1";
            break;
          case "BYE":
            response = "Bye!";
            break;
          default:
            response = "INVALID REQUEST";
            break;
          }

          System.out.println("[SERVER] Request: " + request + ". Response: " + response);

          // Send response back to client
          outputStream.writeUTF(response);
        } catch (Exception e) {
          e.printStackTrace();
        }
      }

      // Close connection
      serverSocket.close();
      inputStream.close();
      System.out.println("[SERVER] Connection closed.");
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
