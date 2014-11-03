#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT 4000
#define HISPORT 3000
#define MAXS 100

void errxt(char *s);

int main(char argc,char **argv)
{
	if (argc != 2) {
		fprintf(stderr,"usage: no address\n");
		exit(EXIT_FAILURE);
	}

	int sockfd;
	struct sockaddr_in sa_addr;
	struct sockaddr_in their_addr;
	int addr_len, numbytes;
	char buf[MAXS];
	char array[MAXS];
	struct timeval tv;
	int r, s;

	fd_set readfds;
	fd_set master;

	tv.tv_sec = 5;
	tv.tv_usec = 0;

/* socket */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		errxt("socket failed");
	}

	memset (&(sa_addr.sin_zero),'\0', sizeof(sa_addr));
	sa_addr.sin_family = AF_INET;
	sa_addr.sin_port = htons(MYPORT);
	sa_addr.sin_addr.s_addr = INADDR_ANY;

	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(HISPORT);
	their_addr.sin_addr.s_addr = inet_addr(argv[1]);

/* bind */
	if (bind(sockfd, (struct sockaddr *)&sa_addr, sizeof(sa_addr)) == -1) {
		errxt("Failed to bind");
	}

	addr_len = sizeof(their_addr);

	FD_ZERO(&readfds);
	FD_ZERO(&master);
	FD_SET(0,&master);
	FD_SET(sockfd, &master);	
	
	s = sockfd + 1;	
	
	for(;;) {	
		readfds = master;
		r = select(s, &readfds, NULL, NULL, NULL);
		if (FD_ISSET(sockfd, &readfds)) {
			if ((numbytes = recvfrom(sockfd, buf, MAXS-1, 0,
				(struct sockaddr *)&their_addr, &addr_len)) == -1) {
				errxt("failed to receive");
			}
			buf[numbytes] = '\0';
			printf("user1 : %s \n", buf);
		} else if (FD_ISSET(0, &readfds)) {
			printf("sent\n");
			while(fgets(array, sizeof(array), stdin) != NULL)
				break;
			if ((numbytes = sendto(sockfd, array, strlen(array),0,
				(struct sockaddr *)&their_addr,sizeof(struct sockaddr))) == -1) {
				errxt("Failed to send");
				}
			}
		}
	close(sockfd);
	return 0;
}

void errxt(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}
