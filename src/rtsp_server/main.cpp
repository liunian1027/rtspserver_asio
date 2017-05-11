#include <iostream>
#include "dhnet/tcp/async_server.hpp"
#include "dhnet/rtsp/parser.hpp"

int main(int argc, char* argv[])
{
#ifdef _DEBUG
    void dbg_open();
    dbg_open();
#endif

    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: async_tcp_echo_server <port> <webroot>\n";
            return 1;
        }
        int port = std::atoi(argv[1]);
        dhnet::rtsp::parser request_handler(argv[2]);
        dhnet::tcp::async_server<dhnet::rtsp::parser> s(request_handler, port);

        s.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

#ifdef _DEBUG
    void dbg_close();
    dbg_close();
#endif

    return 0;
}



