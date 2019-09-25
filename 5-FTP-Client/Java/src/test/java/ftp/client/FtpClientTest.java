package ftp.client;

import org.junit.Test;

import static org.junit.Assert.assertNotNull;

public class FtpClientTest {
    @Test
    public void testAppHasAGreeting() {
        FtpClient classUnderTest = new FtpClient();
        assertNotNull("FtpClient should have a greeting", classUnderTest.getGreeting());
    }
}
