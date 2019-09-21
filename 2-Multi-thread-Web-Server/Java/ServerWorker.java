import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.time.LocalDateTime;

/**
 * ServerWorker
 */
public class ServerWorker implements Runnable {

  private Socket clientSocket = null;
  private String serverResponse = null;

  public ServerWorker(Socket clientSocket, String serverResponse) {
    this.clientSocket = clientSocket;
    this.serverResponse = serverResponse;
  }

  @Override
  public void run() {
    try {
      InputStream input = clientSocket.getInputStream();
      OutputStream output = clientSocket.getOutputStream();

      LocalDateTime time = LocalDateTime.now();

      output.write(("HTTP/1.1 200 OK\n\nServer Worker: " + this.serverResponse + " - " + time + "\n").getBytes());

      output.close();
      input.close();

      System.out.println("[WORKER] Request processed: " + time);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

}
