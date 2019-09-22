import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.TimeZone;

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
      SimpleDateFormat dateFormat = new SimpleDateFormat("E, d MMM yyyy HH:mm:ss z", Locale.US);
      dateFormat.setTimeZone(TimeZone.getTimeZone("GMT"));
      String time = dateFormat.format(new Date());

      // Get current worker thread name
      String currentThreadName = Thread.currentThread().getName();

      // Debug
      BufferedReader bf = new BufferedReader(new InputStreamReader(input));
      System.out.println("[WORKER] Request: " + bf.readLine());

      // Body builder
      String body = "";
      body += "<html><p><h3>Server Worker:</h3></p>";
      body += "<p>" + this.serverResponse + " - " + time + " - " + currentThreadName + "</p></html>";

      // Response builder
      String response = "";
      response += "HTTP/1.1 200 OK\r\n";
      response += "Server: Potato server v0.0.1\r\n";
      response += "Date: " + time + "\r\n";
      response += "Content-Type: text/html\r\n";
      response += "Content-Length: " + body.length() + "\r\n\r\n";

      response += body;

      // Write response
      output.write(response.getBytes());

      output.close();
      input.close();

      System.out.println("[WORKER] Request processed: " + time + " - " + currentThreadName);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
