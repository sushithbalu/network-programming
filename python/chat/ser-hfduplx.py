from socket import *
fd=socket(AF_INET, SOCK_DGRAM)
fd.bind(('127.0.0.1', 4000))
print "socket ready!"
while(1):
	x = fd.recvfrom(100)
	print x[0]
	i = raw_input("ser> ")
	fd.sendto(i, x[1])
	print "\nwaiting for client's response.."
