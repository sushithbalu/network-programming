from socket import *
from select import *

addr_info1 = ('127.0.0.1', 3000)
addr_info2 = ('127.0.0.1', 4000)
BUFFER = 2000

sockfd = socket(AF_INET, SOCK_DGRAM)
sockfd.bind(addr_info1)
client = 0
print "user1>"
while True:
	inp = [client, sockfd]
	(rlist, wlist, elist) = select(inp, [], [])
	if rlist[0] == sockfd:
		print "user2> " + sockfd.recvfrom(BUFFER)[0]
	else :
		msg = raw_input("user1>\n")
		sockfd.sendto(msg, addr_info2)
