# Project 6 - Network Download

## Target

Network Download acts as http client. It encapsulates all the functionality necessary to retrieve URL from a Web server. You must first connect to the server with TCP port 80. Before doing anything, and finally disconnect after you're completely finished interacting with the server. Once you setup a TCP connection, you can interactive with Web server using http request/response commands. The http response includes URL resource. You need write file to store the resource.

## Usage

### Pre-configurations

Change `config.properties` according to your own settings.

```properties
# File to be downloaded URL
network.download.url=https://i.loli.net/2018/11/18/5bf11bc42a1d3.jpg
# Download destination (Relative to user home directory)
network.download.destination=Downloads
```

Downloaded file is stored at `/User/<YOUR_USER_NAME>/Downloads` (Unix) or `C:\User\<YOUR_USER_NAME>\Downloads` (Windows) by default.

### Building

```shell script
gradle build
```

### Running task

```shell script
gradle run -q --console=plain
```

### Using downloader

To see a full list of commands available, type `help`:

![](https://i.loli.net/2019/10/05/bs8LKMRTwk3D1VI.png)

To download file specified in `config.properties`, type `download`:

![](https://i.loli.net/2019/10/05/cqIkP8iUtzBT1d7.png)
