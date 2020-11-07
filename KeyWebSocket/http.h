#pragma once

#include <cpprest/filestream.h>
#include <cpprest/http_listener.h>



class WebServer
{
private:
	web::http::experimental::listener::http_listener m_listener;
	std::map<utility::string_t, std::tuple<utility::string_t, utility::string_t> > m_htmlcontentmap;

	void handle_get(web::http::http_request message);
	static void handle_error(pplx::task<void>& t);

public:
	WebServer(utility::string_t url);

	pplx::task<void> open();
	pplx::task<void> close();
};