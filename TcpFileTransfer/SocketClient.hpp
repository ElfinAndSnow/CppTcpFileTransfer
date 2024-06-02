#pragma once

#include "Socket.hpp"

class SocketClient : public Socket {
	
protected:

	bool _iscreated = false, _isconnected = false;
	
public:

	SocketClient(char* host, char* port) {

		_host = host;
		_port = port;
	}

	int Create() {

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		iResult = getaddrinfo(_host, _port, &hints, &result);
		if (iResult != 0) {

			printf("getaddrinfo failed: %d\n", iResult);

			return 1;
		}

		ptr = result;

		_connectfd = INVALID_SOCKET;
		_connectfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (_connectfd == INVALID_SOCKET) {

			freeaddrinfo(result);

			printf("Error at socket()\n");

			return 1;
		}

		_iscreated = true;

		return 0;
	}

	void Connect() {

		if (!_iscreated)
			return;

		iResult = connect(_connectfd, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {

			_connectfd = INVALID_SOCKET;
		}

		freeaddrinfo(result);

		if (_connectfd == INVALID_SOCKET) {

			printf("[-]Unable to connect to server!\n");

			return;
		}

		printf("[+]Connected to server.\n");

		_isconnected = true;

	}

};