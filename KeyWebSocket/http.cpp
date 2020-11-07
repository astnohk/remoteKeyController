
#include "http.h"




WebServer::WebServer(utility::string_t url)
    : m_listener(url)
{
    // Initialize listener
    m_listener.support(web::http::methods::GET, std::bind(&WebServer::handle_get, this, std::placeholders::_1));

    // Initialize contentmap
    m_htmlcontentmap[U("/")] = std::make_tuple(U("www/index.html"), U("text/html"));
    m_htmlcontentmap[U("/main.js")] = std::make_tuple(U("www/main.js"), U("text/javascript"));
}

pplx::task<void>
WebServer::open()
{
    return m_listener.open().then([](pplx::task<void> t) { handle_error(t); });
}

pplx::task<void>
WebServer::close()
{
    return m_listener.close().then([](pplx::task<void> t) { handle_error(t); });
}

void
WebServer::handle_get(web::http::http_request message)
{
    auto path = message.relative_uri().path();
    auto content_data = m_htmlcontentmap.find(path);
    if (content_data == m_htmlcontentmap.end()) {
        message.reply(web::http::status_codes::NotFound, U("Not found")).then([](pplx::task<void> t) { handle_error(t); });
        return;
    }

    auto file_name = std::get<0>(content_data->second);
    auto content_type = std::get<1>(content_data->second);
    concurrency::streams::fstream::open_istream(file_name, std::ios::in)
        .then([=](concurrency::streams::istream is) {
                message.reply(web::http::status_codes::OK, is, content_type).then([](pplx::task<void> t) { handle_error(t); });
            })
        .then([=](pplx::task<void> t) {
            try {
                t.get();
            }
            catch (...) {
                message.reply(web::http::status_codes::InternalError).then([](pplx::task<void> t) { handle_error(t); });
            }
        });
}

void
WebServer::handle_error(pplx::task<void>& t)
{
    try {
        t.get();
    }
    catch (...) {
        // Ignore the error
    }
}

