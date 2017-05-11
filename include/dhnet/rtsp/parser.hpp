
#ifndef RTSP_PARSER_H__
#define RTSP_PARSER_H__

#ifdef __WIN32__
#include <windows.h>
#endif
#include <unistd.h>

#include "http.hpp"
#include "reply_factory.hpp"

#include "rtp_producer.hpp"


namespace dhnet {
namespace rtsp {


/// RTSP命令
namespace command
{
    static char const options[] = "OPTIONS";
    static char const describe[]= "DESCRIBE";
    static char const setup[]   = "SETUP";
    static char const play[]    = "PLAY";
    static char const teardown[]= "TEARDOWN";
}


/// RTSP命令解析类
class parser
{
public:
    parser(char const* webroot)
        : m_webroot(webroot)
        , m_need_parse(false)
        , m_is_playing(false)
    {
        if ( m_webroot[m_webroot.size()] =='/')
        {
            m_webroot.resize(m_webroot.size()-1);
        }
    }

    /// 接收客户端数据
    /// \param [in] first 数据区间输入迭代器开始
    /// \param [in] last 数据区间输入迭代器结束
	template<typename InIt>
	void handle_request(InIt first, InIt last)
	{
	    m_packet_helper.consume(first,last);
        std::vector<uint8_t> req_packet;
	    if ( m_packet_helper.product(std::back_inserter(req_packet)) > 0 )
	    {
	        // out packet
            std::copy(req_packet.begin(), req_packet.end(),
                std::ostream_iterator<uint8_t>(std::cerr) );
            std::cerr<<std::endl;

            request req(req_packet.begin(), req_packet.end());
            m_request.swap(req);
            m_need_parse = true;
	    }
	}

    /// 提供服务器端响应
    /// \param [in] rep_packet 输出响应的输出迭代器
    /// \return 返回输出的字节数
	template<typename OutIt>
	size_t fetch_reply(OutIt rep_packet)
	{
	    if ( m_need_parse )
	    {
            std::string const& cseq_str = find_value(m_request.headers(),"CSeq");
            assert(!cseq_str.empty());
            int cseq = atoi(cseq_str.c_str());

            std::string const& method = m_request.method();
            if ( method == command::options )
            {
                on_options(cseq );
            }
            else if ( method == command::describe )
            {
                on_describe(cseq );
            }
            else if ( method == command::setup )
            {
                on_setup(cseq );
            }
            else if ( method == command::play )
            {
                m_is_playing = true;
                on_play(cseq);
            }
            else if ( method == command::teardown )
            {
                m_is_playing = false;
                on_teardown(cseq);
            }
            m_need_parse = false;
            return to_packet(m_reply,rep_packet);
	    }
	    else if ( m_is_playing )
	    {
            // rtp reply
            return fetch_rtp(rep_packet);
	    }

        return 0;
	}

private:
    void on_options(int cseq)
    {
        make_options_reply(cseq).swap(m_reply);
    }

    void on_describe(int cseq)
    {
        make_describe_reply(cseq).swap(m_reply);
    }

    void on_setup(int cseq)
    {
        make_setup_reply(cseq).swap(m_reply);
    }

    void on_play(int cseq)
    {
        std::string filename = m_webroot+"/h264";
        {
            rtp_producer_ptr(new rtp_producer(filename.c_str()))
                .swap(m_producer_ptr);
        }

        make_play_reply(cseq).swap(m_reply);
    }

    void on_teardown(int cseq)
    {
        make_teardown_reply(cseq).swap(m_reply);
    }

	template<typename It>
	size_t fetch_rtp(It rep_packet)
    {
        if (!m_producer_ptr)
        {
            return 0;
        }
#ifdef _DEBUG
        std::cerr<<".";//<<std::endl;
#endif

        size_t size = m_producer_ptr->product(rep_packet);

#ifdef _DEBUG
        std::cerr<<size;//<<std::endl;
#endif

        usleep(5000);

        return size;
    }

private:
    std::string   m_webroot;
    packet_helper m_packet_helper;

    request m_request;
    reply   m_reply;

    bool m_need_parse;
    bool m_is_playing;

    typedef boost::shared_ptr<rtp_producer> rtp_producer_ptr;
    rtp_producer_ptr m_producer_ptr;
};

} // namespace http
} // namespace dhnet

#endif // RTSP_PARSER_H__

