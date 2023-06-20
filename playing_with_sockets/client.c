#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include "readline.c"
#define SECRET "<30617spr>/n"
#define PORT 3060
int main(int argc, char *argv[])
{
    int sockfd;
    int len;
    struct sockaddr_in address;
    int result;
    
    /*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(argv[1]);
    address.sin_port = htons(PORT);
    len = sizeof(address);

    /*  Connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client3");
        exit(1);
    }

    /*  Now read/write via sockfd.  */

    char line[3];  //char + '\n' + '\0'

    //Keep reading chars from terminal until user types ^-d:
    printf("Enter char: ");
    while (fgets(line,3,stdin) != NULL) {
        write(sockfd, line, 1);
        char ch;
        read(sockfd, &ch, 1);
        printf("char from server = %c\n", ch);
        printf("Enter char: ");
    }

    close(sockfd);

    exit(EXIT_SUCCESS);
}
void handle_protocol(int sockfd,char* argv2){
	char* line;
	while((line=readline(sockfd))!=NULL&&!errno){
		if(strncmp(line, "<remcp>/n/0", 15)==0){
			write(sockfd, SECRET, 10);
		}
		else if(strncmp(line, "<ok>/n/0", 15)==0){
			write(sockfd, argv2, PATH_MAX);
			char* buff[4096];
			int fd;
			fd=open(argv2, O_WRONLY|O_CREAT, 0666);
			while(read(sockfd, buff, 4096)>0){
				write(fd, buff, 4096);
			}
		}
	}
	close(sockfd);
}

