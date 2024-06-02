#include "FileServer.hpp"

int main(int argc, char* argv[]) {

	if (argc < 3) {
		fprintf(stderr,
			"[-]Too few arguments. Usage: %s <port> <path>\n",
			argv[0]);

		return -1;
	}
	else if (argc > 3) {
		fprintf(stderr,
			"[-]Too many arguments. Usage: %s <port> <path>\n",
			argv[0]);

		return -1;
	}

	char* port = argv[1];
	char* path = argv[2];

		while (1)
		FileServer server = FileServer(port, path);

	return 0;
}