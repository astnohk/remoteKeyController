#pragma once

#include <string>
#include <vector>


class Config
{
public:
	std::vector<std::string> hostnames;
	int http_port;
	int websocket_port;


	Config();

	void load(const wchar_t* path);
};