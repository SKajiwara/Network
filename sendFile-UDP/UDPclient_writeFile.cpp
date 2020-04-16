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

int UDPclient( string );

int main( int argc, char* argv[] ) {
  if (argc > 1 && strlen(argv[1]) > 0)
    UDPclient(argv[1]);
  else
    cout << "No copy file name specified" << endl;
  return 0;
}

int UDPclient( string fileName ){
  int sockfd;
  struct sockaddr_in servaddr;
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }
  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  char handshakeMsg[] = "Hello this is client";
  socklen_t n, len;

  // Send handshake message
  sendto(sockfd, (const char *)handshakeMsg, strlen(handshakeMsg),
    MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
  printf("\nHandshake message sent.\n\n");

// Open or Create file to write copy
  ofstream copyFile(fileName + ".copy", ifstream::binary);
  if ( copyFile ) {
    size_t length = 0;
    void *p = (void *) &length;
    n = recvfrom(sockfd, p, MAX_BUF_SIZE,
          MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    //printf("%u %lu\n", n, length);

    char* buffer = new char [MAX_BUF_SIZE];
    for (int i=0; i<(length/MAX_BUF_SIZE); i++) {
      n = recvfrom(sockfd, (char *)buffer, MAX_BUF_SIZE,
            MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
      copyFile.write(buffer, MAX_BUF_SIZE);
    }
    // Receive and write reminders
    n = recvfrom(sockfd, (char *)buffer, MAX_BUF_SIZE,
          MSG_WAITALL, (struct sockaddr *) &servaddr, &len);
    buffer[n] = '\0';
    copyFile.write(buffer, length % MAX_BUF_SIZE);
    printf("File received.");
    delete[] buffer;
    copyFile.close();
  }

  close(sockfd);
  return 0;
}
