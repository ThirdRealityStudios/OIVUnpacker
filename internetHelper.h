#include <stdio.h>
//#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <netdb.h>
#include <time.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>

/// @brief Returns the current time from the internet via NTP.
/// @param ntpServerIPv4Address The IPv4 address string of the NTP server simply.
time_t getLiveTime(char *ntpServerIPv4Address);