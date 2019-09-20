/**
 * CLI:
 * TCP Client and Server Command Line Interface
 * Client and Server runs in different threads
 *
 * Requests: TEMPERATURE
 * Response: TEMPERATURE = 60
 */
public class CLI {

  private static int serverPort = 8080;

  public static void main(String[] args) {
    Runnable clientRunnable = new Runnable() {

      @Override
      public void run() {
        Client client = new Client("127.0.0.1", serverPort);
        client.startClient();
      }
    };

    Runnable serverRunnable = new Runnable() {

      @Override
      public void run() {
        Server server = new Server(serverPort);
        server.startServer();
      }
    };

    // Start multiple threads for client and server
    Thread serverThread = new Thread(serverRunnable);
    Thread clientThread = new Thread(clientRunnable);
    serverThread.start();
    clientThread.start();
  }
}
