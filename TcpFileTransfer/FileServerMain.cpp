#include "FileServer.hpp"

int main(int argc, char* argv[]) {

	if (argc == 3) {

		char* path = argv[1];
		char* port = argv[2];

		while (1)
			FileServer server = FileServer(path, port);

	}

	return 0;
}