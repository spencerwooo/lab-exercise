import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.time.LocalDateTime;

/**
 * ServerWorker
 */
public class ServerWorker implements Runnable {

  protected Socket clientSocket = null;
  protected String serverResponse = null;

  public ServerWorker(Socket clientSocket, String serverResponse) {
    this.clientSocket = clientSocket;
    this.serverResponse = serverResponse;
  }

  @Override
  public void run() {
    try {
      InputStream input = clientSocket.getInputStream();
      OutputStream output = clientSocket.getOutputStream();

      // Get current time & date
      LocalDateTime time = LocalDateTime.now();
      // Get current worker thread name
      String currentThreadName = Thread.currentThread().getName();

      // Response builder
      String response = "HTTP/1.1 200 OK\n";
      response += "\n";
      response += "<html><p><strong>Server Worker:</strong></p>";
      response += "<p>" + this.serverResponse + " - " + time + " - " + currentThreadName + "</p></html>";

      // Write response
      output.write(response.getBytes());
      // System.out.println(response);

      output.close();
      input.close();

      System.out.println("[WORKER] Request processed: " + time);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
