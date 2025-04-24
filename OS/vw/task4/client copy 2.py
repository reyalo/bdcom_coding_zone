import socket
from struct import *

# Server config
serverAddressPort = ("10.10.10.11", 5050)
bufferSize = 512

# Message types
REMOTE_COMMUNICATION_MSG_TYPE_QRY = 1  # query
REMOTE_COMMUNICATION_MSG_TYPE_CFM = 3  # confirm (optional)

# Create a UDP socket
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
UDPClientSocket.settimeout(3)

# 🔽 Get filename from user
filename = input("Enter filename to request from server: ").strip().encode()

# Safety: max 60 bytes
if len(filename) > 60:
    print("❗ Filename too long (max 60 bytes)")
    UDPClientSocket.close()
    exit()

# Pad filename to 60 bytes
packed_msg = pack("!bb60s", REMOTE_COMMUNICATION_MSG_TYPE_QRY, len(filename), filename)

# Send to server
UDPClientSocket.sendto(packed_msg, serverAddressPort)

try:
    # Receive server response
    msgFromServer = UDPClientSocket.recvfrom(bufferSize)
    response = msgFromServer[0].decode(errors="ignore")

    if "ERROR" in response.upper():
        print("❌ Server error:", response)
    else:
        print("✅ File content received:\n")
        print(response)

except socket.timeout:
    print("❌ Server not responding")

finally:
    UDPClientSocket.close()
