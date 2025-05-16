import socket
import sys
from struct import *

serverAddressPort   = ("10.10.10.11", 5002)
bufferSize          = 1024

if len(sys.argv) != 2:
    print("Invalid argv")
    sys.exit(0)

# Create a UDP socket at client side
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

UDPClientSocket.settimeout(3)

msg = str(sys.argv[1]).encode()
UDPClientSocket.sendto(pack("=100s", msg), serverAddressPort)
# msgFromServer = UDPClientSocket.recvfrom(bufferSize)
# print(msgFromServer[0])

# while len(msgFromServer[0]) == bufferSize:
#     msgFromServer = UDPClientSocket.recvfrom(bufferSize)
#     print(msgFromServer[0])