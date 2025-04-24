import socket
from struct import pack, unpack

# Server config
serverAddressPort = ("10.10.10.11", 5050)
bufferSize = 1024

# TLV types
TLV_TYPE_FILENAME = 1
TLV_TYPE_FILEDATA = 2
TLV_TYPE_ERROR = 3

# Create socket
UDPClientSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
UDPClientSocket.settimeout(3)

# Get filename
filename = input("Enter filename to request from server: ").strip().encode()

# Construct TLV packet: 1 byte type, 4 byte length, payload
packet = pack("!BI", TLV_TYPE_FILENAME, len(filename)) + filename
UDPClientSocket.sendto(packet, serverAddressPort)

try:
    # Receive TLV response
    msgFromServer = UDPClientSocket.recvfrom(bufferSize)
    data = msgFromServer[0]

    # Parse TLV response
    tlv_type = data[0]
    tlv_len = unpack("!I", data[1:5])[0]
    value = data[5:5+tlv_len].decode(errors="ignore")

    if tlv_type == TLV_TYPE_FILEDATA:
        print("✅ File content received:\n")
        print(value)
    elif tlv_type == TLV_TYPE_ERROR:
        print("❌ Server error:", value)
    else:
        print("⚠️ Unknown TLV type:", tlv_type)

except socket.timeout:
    print("❌ Server not responding")
finally:
    UDPClientSocket.close()
