#include<stdio.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<signal.h>

#define MAXS 10000
#define MAXM 1000000
#define MYPORT 3000

void errxt(char *s);
void response_function(int);

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in sa;
	struct addrinfo hints;
	struct addrinfo *ptr, *p;
	int len;
	char c;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	
	if(getaddrinfo(NULL, "3000", &hints, &ptr) != 0){
		errxt("getaddrinfo");
	}
	for(p = ptr; p != NULL; p = p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, 0)) == -1){
			continue;
		}
		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
			break;
		}
	}
	if(p == NULL){
		errxt("socket");
	}
	freeaddrinfo(ptr);
	if(listen(sockfd, 10) != 0){
		errxt("listen");
	}
	for(;;){
		len = sizeof(struct sockaddr);
		if(accept(sockfd, (struct sockaddr *)&sa, &len) < 0){
			errxt("accept");
		} else{
			if(fork != 0){
				response_function(sockfd);
				exit(EXIT_FAILURE);
			}
		}
	}

	return 0;
}

void response_function(int sockfd)	
{
	char msg[MAXM], *reqline[3], buf[MAXS];
	int recvd, fd, bytes_read;
	char *path;
	
	 getenv("PWD");
	memset((void*)msg, 0, sizeof(msg));
	if(recv(sockfd, msg, sizeof(msg), 0) == -1){
		errxt("recv");
	}else{
		printf("%s", msg);
		reqline[0] = strtok(msg, " \t\n");
		if(strncmp(reqline[0], "GET0", 4) == 0){
			reqline[1] = strtok(NULL, " \t");
			reqline[2] = strtok(NULL, " \t\n");
			if(strncmp(reqline[2], "HTTP/1.0", 8) != 0){
				write(sockfd, "Bad requist\n", 13);
			}else{
				if(strncmp(reqline[1], "/0", 2) == 0){
					reqline[1] = "/index.html";
				}
				printf("file:%s\n", path);
				if((fd = open(path, O_RDONLY)) != -1){
					send(sockfd, "HTTP/1.0 200 OK\n\n", 17, 0);
					while((bytes_read = read(fd, buf, MAXS)) > 0){
						write(sockfd, buf, bytes_read);
					}
				}else {
					write(sockfd, "failed\n", 7);
				}
			}
		}
	}
	close(sockfd);
}

void errxt(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}


