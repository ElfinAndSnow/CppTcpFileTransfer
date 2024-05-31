#pragma once

#include "SocketServer.hpp"
#include <iostream>

constexpr auto SEND_FILE_ERROR = -1;
constexpr auto SEND_FILE_SUCCESS = 0;

class FileServer : public SocketServer {

public:

    FileServer(char* shareFolder, char* port) : SocketServer(port) {

        _shareFolder = shareFolder;
        
        SendFile();
        
        CloseClient();
    }

protected:

    char* _shareFolder;

	int SendFile() {

        if (!_isconnected)
            return SEND_FILE_ERROR;

        // Receiving file name from user.
        char file_name[BUFFER_SIZE] = { 0 };
        if (RecvData(file_name, BUFFER_SIZE) == SOCKET_ERROR) {
            
            printf("[-]Failed in receiving file name from (%s).\n", _userip);

            return SEND_FILE_ERROR;
        }

        char full_name[BUFFER_SIZE] = { 0 };
        snprintf(full_name, BUFFER_SIZE, "%s%s", _shareFolder, file_name);

        // Sending data
        char buffer[BUFFER_SIZE] = { 0 };
        FILE* fp = fopen(full_name, "r");
        if (fp != NULL) {

            long long int length = 0;
            long long int total_length = 0;

            while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0) {
                if (SendData(buffer, length) == SOCKET_ERROR) {

                    fclose(fp);

                    printf("[-]Error in sending file to (%s).\n", _userip);

                    return SEND_FILE_ERROR;
                }

                memset(buffer, 0, BUFFER_SIZE);
                total_length += length;
            }

            fclose(fp);

            printf("[+]File %s has been sent successfully to (%s).\n", file_name, _userip);
            printf("[+]Total length of file %s: %lld bytes\n", file_name, total_length);
            
            return SEND_FILE_SUCCESS;

        }

        printf("[-]File %s requested by (%s)not found.\n", file_name, _userip);
        
        return SEND_FILE_ERROR;

	}


};