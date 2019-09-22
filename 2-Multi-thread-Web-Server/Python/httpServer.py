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
        src = self.client_address[0]
        headers = []
        logging.info('recv http request to {} from {} with method {} version {}'.format(
            http_request_path, src, http_method, http_version))
        logging.info('headers:')
        for header in splited_request[1:]:
            # headers
            headers.append(header.split(': '))
            logging.info('  {}'.format(header))
        response_body = """Welcome to Potato Sever. \nyou are requesting {request_path},\nyour ip is {src}
        """.format(request_path=http_request_path, src=src).encode()

        response_headers = [
            'HTTP/1.1 200 OK',
            'Content-Length: {}'.format(len(response_body)),
            'Server: Potato Server/0.2',
            'Content-Type: text/html'
            ]
        response = '\r\n'.join(response_headers).encode() + b'\r\n\r\n' + response_body + b'\r\n'
        self.request.sendall(response)

if __name__ == '__main__':
    logging.basicConfig(format='[%(levelname)s] %(asctime)s: %(message)s', level=logging.INFO)
    paser = argparse.ArgumentParser()
    paser.add_argument('host', type=str)
    paser.add_argument('port', type=int)
    args = paser.parse_args()
    with socketserver.ThreadingTCPServer((args.host, args.port), SimpleHttpServer) as server:
        server.serve_forever()