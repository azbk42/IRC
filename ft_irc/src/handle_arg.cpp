#include "include.hpp"

int checkPort(char *argv)
{
	for (int i = 0; argv[i]; i++){
		if (!isdigit(argv[i])){
			std::cout << "Error: port must be a number" << std::endl;
			return -1;
		}
	}
	int Port = atoi(argv);
	if (Port < 1024 || Port > 49151){
		std::cout << "Error: Port must be between 1024 and 49151" << std::endl;
		return -1;
	}
	return (Port);
}