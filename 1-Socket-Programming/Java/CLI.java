/**
 * CLI
 */
public class CLI {

  private static int serverPort = 8080;
  private static int clientPort = 8081;

  public static void main(String[] args) {
    Server server = new Server(serverPort);
    Client client = new Client(clientPort);

    server.startServer();
    client.startClient();
  }
}
