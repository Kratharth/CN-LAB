from socket import *

serverSocket = socket(AF_INET, SOCK_STREAM)
serverPort = 12000
serverName = "10.90.2.154"
serverSocket.bind((serverName, serverPort))
serverSocket.listen(1)

while(True):
	conn, addr = serverSocket.accept()
	fileName = conn.recv(1024)
	with open(fileName.decode(), "r") as f:
		message = f.read()
	conn.send(message.encode())
	conn.close()
