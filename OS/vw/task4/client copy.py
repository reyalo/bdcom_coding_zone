import socket
import sys
from struct import pack

# Configuration
serverAddressPort = ("1.1.1.1", 5002)
bufferSize = 1024

# Argument check
if len(sys.argv) != 2:
    print("Usage: python script.py <message>")
    sys.exit(1)

# Prepare the message
msg = str(sys.argv[1]).encode()

# Optional: Pack message to 32 bytes (uncomment if required by protocol)
# if len(msg) > 32:
#     msg = msg[:32]
# msg = pack("=32s", msg)

# Create and use UDP socket
try:
    UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    
    # Send message
    UDPClientSocket.sendto(msg, serverAddressPort)
    
    # Receive initial response
    msgFromServer, addr = UDPClientSocket.recvfrom(bufferSize)
    print(f"Received from {addr}: {msgFromServer}")

    # Continue receiving if more data expected
    while len(msgFromServer) == bufferSize:
        msgFromServer, addr = UDPClientSocket.recvfrom(bufferSize)
        print(f"Received from {addr}: {msgFromServer}")

except Exception as e:
    print("Error occurred:", e)

finally:
    UDPClientSocket.close()
