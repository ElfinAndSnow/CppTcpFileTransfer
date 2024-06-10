#pragma once

#include "SocketClient.hpp"

#include <cstdio>
#include <filesystem>
#include <time.h>

constexpr int SET_FILEPATH_ERROR = 1;
constexpr int SET_FILEPATH_SUCCESS = 0;

constexpr int RECV_FILE_ERROR = 1;
constexpr int RECV_FILE_SUCCESS = 0;

class FileClient : public SocketClient {

public:
  FileClient(char *host, char *port, char *filename, char *savepath)
      : SocketClient(host, port) {

    _filename = filename;
    _savepath = ensureSlashAtEnd(savepath);

    if (!std::filesystem::exists(_savepath)) {

      printf("[-]Path (%s) doesn't exist.", _savepath);

      return;
    }

    Launch();

    delete (_savepath);
  }

  void Launch() {

    Init();

    Create();
    Connect();

    SetFilePath();
    RecvFile();

    CloseClient();
  }

protected:
  char *_filename;
  char *_savepath;
  char _fullpath[BUFFER_SIZE] = {0};

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

    char buffer[BUFFER_SIZE] = {0};
    FILE *fp;

    clock_t start_time = clock();
    clock_t current_time;
    double elapsed_time;
    int speed;
    long fileSize = 0;

    if (RecvData((char *)&fileSize, sizeof(fileSize)) == SOCKET_ERROR) {

      printf("[-]Failed in receiving the file size.\n");

      return RECV_FILE_ERROR;
    }

    if (fileSize <= 0) {

      printf("[-]Requested file doesn't exist.\n");

      return RECV_FILE_ERROR;
    }

    printf("[+]File size: %ld KB.\n", (fileSize / 1024));

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

        current_time = clock();
        elapsed_time = (double)(current_time - start_time) / CLOCKS_PER_SEC;
        speed = (int)total_length / elapsed_time;
        double percentage = (double)total_length / fileSize;
        printProgressbar(percentage, speed);
      }

      printf("\n");

      if (iResult < 0) {

        perror("[-]Error in receiving file.");

        return RECV_FILE_ERROR;
      }

      printf("[+]Total bytes received: %lld\n", total_length);

      fclose(fp);

      return RECV_FILE_SUCCESS;
    }

    return RECV_FILE_ERROR;
  }

  void printProgressbar(double percentage, int speed) {

    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);

    int barWidth = 20;
    int barLength = barWidth * percentage;

    std::cout << ("\r[+]Downloading... [");
    for (int i = 0; i < barWidth; i++) {

      if (i < barLength)
        std::cout << "=";
      else if (i == barLength)
        std::cout << ">";
      else
        std::cout << " ";
    }

    std::cout << "] " << (int)(percentage * 100.0)
              << "%, current speed: " << (speed / 1024) << " KB/s";

    std::cout.flush();

    setvbuf(stdout, NULL, _IONBF, BUFSIZ);
  }
};
