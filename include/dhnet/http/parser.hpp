
#ifndef HTTP_PARSER_H__
#define HTTP_PARSER_H__

#include "packet_helper.hpp"
#include "request.hpp"
#include "reply.hpp"
#include "to_packet.hpp"


namespace dhnet {
namespace http {


class parser
{
public:
    parser(char const* webroot)
        : m_webroot(webroot)
    {
        if ( m_webroot[m_webroot.size()] =='/')
        {
            m_webroot.resize(m_webroot.size()-1);
        }
    }

    /// 接收数据包
	template<typename InIt>
	void handle_request(InIt first, InIt last)
	{
	    m_packet_helper.consume(first,last);
	}

    /// 取出响应包
	template<typename OutIt>
	size_t fetch_reply(OutIt rep_packet)
	{
        std::vector<uint8_t> req_packet;

	    if ( m_packet_helper.product(std::back_inserter(req_packet)) > 0 )
	    {
#if 1
	        // out packet
            std::copy(req_packet.begin(), req_packet.end(),
                std::ostream_iterator<uint8_t>(std::cerr) );
            std::cerr<<std::endl;
#endif

            request req(req_packet.begin(), req_packet.end());
            if (req.method()=="GET")
            {
                reply rep(req.protocol(), m_webroot+req.url());
                return to_packet(rep, rep_packet);
            }
            else if (req.method()=="POST")
            {
                reply rep(req.protocol(), status_code::ok);
                return to_packet(rep, rep_packet);
            }
	    }
        return 0;
	}

private:
    std::string   m_webroot;
    packet_helper m_packet_helper;
};

} // namespace http
} // namespace dhnet

#endif // HTTP_PARSER_H__
