#include <stdio.h>
#include <time.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

#pragma comment(lib, "Ws2_32.lib")

time_t getLiveTime()
{
	// TODO Use multiple servers if one is not reachable.. Also: resolve hostnames to ip for easier NTP server reading..
	char *hostname="130.149.7.7";//"131.173.16.220";//"tick.usno.navy.mil";
	int portno=123;     //NTP is port 123
	int maxlen=1024;        //check our buffers
	long i;          // misc var i
	char msg[48]={8,0,0,0,0,0,0,0,0};    // the packet we send
	char buf[1024]; // the buffer we get back
	struct protoent *proto;     //
	struct sockaddr_in server_addr;
	SOCKET s;  // socket
	time_t tmit;   // the time -- This is a time_t sort of


	//=====================================================================================
	// Very Windows specific here:
	//=====================================================================================
    //Initialise the winsock stack
    WSADATA wsaData;
    BYTE wsMajorVersion = 1;
    BYTE wsMinorVersion = 1;
    WORD wVersionRequested = MAKEWORD(wsMinorVersion, wsMajorVersion);   
    
	if(WSAStartup(wVersionRequested, &wsaData) != 0) 
    {
        fputs("Failed to load winsock stack\n", stderr);
        WSACleanup();
        
		return -1;
    }
    
	if(LOBYTE(wsaData.wVersion) != wsMajorVersion || HIBYTE(wsaData.wVersion) != wsMinorVersion)
    {
        fputs("Winsock stack does not support version which this program requires\n", stderr);
        WSACleanup();
		
        return -1;
    }
	//=====================================================================================

	// Create a UDP socket
	proto = getprotobyname("udp");
	int err = GetLastError();
	s = socket(PF_INET, SOCK_DGRAM, proto->p_proto);
	
	// Check if connection establishment was successful.
	if(s == INVALID_SOCKET)
	{
		fputs("Error: could not connect to the internet (are you offline?) (Hence, exiting now..)", stderr);
		
		exit(EXIT_FAILURE);
	}

	memset( &server_addr, 0, sizeof( server_addr ));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(hostname);
	server_addr.sin_port=htons(portno);

	/*
	 * build a message.  Our message is all zeros except for a one in the
	 * protocol version field
	 * msg[] in binary is 00 001 000 00000000 
	 * it should be a total of 48 bytes long
	*/

	// Send the NTP data
	i = sendto(s,msg,sizeof(msg),0,(struct sockaddr *)&server_addr,sizeof(server_addr));

	int iResult = -1;

	iResult = recv(s, buf, 1024, 0);

	if(iResult == 0)
	{
		fputs("Error: the server did not respond correctly (no payload data) (Hence, exiting now..)", stderr);
		
		exit(EXIT_FAILURE);
	}
	else if(iResult < 0)
	{
		fprintf(stderr, "Error: response of the contains an error (Code %d) (Hence, exiting now..)", WSAGetLastError());
		
		exit(EXIT_FAILURE);
	}

	unsigned long *timePtr = (long*) &buf[32]; // Only read the seconds (first 4 bytes = seconds)

	tmit = ntohl(*timePtr); // inverse bits to get the "real" time.

	// As defined in RFC for NTP, a difference must be applied for the value
	tmit -= 2208988800L;

	return tmit;
}