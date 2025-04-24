import socket
from struct import *

serverAddressPort = ("10.10.10.11", 5050)
bufferSize = 64

REMOTE_COMMUNICATION_MSG_TYPE_QRY = 1  # query
REMOTE_COMMUNICATION_MSG_TYPE_CFM = 3  # confirm

# Create a UDP socket at client side
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

UDPClientSocket.settimeout(3)

# Send to server using created UDP socket
msg = "hello".encode()  # Encode the string to bytes
UDPClientSocket.sendto(pack("!bb60s", REMOTE_COMMUNICATION_MSG_TYPE_QRY, len(msg), msg), serverAddressPort)

try:
    msgFromServer = UDPClientSocket.recvfrom(bufferSize)
    print(msgFromServer[0].decode())



    msg = f"How do you do {msgFromServer[1][0]} {msgFromServer[1][1]}".encode()
    UDPClientSocket.sendto(pack("!bb60s", REMOTE_COMMUNICATION_MSG_TYPE_CFM, len(msg), msg), serverAddressPort)
    
    msgFromServer = UDPClientSocket.recvfrom(bufferSize)
    print(msgFromServer[0].decode())


except socket.timeout:
    print("Server not responding")
finally:
    UDPClientSocket.close()
