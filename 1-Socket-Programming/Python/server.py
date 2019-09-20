import socketserver
import argparse

class SimpleTcpRepeter(socketserver.BaseRequestHandler):
    temperature = 60
    humidity = 20
    light = 2
    def handle(self):
        while True:
            try:
                self.request.recv(1024)
            except ConnectionAbortedError:
                return
            print('recv query from client {}'.format(self.client_address[0]))
            self.request.sendall("TEMPERATURE={} HUMIDITY={} LIGHT={}\n".format(self.temperature, self.humidity, self.light).encode())

if __name__ == '__main__':
    paser = argparse.ArgumentParser()
    paser.add_argument('host', type=str, help='host to listen')
    paser.add_argument('port', type=int, help='port to listen')
    args = paser.parse_args()
    with socketserver.TCPServer((args.host, args.port), SimpleTcpRepeter) as server:
        server.serve_forever()