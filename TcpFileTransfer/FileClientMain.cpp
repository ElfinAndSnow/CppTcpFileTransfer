#include "FileClient.hpp"

int main(int argc, char* argv[]) {

	if (argc < 5) {
		fprintf(stderr,
			"[-]Too few arguments. Usage: %s <host> <port> <filename> <savepath>\n",
			argv[0]);
		
		return -1;
	}
	else if (argc > 5) {
		fprintf(stderr,
			"[-]Too many arguments. Usage: %s <host> <port> <filename> <savepath>\n",
			argv[0]);

		return -1;
	}
	
	char* host = argv[1];
	char* port = argv[2];
	char* filename = argv[3];
	char* savepath = argv[4];

	FileClient client = FileClient(host, port, filename, savepath);

	return 0;
}