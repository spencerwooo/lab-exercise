import argparse
import socketserver
import logging

class SimpleHttpServer(socketserver.BaseRequestHandler):
    def handle(self):
        request_data = self.request.recv(4096).decode()
        splited_request = request_data.split('\r\n')[:-1]
        base_info = splited_request[0].split()
        http_method = base_info[0]
        http_request_path = base_info[1]
        http_version = base_info[2]
        headers = []
        for header in splited_request[1:]:
            # headers
            headers.append(header.split(': '))
        logging.info()

if __name__ == '__main__':
    logging.basicConfig(format='[%(levelname)s] %(asctime)s: %(message)s', level=logging.INFO)
