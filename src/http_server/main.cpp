//
// async_tcp_echo_server.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2008 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <iostream>
#include "dhnet/tcp/async_server.hpp"
#include "dhnet/http/parser.hpp"
/*
class my_request_handler
{
public:
	my_request_handler()
		: reply_count_(0)
	{

	}

	template<typename It>
	void handle_request(It first, It last)
	{
		reply_count_= (first==last) ? 0 : std::atoi(reinterpret_cast<char const*>(&(*first)));
	}

	template<typename It>
	size_t fetch_reply(It reply)
	{
		if (reply_count_>0)
		{
			*reply++ = '0'+reply_count_;
			--reply_count_;
			return 1;
		}
		else
		{
			return 0;
		}
	}

private:
	int reply_count_;
};

class http_handler
{
public:
	template<typename It>
	void handle_request(It first, It last)
	{
	    m_packet_helper.consume(first,last);
	}

	template<typename It>
	size_t fetch_reply(It rep_packet)
	{
        dhnet::http::packet_type req_packet;

	    if ( m_packet_helper.product(std::back_inserter(req_packet)) > 0 )
	    {
	        // out packet
            std::copy(req_packet.begin(), req_packet.end(),
                std::ostream_iterator<uint8_t>(std::cerr) );
            std::cerr<<std::endl;

            dhnet::http::request req(req_packet.begin(), req_packet.end());
            if (req.method()=="GET")
            {
                std::string webroot("F:/tmp/web");
                std::string filename = webroot+req.url();
                dhnet::http::reply   rep(filename);
                return dhnet::http::to_packet(rep, rep_packet);
            }
	    }
        return 0;
	}

private:
    dhnet::http::packet_helper m_packet_helper;
};
*/
int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: async_tcp_echo_server <port> <webroot>\n";
            return 1;
        }
        int port = std::atoi(argv[1]);
        dhnet::http::parser request_handler(argv[2]);
        dhnet::tcp::async_server<dhnet::http::parser> s(request_handler, port);

        s.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}

