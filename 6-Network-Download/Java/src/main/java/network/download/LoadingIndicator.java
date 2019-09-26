package network.download;

/**
 * LoadingIndicator
 */
public class LoadingIndicator extends Thread {
    boolean loading = true;
    private String botName;
    private String greetings;

    LoadingIndicator(String botName, String greetings) {
        this.botName = botName;
        this.greetings = greetings;
    }

    public void run() {
        String animation = "|/-\\";
        int i = 0;
        while (loading) {
            System.out.print("\r[" + this.botName + "] " + this.greetings + " " + animation.charAt(i++ % animation.length()));
            try {
                Thread.sleep(100);
            } catch (Exception e) {
                // Do nothing
            }
        }
    }
}