## Network
My work from Network class
1. File copy with a maximum buffer size using C++
  - This creates a copy file of a file you specified adding .copy after the file name.
  - To run: ```$ g++ copy.cpp -o copy
  $ ./copy.cpp file_name```
2. Send a file over a local network with a buffer size using UDP.
  - This sends a file to a client to server (should've been opposite...).
  - Run the server followed by the client on different terminals.
  - To run: 
  
  ```
  $ g++ UDPclient_writeFile.cpp -o client
  $ ./client file_name

  $ g++ UDPserver_readFile.cpp -o server
  $ ./server file_name
  ```
3. Just exchanging messages using UDP over a local network
  - [C](https://github.com/SKajiwara/Network/tree/master/local_communication/C)
  - [C++](https://github.com/SKajiwara/Network/tree/master/local_communication/CPP)
### Code references
1. [c++ implementation of UPD client/server]( https://www.daniweb.com/programming/software-development/threads/429287/udp-client-server)
2. [c implementation of UPD client/server](https://www.geeksforgeeks.org/udp-server-client-implementation-c/)
