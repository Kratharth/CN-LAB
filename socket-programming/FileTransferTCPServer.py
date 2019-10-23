from socket import *

serverSocket = socket(AF_INET, SOCK_STREAM)
serverPort = 12000
serverName = "10.124.6.95"
serverSocket.bind((serverName, serverPort))
print('Server Listening at port 12000')
serverSocket.listen(1)

while(True):
	conn, addr = serverSocket.accept()
	fileName = conn.recv(1024)
	with open(fileName.decode(), "r") as f:
		message = f.read()
	conn.send(message.encode())
	conn.close()
