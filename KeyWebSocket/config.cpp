#include "config.h"

#include <fstream>
#include <iostream>


Config::Config()
	: hostnames()
	, http_port(8080)
	, websocket_port(51234)
{
}


void
Config::load(const wchar_t* path)
{
	std::ifstream ifs(path);

	if (ifs.is_open()) {
		std::string tmp;
		while (std::getline(ifs, tmp)) {
			size_t n = tmp.find_first_of(" \t");
			size_t s = tmp.find_first_not_of(" \t", n);
			size_t e = tmp.find_first_of(" \t", s);
			// Check
			if (tmp.compare(0, n, "hostname") == 0) {
				hostnames.push_back(tmp.substr(s, e - s));
				std::cout << "add hostname '" << hostnames.back() << "'" << std::endl;
			}
			else if (tmp.compare(0, n, "http_port") == 0) {
				http_port = std::stoi(tmp.substr(s, e - s));
				std::cout << "set http_port = " << http_port << std::endl;
			}
			else if (tmp.compare(0, n, "websocket_port") == 0) {
				websocket_port = std::stoi(tmp.substr(s, e - s));
				std::cout << "set websocket_port = " << websocket_port << std::endl;
			}
		}
	}
}
