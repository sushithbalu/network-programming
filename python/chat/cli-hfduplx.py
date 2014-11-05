from socket import *

fd = socket(AF_INET, SOCK_DGRAM)
while(1):
	i = raw_input("cli> ")
	fd.sendto(i, ('127.0.0.1', 4000))
	print "\nwaiting for server's response.."
	print fd.recvfrom(100)[0]
