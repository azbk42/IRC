#include "include.hpp"

std::string to_uppercase(const std::string &str) 
{
    std::string upper_str = str;
    std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(), ::toupper);
    return upper_str;
}

int checkPort(char *argv)
{
	for (int i = 0; argv[i]; i++){
			if (!isdigit(argv[i])){
				std::cout << "Error: port must be a number" << std::endl;
				return -1;
			}
		}
		int Port = atoi(argv);
		if (Port < 1024 || Port > 49151){ 	// confirmer 49151, 65535 d'apres beej
			std::cout << "Error: Port must be between 1024 and 49151" << std::endl;
			return -1;
		}
	return (Port);
}