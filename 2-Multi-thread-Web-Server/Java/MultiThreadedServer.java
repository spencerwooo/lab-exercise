import java.net.ServerSocket;
import java.net.Socket;

/**
 * MultiThreadedServer
 */
public class MultiThreadedServer implements Runnable {

  protected int serverPort = 8080;
  protected ServerSocket serverSocket = null;
  protected boolean isStopped = false;
  protected Thread serverThread = null;

  public MultiThreadedServer(int port) {
    this.serverPort = port;
  }

  @Override
  public void run() {
    synchronized (this) {
      this.serverThread = Thread.currentThread();
    }

    openServerSocket();
    System.out.println("[SERVER] Starting server: " + this.serverSocket.getLocalSocketAddress());

    while (!isStopped()) {
      Socket clientSocket = null;
      try {
        clientSocket = this.serverSocket.accept();
      } catch (Exception e) {
        if (isStopped()) {
          System.out.println("Server stopped.");
          return;
        }

        throw new RuntimeException("Error accepting client connection. ", e);
      }

      new Thread(new ServerWorker(clientSocket, "MultiThreaded Server")).start();
    }
  }

  private void openServerSocket() {
    try {
      this.serverSocket = new ServerSocket(this.serverPort);
    } catch (Exception e) {
      throw new RuntimeException("Cannot open port " + this.serverPort, e);
    }
  }

  private synchronized boolean isStopped() {
    return this.isStopped;
  }

  public synchronized void stopServer() {
    this.isStopped = true;
    try {
      this.serverSocket.close();
    } catch (Exception e) {
      throw new RuntimeException("Error closing server. ", e);
    }
  }

  public static void main(String[] args) {
    MultiThreadedServer multiThreadedServer = new MultiThreadedServer(9000);
    new Thread(multiThreadedServer).start();
  }
}
