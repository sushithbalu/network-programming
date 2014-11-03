from socket import *

fd = socket(AF_INET, SOCK_DGRAM)
i = raw_input()
while(1):
	fd.sendto(i, ('127.0.0.1', 4000))	
	x =  fd.recvfrom(100)
	print x[0]
