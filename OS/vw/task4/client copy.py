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
    file_content = bytearray()
    
    while True:
        # Receive chunk from server
        msgFromServer = UDPClientSocket.recvfrom(bufferSize)
        data = msgFromServer[0]
        
        if len(data) < 2:  # Need type + length
            print("❌ Malformed response received")
            break
            
        msg_type = data[0]
        msg_length = data[1]
        
        # Check for EOF
        if msg_length == 0:
            break
            
        # Get chunk content
        chunk = data[2:2+msg_length]
        
        # Check for error message
        if b"ERROR" in chunk:
            print("❌ Server error:", chunk.decode(errors='ignore'))
            break
            
        # Add chunk to file content
        file_content.extend(chunk)
    
    # Print result
    if file_content:
        print("✅ File content received:\n")
        print(file_content.decode(errors='ignore'))

except socket.timeout:
    print("❌ Server not responding")

finally:
    UDPClientSocket.close()
