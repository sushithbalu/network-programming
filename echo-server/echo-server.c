#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT 4000
#define BACKLOG 5
#define MAXS 100

void errxt(char *s);

int main(void)
{
	int sockfd, sockfd_new;
	struct sockaddr_in sa;
	struct sockaddr_in tr;
	int sin_size;
	char buf[MAXS];
	int numbytes, optval;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		errxt("socket");
	}

	memset(&(sa.sin_zero),0 ,sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(MYPORT);
	sa.sin_addr.s_addr = INADDR_ANY;

	if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
		errxt("bind");
	}

	optval = 1;
	if (setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, 
		&optval, sizeof optval) == -1) {
		errxt("setsockopt");
	}	
	printf("server: waiting for connections..\n");
	if (listen(sockfd, BACKLOG) == -1) {
		errxt("listen");
	}
	for(;;) {
		sin_size = sizeof(struct sockaddr_in);
		if ((sockfd_new = accept(sockfd, 
			(struct sockaddr *)&tr, &sin_size)) == -1) {
			errxt("accept");
		}
		printf("server : connection established with %s\n",inet_ntoa(tr.sin_addr));
		if (!fork()) {
			close(sockfd);
			for(;;) {
				if ((numbytes = recv(sockfd_new, buf, MAXS-1, 0)) == -1) 
					perror("recv");
				buf[numbytes] = '\0';
				printf("received %d bytes\n", numbytes);
				if (send(sockfd_new, buf, strlen(buf), 0) == -1){
					errxt("send");
				}
			}
			close(sockfd_new);
		}
		close(sockfd_new);
	}
	return 0;
}

void errxt(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}
