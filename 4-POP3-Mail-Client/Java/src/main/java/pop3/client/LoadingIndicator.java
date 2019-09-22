package pop3.client;

/**
 * LoadingIndicator
 */
public class LoadingIndicator extends Thread {
  private boolean loading = true;

  public void setLoading(boolean loading) {
    this.loading = loading;
  }

  public void run() {
    String animation = "|/-\\";
    int i = 0;
    while (loading) {
      System.out.print("\r[POP3 Client] Fetching " + animation.charAt(i++ % animation.length()));
      try {
        Thread.sleep(100);
      } catch (Exception e) {
        // Do nothing
      }
    }
  }
}
