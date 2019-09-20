import socket
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('host', type=str)
parser.add_argument('port', type=int)
args = parser.parse_args()

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.connect((args.host, args.port))
    while True:
        try:
            input()
        except EOFError:
            break
        sock.sendall(b'query')
        recv = sock.recv(2048)
        print(recv.decode())
