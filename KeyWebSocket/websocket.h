#pragma once

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>


using websocketpp_server = websocketpp::server<websocketpp::config::asio>;


void on_message(websocketpp_server* server, websocketpp::connection_hdl hdl, websocketpp_server::message_ptr msg);