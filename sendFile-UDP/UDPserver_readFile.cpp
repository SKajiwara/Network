#include<iostream>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string>
#include <fstream>
using namespace std;

#define PORT 8080
#define MSG_CONFIRM 0
const unsigned short int MAX_BUF_SIZE = 1024;

int UDPserver( string );

int main( int argc, char* argv[] ) {
  if( argc > 1 && strlen(argv[1]) > 0)
    UDPserver(argv[1]);
  else
    cout << "No file to send specified" << endl;
  return 0;
}

int UDPserver( string fileName ) {
  // Socket() -> Bind()
  int sockfd;
  struct sockaddr_in servaddr, cliaddr;
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);
  if ( bind(sockfd, (const struct sockaddr *)&servaddr,
        sizeof(servaddr)) < 0 )
  {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

  // handshake with the client
  char handshakeMsg[MAX_BUF_SIZE];
  socklen_t len, n;
  len = sizeof(cliaddr); // len is value/result
  n = recvfrom(sockfd, (char *)handshakeMsg, MAX_BUF_SIZE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,	&len);
	handshakeMsg[n] = '\0';
	printf("Handshake msg received\nClient : %s\n\n", handshakeMsg);

  ifstream readingFile(fileName, ifstream::binary);
  if ( readingFile && strlen(handshakeMsg) > 0) {

    // Get length of file in characters
    readingFile.seekg(0, readingFile.end);
    size_t length = readingFile.tellg();
    readingFile.seekg(0, readingFile.beg);

    // Send the length of the file
    void *p=(void *)&length;
    sendto(sockfd, p, sizeof(length),
      MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    printf("File size sent.\n\n");

    // Send big chunks
    char* buffer = new char [MAX_BUF_SIZE];
    for (int i=0; i<(length/MAX_BUF_SIZE); i++){
      readingFile.read(buffer, MAX_BUF_SIZE);
      sendto(sockfd, (char *)buffer, strlen(buffer),
    		MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    }
    // Send reminders
    readingFile.read(buffer, length % MAX_BUF_SIZE );
    sendto(sockfd, (char *)buffer, strlen(buffer),
      MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len);
    printf("File sent.\n\n");

    delete[] buffer;
    readingFile.close();
  }

  return 0;
}
