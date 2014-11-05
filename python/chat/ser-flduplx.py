from socket import *
from select import *
addr_info1 = ('127.0.0.1', 4000)
addr_info2 = ('127.0.0.1', 3000)

BUFFER = 2000
sockfd = socket(AF_INET, SOCK_DGRAM)
sockfd.bind(addr_info1)
server = 0
print "user2>"
while True:
	inp = [server, sockfd]
	(rlist, wlist, elist) = select(inp, [], [])
	if rlist[0] ==  server:
		msg = raw_input("user2>\n")
		sockfd.sendto(msg, addr_info2)
	else :
		print "user1> " + sockfd.recvfrom(BUFFER)[0]
		
