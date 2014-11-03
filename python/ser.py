from socket import *
fd=socket(AF_INET, SOCK_DGRAM)
fd.bind(('127.0.0.1', 4000))
i = raw_input()
while(1):
	x = fd.recvfrom(100)
	fd.sendto(i, x[1])
	print x[0]
close(fd);
