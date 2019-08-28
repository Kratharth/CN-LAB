from socket import *

serverName = "10.90.2.154"
serverPort = 12000
serverSocket = socket(AF_INET, SOCK_DGRAM)
serverSocket.bind((serverName, serverPort))
print("The server is ready to receive ")
while True:
	fileName, addr = serverSocket.recvfrom(1024)
	fileName = fileName.decode()
	with open(fileName, "r") as f:
		message = f.read() 
	serverSocket.sendto(message.encode(), addr)
	
