from socket import socket
import os
BACKLOG = 10
addr_info = ('127.0.0.1', 4000)


def res_fun(sockfd_n):
	x = sockfd_n.recvfrom(100)
	print x[0]
	x = x[0][:100].split()
	if (x[0] == 'GET'):
		if (x[1] == '/apple.html'):
			fileo = os.getcwd() + x[1]
			fd = os.open(fileo, os.O_RDONLY)
			string = os.read(fd)
			while  string:
				os.write(sockfd_n, string)
				string = os.read(fd)

sockfd = socket()
sockfd.bind(addr_info)
sockfd.listen(BACKLOG)
while True:
	(sockfd_new, from_port) = sockfd.accept()
	if(os.fork())== 0:
		socket.close(sockfd)
		res_fun(sockfd_new)
		socket.close(sockfd_new)
		exit(0)
	
	socket.close(sockfd_new)
	
