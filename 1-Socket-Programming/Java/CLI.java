/**
 * CLI
 */
public class CLI {

  public static void main(String[] args) {
    Server server = new Server();
    Client client = new Client();

    server.startServer();
    client.startClient();
  }
}
