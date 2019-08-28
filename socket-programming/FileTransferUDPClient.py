from socket import *

serverPort = 12000
serverName = "10.90.2.154"
clientSocket = socket(AF_INET, SOCK_DGRAM)
fileName = input("Enter the file name ").encode()
clientSocket.sendto(fileName, (serverName, serverPort))
fileContent, serverAddr = clientSocket.recvfrom(1024)
print(fileContent.decode())
clientSocket.close()
