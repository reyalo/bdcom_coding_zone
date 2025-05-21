import socket
import sys
from struct import pack

# Server address (match your server's IP)
server_addr_port = ("10.10.10.11", 8001)

# Create UDP socket
sock = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
sock.settimeout(3)

# Prepare data with exact byte lengths
type_value = 2  # Update operation
length_value = 74  # Fixed size of employee_PL
mask_value = 0x7F  # Enable all fields
id_value = 5
name_value = bytes("Ashik box".ljust(20, '\0'), 'utf-8')  # 20 bytes
gender_value = 1  # Male
birthday_value = bytes("2025-05-19 18:30:00".ljust(20, '\0'), 'utf-8')  # 20 bytes
board_date_value = bytes("2025-05-21 08:56:00".ljust(20, '\0'), 'utf-8')  # Current time (08:56 AM +06)
salary_value = 30000
evaluation_result_value = 1  # A

# Pack the data
header = pack('!B H', type_value, length_value)  # employee_HD (3 bytes)
payload = pack('!I I 20s B 20s 20s I B',
              mask_value, id_value, name_value, gender_value,
              birthday_value, board_date_value, salary_value, evaluation_result_value)  # employee_PL (74 bytes)
data = header + payload

# Debug: Print sent data details
print(f"Header size: {len(header)} bytes")
print(f"Payload size: {len(payload)} bytes")
print(f"Total size: {len(data)} bytes")
print(f"Sending data: {data.hex()}")

# Send the packet
sock.sendto(data, server_addr_port)

# Optional: Receive response
try:
    response, addr = sock.recvfrom(1024)
    print(f"Received response: {response.decode()}")
except socket.timeout:
    print("No response from server (timeout)")
except Exception as e:
    print(f"Error: {e}")

sock.close()