#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

#define PORT 8080
#define MAXLINE 1024
#define MSG_CONFIRM 0

int main()
{
  int sockfd;
  char buffer[MAXLINE];
  char hello[] = "Hello from server";
  struct sockaddr_in servaddr, cliaddr;

  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  // Bind the socket with the server address
  if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
  {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

  socklen_t len, n;
  len = sizeof(cliaddr); // len is value/result
  n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,	&len);
	buffer[n] = '\0';
	printf("Client : %s\n", buffer);
  // Made MSG_CONFIRM => 0
	sendto(sockfd, (const char *)hello, strlen(hello),
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
	printf("Hello message sent.\n");

  return 0;
}
