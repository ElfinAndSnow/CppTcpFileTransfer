#pragma once

#ifdef _WIN32
// On Windows
#pragma comment(lib, "Ws2_32.lib")

#ifndef WIN32_LEAD_AND_MEAN
#define WIN32_LEAD_AND_MEAN
#endif

#include <winsock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <Windows.h>

#else
// On Unix
typedef int SOCKET;
constexpr auto INVALID_SOCKET = -1;
constexpr auto SOCKET_ERROR = -1;

#define sprintf_s sprintf

#include <sys/sendfile.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>

constexpr auto BUFFER_SIZE = 1024;
constexpr auto DEFAULT_PORT = "50001";

class Socket {

public:

	SOCKET _sockfd;
	SOCKET _connectfd;

	char* _host = NULL;
	char* _port = NULL;

	struct addrinfo* result = NULL, * ptr = NULL, hints;

	int iResult;

	bool Init() {

#ifdef _WIN32
		WSADATA wsaData;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed: %d\n", iResult);
			return 0;
		}
		return 1;
#else
		return 1;
#endif

	}

	int WinsockCleanup() {

#ifdef _WIN32
		return WSACleanup();
#else
		return 0;
#endif

	}

	void CloseListen()
	{
#ifdef _WIN32
		closesocket(_sockfd);
		WSACleanup();
#else
		close(_sockfd);
#endif
	}

	void CloseClient() {
		
#ifdef _WIN32
		closesocket(_connectfd);
		WSACleanup();
#else
		close(_connectfd);
#endif

	}

	int SendData(void* buf, size_t len)
	{
		int res = send(_connectfd, (const char*)buf, len, 0);
		return res;
	}


	int RecvData(void* buf, size_t len)
	{
		int res = recv(_connectfd, (char*)buf, len, 0);
		return res;
	}

};