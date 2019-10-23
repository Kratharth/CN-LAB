from socket import *

serverName = "10.124.6.95"
serverPort = 12000
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind((serverName, serverPort))
print('Server Listening')
print("The server is ready to receive ")
while True:
	fileName, addr = serverSocket.recvfrom(1024)
	fileName = fileName.decode()
	with open(fileName, "r") as f:
		message = f.read() 
	serverSocket.sendto(message.encode(), addr)
	
