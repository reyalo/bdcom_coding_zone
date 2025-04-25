import socket
from struct import *
import os  # New import for file verification

# Server config
serverAddressPort = ("10.10.10.11", 5050)
bufferSize = 64

# Message types
REMOTE_COMMUNICATION_MSG_TYPE_QRY = 1  # query
REMOTE_COMMUNICATION_MSG_TYPE_CFM = 3  # confirm (optional)

# Create a UDP socket
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
UDPClientSocket.settimeout(3)

#Get filename from user
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
    total_chunks = 0
    
    while True:
        # Receive chunk from server
        msgFromServer = UDPClientSocket.recvfrom(bufferSize)
        data = msgFromServer[0]
        
        if len(data) < 2:  # Need type + length
            print("Malformed response received")
            break
            
        msg_type = data[0]
        msg_length = data[1]
        
        print(f"Received chunk {total_chunks + 1}: type={msg_type}, length={msg_length}")
        
        # Check for EOF
        if msg_length == 0:
            print(f"Total chunks received: {total_chunks}")
            break
            
        # Get chunk content
        chunk = data[2:2+msg_length]
        chunk_size = len(chunk)
        
        if chunk_size != msg_length:
            print(f"Warning: Expected {msg_length} bytes but got {chunk_size}")
            
        # Check for error message
        if b"ERROR" in chunk:
            print("Server error:", chunk.decode(errors='ignore'))
            break
            
        # Add chunk to file content
        file_content.extend(chunk)
        total_chunks += 1
        
        print(f"Total bytes received so far: {len(file_content)}")
    
    # Save result to file
    if file_content:
        try:
            # Use absolute path
            output_path = r"c:\Users\Admin\Documents\bdcom_coding_zone\OS\vw\task4\output.txt"
            with open(output_path, 'wb') as f:
                f.write(file_content)
            print(f"\nFile saved to {output_path} ({len(file_content)} bytes)")
            
            # Verify file was written
            if not os.path.exists(output_path):
                print("Error: File was not created")
            else:
                print(f"File size: {os.path.getsize(output_path)} bytes")
                
        except IOError as e:
            print(f"Error saving file: {str(e)}")
            print(f"Current working directory: {os.getcwd()}")

except socket.timeout:
    print("Server not responding")

finally:
    UDPClientSocket.close()
