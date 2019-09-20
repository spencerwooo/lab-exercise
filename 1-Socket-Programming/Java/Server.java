import java.io.DataInputStream;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Server
 */
public class Server {

  private Socket socket = null;
  private ServerSocket server = null;
  private DataInputStream inputStream = null;

  private int serverPort;

  public Server(int port) {
    serverPort = port;
  }

  public void startServer() {
    System.out.println("[SERVER] Starting server, listening at port: " + serverPort);
  }
}
