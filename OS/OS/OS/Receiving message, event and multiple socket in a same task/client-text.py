import socket
from struct import *

serverAddressPort   = ("1.1.1.1", 5001)
bufferSize          = 64

REMOTE_COMMUNICATION_MSE_TYPE_QRY = 1    # query
REMOTE_COMMUNICATION_MSE_TYPE_CFM = 3    # confirm

# Create a UDP socket at client side
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

UDPClientSocket.settimeout(3)

# Send to server using created UDP socket
msg = b"Hello"
UDPClientSocket.sendto(pack("=bh60s", REMOTE_COMMUNICATION_MSE_TYPE_QRY, socket.ntohs(len(msg)), msg), serverAddressPort)
msgFromServer = UDPClientSocket.recvfrom(bufferSize)
print(msgFromServer[0])

msg = str("How do yo do" + " " + str(msgFromServer[1][0]) + " " + str(msgFromServer[1][1])).encode()
UDPClientSocket.sendto(pack("=bh60s", REMOTE_COMMUNICATION_MSE_TYPE_CFM, socket.ntohs(len(msg)), msg), serverAddressPort)
msgFromServer = UDPClientSocket.recvfrom(bufferSize)
print(msgFromServer[0])