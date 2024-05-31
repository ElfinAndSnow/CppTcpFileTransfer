#pragma once

#include "SocketClient.hpp"

constexpr int SET_FILEPATH_ERROR = 1;
constexpr int SET_FILEPATH_SUCCESS = 0;

constexpr int RECV_FILE_ERROR = 1;
constexpr int RECV_FILE_SUCCESS = 0;

class FileClient : public SocketClient {

public:

	FileClient(char* host, char* port, char* filename, char* savepath) : SocketClient(host, port){

		_host = host;
		_port = port;
		_filename = filename;
		_savepath = savepath;

		SetFilePath();
		RecvFile();

		CloseClient();
	}

protected:

	char* _filename;
	char* _savepath;
	char _fullpath[BUFFER_SIZE] = { 0 };

	bool _isfilepathset = false;

	int SetFilePath() {

		if (!_isconnected)
			return SET_FILEPATH_ERROR;

		iResult = SendData(_filename, (int)strlen(_filename));
		if (iResult == SOCKET_ERROR) {

			perror("[-]Failed to send filename to server.");

			return SET_FILEPATH_ERROR;
		}

		printf("[+]Requested file %s\n", _filename);

		snprintf(_fullpath, sizeof(_fullpath), "%s%s", _savepath, _filename);
		printf("[+]The file will be saved in the path %s\n", _fullpath);

		_isfilepathset = true;

		return SET_FILEPATH_SUCCESS;
	}

	int RecvFile() {

		if (!_isfilepathset)
			return RECV_FILE_ERROR;

		char buffer[BUFFER_SIZE] = { 0 };
		FILE* fp;
		int bytes_read;

		fp = fopen(_fullpath, "w");
		if (fp != NULL) {

			long long int length = 0;
			long long int total_length = 0;
			
			// Receiving data
			while ((length = RecvData(buffer, BUFFER_SIZE)) > 0) {

				iResult = length;

				if (fwrite(buffer, sizeof(char), length, fp) != length) {

					perror("[-]Error in writing to file.");

					break;
				}
				total_length += length;
				memset(buffer, 0, BUFFER_SIZE);
			}

			if (iResult < 0) {
				
				perror("[-]Error in receiving file.");

				return RECV_FILE_ERROR;
			}

			printf("[+]Total bytes received: %lld\n", total_length);

			fclose(fp);

			return RECV_FILE_SUCCESS;

		}

		fclose(fp);

		return RECV_FILE_ERROR;

	}

};