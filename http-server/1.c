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
	int sockfd, sockfd_new;
	struct sockaddr_in sa;
	struct sockaddr_in tr;
//	struct a;
//	struct addrinfo *ptr;
	int len, optval;
	char c;

	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(MYPORT);
	sa.sin_addr.s_addr = INADDR_ANY;
/*socket*/
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		errxt("socket");
	}
/*bind*/
	if(bind(sockfd,(struct sockaddr *)&sa, sizeof sa) == -1){
			errxt("bind");
	}
/*setsockopt get options on socket*/
	optval = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
		 &optval, sizeof optval) == -1){
		errxt("setsockopt");
	}
	if(listen(sockfd, 4) != 0){
		errxt("listen");
	}
	for(;;){
		len = sizeof(struct sockaddr);
		if((sockfd_new = accept(sockfd, (struct sockaddr *)&tr, &len)) < 0){
			errxt("accept");
		} else{
			if(!fork()){
				close(sockfd);
				response_function(sockfd_new);
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
	char *path, *ptr;
	int i;
	path = getenv("PWD");
	memset((void*)msg, '\0', sizeof(msg));
	if(recv(sockfd, msg, sizeof(msg), 0) == -1){
		errxt("recv1");
	}else{
		printf("%s", msg);
		reqline[0] = strtok(msg, " \t\n"); 
		if(strncmp(reqline[0], "GET", 4) == 0){
			reqline[1] = strtok(NULL, " \t");
			reqline[2] = strtok(NULL, " \t\n");
		
			if(strncmp(reqline[2], "HTTP/1.1", 8) != 0){
				write(sockfd, "Bad requist\n", 12);
			}else{
				if(strncmp(reqline[1], "/\0", 2) == 0){
					reqline[1] = "/index.html";
					printf("%s", reqline[1]);
				}
	
				ptr = strdup(path);
				strcpy(&ptr[strlen(path)], reqline[1]);
				printf("file:%s\n", ptr);
				if((fd = open(ptr, O_RDONLY)) != -1){
					printf("OK\n");
					send(sockfd, "HTTP/1.1 200 OK\n\n", 17, 0);
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

