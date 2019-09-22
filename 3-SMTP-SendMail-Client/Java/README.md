# Project 3 - SMTP SendMail Client

## Target

SMTP Client encapsulates all the functionality necessary to send mail through an SMTP server. You must first connect to the server with TCP port 25. Before doing anything, and finally disconnect after you're completely finished interacting with the server. Once you setup a TCP connection, you can interactive with SMTP server using SMTP commands. The convention for all the SMTP command methods in SMTP Client is such that they must return a boolean value or some other value.

## Usage

This project uses [Mailtrap.io - Safe Email Testing for Staging & Development](https://mailtrap.io) as dummy SMTP server. Please configure [config.properties](src/main/resources/config.properties) accordingly.

### Pre-building

You need to install Gradle in order to build the project.

```shell
# macOS using Homebrew
brew install gradle

# or ... Windows using Scoop
scoop install gradle
```

Verify Gradle installation by:

```shell
gradle -v
```

### Building

Use Gradle to build the project.

```shell
gradle build
```

### Running

This program is an interactive console application. For best experience, please silence all Gradle task prompts.

```shell
gradle run -q --console=plain
```

## Expected result

### Console app

[![asciicast](https://asciinema.org/a/G0Q4uIJckJZvYCEPMPFbzQm6l.svg)](https://asciinema.org/a/G0Q4uIJckJZvYCEPMPFbzQm6l?t=3)

### SMTP Server - [Mailtrap.io](https://mailtrap.io)

![](https://i.loli.net/2019/09/22/LovAGQeJXrBN2On.png)
