import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('172.20.241.9', 20000))
s.sendall(b'04\n')

received_data = b''

chunks = []
while True:
    data = s.recv(1024)
    if len(data) == 0:
        break
    received_data += data
    chunks.append(data.decode('utf-8'))

for i in chunks:
    print(i, end='')

s.close()

decoded_data = received_data.decode('utf-8')

with open('vastaanotettu_data.txt', 'w') as file:
    file.write(decoded_data)