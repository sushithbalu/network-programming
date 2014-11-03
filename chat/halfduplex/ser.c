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
#define MAXS 100

void errxt(char *);

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in sa;
	struct sockaddr_in their_addr;
	int numbytes, addr_len;
	char msg[MAXS];
	char array[MAXS];

/* socket */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		errxt("socket Failed");
	}
	memset (&(sa.sin_zero), 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(MYPORT);
	sa.sin_addr.s_addr = INADDR_ANY;

/* bind */	
	if (bind(sockfd, (struct sockaddr *)&sa,sizeof(struct sockaddr)) == -1) {
		errxt("Failed to bind");
	}

	addr_len = sizeof(their_addr);
	for(;;) {
/*receive*/
		if ((numbytes = recvfrom(sockfd, msg, MAXS-1, 0,
			(struct sockaddr *)&their_addr, &addr_len)) == -1) {
			errxt("recvfrom Failed");
		}
		msg[numbytes] = '\0';
		printf("client: %s\n", msg);
		printf("server: ");
/*send*/
		while((fgets(array,sizeof(array), stdin)) != NULL)
			break;
		printf("waiting for client's response..\n");
		if ((numbytes = sendto(sockfd, array, strlen(array),0,
			(struct sockaddr *)&their_addr,sizeof(struct sockaddr))) == -1) {
			errxt("sendto Failed");
		}
	}
/* close */
	close(sockfd);

	return 0;
}

void errxt(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}
