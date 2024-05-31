#include "FileClient.hpp"

int main(int argc, char* argv[]) {

	if (argc == 5) {

		char* host = argv[1];
		char* port = argv[2];
		char* filename = argv[3];
		char* savepath = argv[4];

		FileClient client = FileClient(host, port, filename, savepath);

	}

	return 0;
}