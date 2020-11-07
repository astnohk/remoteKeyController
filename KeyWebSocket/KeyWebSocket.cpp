// KeyWebSocket.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "config.h"
#include "http.h"
#include "websocket.h"

#include <Windows.h>
#include <chrono>
#include <iostream>
#include <thread>



static BYTE key_playStop = VK_F8;
static BYTE key_backward = VK_F9;
static BYTE key_forward = VK_F10;
static BYTE key_windowOpenMinimize = VK_F11;


void
send_Ctrl_Function(BYTE key)
{
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event(key, 0, 0, 0);
    keybd_event(key, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
}

void
on_message(websocketpp_server* server, websocketpp::connection_hdl hdl, websocketpp_server::message_ptr msg)
{
    std::cout << "on_message called with hdl: " << hdl.lock().get()
        << " and message: " << msg->get_payload() << std::endl;

    if (msg->get_payload() == "playStop") {
        send_Ctrl_Function(key_playStop);
        return;
    }
    else if (msg->get_payload() == "backward") {
        send_Ctrl_Function(key_backward);
        return;
    }
    else if (msg->get_payload() == "forward") {
        send_Ctrl_Function(key_forward);
        return;
    }
    else if (msg->get_payload() == "windowOpenMinimize") {
        send_Ctrl_Function(key_windowOpenMinimize);
        return;
    }

    try {
        server->send(hdl, msg->get_payload(), msg->get_opcode());
    }
    catch (websocketpp::exception const& e) {
        std::cout << "Echo failed because: " << "(" << e.what() << ")" << std::endl;
    }
}



int
main()
{
    // Load config
    Config config;
    config.load(L"KeyWebSocket.cfg");
    std::cout << std::endl;

    // HTTP server
    if (config.hostnames.size() == 0) {
        // Add localhost
        config.hostnames.push_back(std::string("127.0.0.1"));
    }
    std::vector<WebServer> http_listeners;
    for (size_t i = 0; i < config.hostnames.size(); ++i) {
        std::wstringstream address;
        wchar_t wbuf[256];
        size_t count;
        errno_t err = mbstowcs_s(&count, wbuf, 256, config.hostnames[i].c_str(), 255);
        if (err == 0) {
            address << U("http://") << wbuf << U(":") << config.http_port;

            http_listeners.emplace_back(address.str());
            std::wcout << "start http server on (" << address.str() << ")" << std::endl;
            http_listeners.back().open().wait();
        }
    }

    // WebSocket server
    websocketpp_server websocket_listener;

    try {
        //websocket_listener.set_access_channels(websocketpp::log::alevel::all);
        //websocket_listener.clear_access_channels(websocketpp::log::alevel::frame_payload);

        websocket_listener.init_asio();
        websocket_listener.set_message_handler(websocketpp::lib::bind(&on_message, &websocket_listener, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));

        std::cout << "start websocket server on (" << config.websocket_port << ")" << std::endl;
        websocket_listener.listen(config.websocket_port);
        websocket_listener.start_accept();

        std::thread t_websocket([&]{ websocket_listener.run(); });
        t_websocket.detach();
    }
    catch (websocketpp::exception const& e) {
        std::cout << e.what() << std::endl;
    }
    catch (std::exception const& e) {
        std::cout << "other exception: " << e.what() << std::endl;
    }

    // Waiting loop
    std::cout << "wait for requests..." << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
