#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
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
#define SECRET "<30617spr>\n"
#define PORT 3060
void handle_client(int sockfd);
//ls -of
int main()
{
    int server_sockfd, client_sockfd, stat;
    socklen_t server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);
    server_len = sizeof(server_address);
    if(bind(server_sockfd, (struct sockaddr *)&server_address, server_len)==-1){
		perror("bind failed");
		return EXIT_FAILURE;
	}
	int i=1;
	setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));
    listen(server_sockfd, 5);

    while(1) {
        printf("server waiting\n");
        client_len = sizeof(client_address);
        client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
        if(fork()==0){
			handle_client(client_sockfd);
			exit(EXIT_SUCCESS);
		}
    }
    close(client_sockfd);
    while(wait(&stat)!=-1){
		if(!WIFEXITED(stat)||WEXITSTATUS(stat)!= EXIT_SUCCESS)
			return EXIT_FAILURE;
	}
    return EXIT_SUCCESS;
}
void handle_client(int sockfd){
	char* line;
	write(sockfd, "<remcp>\n", 10);
	while((line=readline(sockfd))!=NULL&&!errno){
		if(strncmp(line, SECRET, 15)==0){
			write(sockfd, "<ok>\n", 10);
			printf(SECRET);
		}
		else if(line[0]!='<'){
			int fd;
			char buff[4096];
			line[sizeof(line)-2]='\0';
			if((fd=open(line,O_RDONLY))==-1){
				while(int nread = read(fd, buff, 4096)>0)
					write(sockfd, buff, nread);
			}
			else{
				perror("error opening file");
			}
		}
	}
	close(sockfd);
}
			
				//const char *ok="<ok>/n"
				//write(fd,"<ok>/n", 7)
				//if (strcmp(buff, ok)==/0)
				//cannot explain what a race condition is just understand simple ones in code
