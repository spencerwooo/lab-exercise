/**
 * Client
 */
public class Client {

  private int clientPort;

  public Client(int port) {
    clientPort = port;
  }

  public void startClient() {
    System.out.println("[CLIENT] Starting client, listening at port: " + clientPort);
  }
}
