#pragma once

#include "Socket.hpp"

class SocketServer : public Socket {

protected:

	char _userip[INET_ADDRSTRLEN] = { 0 };
	struct sockaddr_in _client_addr;

	bool _iscreated = false, _isbound = false, _islistening = false, _isconnected = false;

public:

	SocketServer(char* port){

		_port = port;
	}

	int Create() {

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		iResult = getaddrinfo(NULL, _port, &hints, &result);
		if (iResult != 0) {

			printf("[-]getaddrinfo() failed: %d\n", iResult);
			perror("[-]Failed to create socket.");
			
			return 1;
		}

		_sockfd = INVALID_SOCKET;
		_sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

		if (_sockfd == INVALID_SOCKET) {

			freeaddrinfo(result);

			printf("[-]Error at socket()");
			perror("[-]Failed to create socket.");

			return 1;
		}

		// Reuse of the ports.
		int opt = 1;
		if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*) & opt, sizeof(opt)) < 0) {

			perror("[-]Failed to reuse the port.");
			
			return 1;
		}


		printf("[+]Create Socket successful.\n");

		_iscreated = true;

		return 0;
	}

	void Bind() {

		if (!_iscreated)
			return;

		iResult = bind(_sockfd, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			
			freeaddrinfo(result);

			perror("[-]Error in binding.");

			return;
		}

		freeaddrinfo(result);

		printf("[+]Binding successful.\n");
		printf("[+]Listening on the port: %s\n", _port);

		_isbound = true;

	}

	void Listen() {

		if (!_isbound)
			return;

		if (listen(_sockfd, SOMAXCONN) == SOCKET_ERROR) {

			perror("[-]Error in listening.");

			return;
		}

		printf("[+]Listening...\n");

		_islistening = true;

	}

	void Accept() {

		if (!_islistening)
			return;

		socklen_t client_addr_size = sizeof(_client_addr);

		_connectfd = INVALID_SOCKET;
		_connectfd = accept(_sockfd, (struct sockaddr*)&_client_addr, &client_addr_size);
		inet_ntop(AF_INET, &(_client_addr.sin_addr), _userip, INET_ADDRSTRLEN);
		if (_connectfd == INVALID_SOCKET) {

			printf("[-]Failed in creating a connectfd for (%s).\n", _userip);

			return;
		}

		printf("[+]Succeeded in creating a connectfd for (%s).\n", _userip);

		_isconnected = true;

	}

};