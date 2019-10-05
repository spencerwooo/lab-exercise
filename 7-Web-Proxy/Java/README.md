# Project 7 - Web Proxy

## Target

Web Proxy acts as Application layer gateway. Web browsers setup Proxy’s IP address and TCP port number. When browsers access the target web site, browsers first send the URL to our Web Proxy. Web Proxy receives the URL and forward the request to target web site. Once Web proxy receive the http response from web site, it will forward the response to original web browser.

## Usage

This is a very primitive proxy server, which is only capable of resolving specific requests, in which case: `http://example.com`. Host, local port and remote port are defined in `App.java`, starting from line 107:

```java
// Remote host: http://example.com 93.184.216.34
String host = "93.184.216.34";
// Listening on local port: 9000
int localPort = 9000;
// Default HTTP port: 80
int remotePort = 80;
```

Ideally, DNS resolution is done at the proxy server side, where the browser first sends HTTP requests directly to the proxy server, the proxy server does DNS lookup on behalf of the browser, then returns the remote server's response to the browser. However, in our case, we didn't implement such DNS resolution service at the proxy's side. Instead, we hard-coded the remote server's IP into the code, which is not a very elegant way to implement this.

Elegance aside, to run this code, simply:

```shell script
gradle run -q --console=plain
```

Then, set your browser's proxy configurations to:

- HTTP Proxy
- Proxy setting: `127.0.0.1:9000`

![](https://i.loli.net/2019/10/04/emZwCp5LyRiO1rj.png)

And try to connect to `http://example.com`.

You should get an output that resembles to the screenshot below:

![](https://i.loli.net/2019/10/04/8ZuCNJlbn71avw2.png)

And in your browser, you should be able to access `http://example.com`:

![](https://i.loli.net/2019/10/04/ehj3SaPqLBKAiYT.png)
