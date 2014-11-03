#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MYPORT 4000
#define MAXS 100

int main(int argc, char **argv)
{
	int sockfd;
	int numbytes, addr_len;
	struct sockaddr_in their_addr;
	struct hostent *he;
	char array[MAXS];
	char buf[MAXS];

	if (argc != 2) {
		fprintf(stderr,"usage : %s ip \n", argv[1]);
		exit(EXIT_FAILURE);
	}
	if ((he=gethostbyname(argv[1])) == NULL) {
		errxt("gethostbyname");
	}
/* socket function */
	if ((sockfd = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
		errxt("socket");
	}
/* clean and assign */
	memset(&(their_addr.sin_zero), 0, sizeof(their_addr));
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(MYPORT);
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);

/* send and recieve */
	addr_len = sizeof(their_addr);
	for(;;) {
		printf("client: ");
		while(fgets(array, sizeof(array), stdin) != NULL )
			break;
		if ((numbytes=sendto(sockfd, array, strlen(array), 0, 
			(struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
			errxt("sendto");
		}

		printf("sent %d bytes to %s\n", numbytes, inet_ntoa(their_addr.sin_addr));
		printf("waiting for sever's response..\n");
			 
		if ((numbytes = recvfrom(sockfd, buf, MAXS - 1, 0,
			(struct sockaddr *)&their_addr, &addr_len)) == -1) {
			errxt("recvfrom");
		}
		buf[numbytes] = '\0';
		printf("server: %s\n", buf);
	}
	close(sockfd);

	return 0;
}

void errxt(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

