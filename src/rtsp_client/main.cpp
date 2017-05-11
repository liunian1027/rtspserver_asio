#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <dhnet/dhasio.hpp>
//#include <CompLib/RTP/CRTPPacket.h>
#include <dhnet/http/packet_helper.hpp>

using namespace std;
using namespace dhnet::asio;


class rtp_packet_helper
{
    typedef std::deque<uint8_t> data_type;
    typedef data_type::iterator data_iterator;
public:
    /// 接收数据片段
    template<typename It>
    void consume(It first, It last)
    {
        std::copy(first, last, std::back_inserter(m_data));
    }

    /// 生成完整RTP数据报文(不包括$0ll)
    template<typename It>
    size_t product(It packet)
    {
        if (m_data.size()>4)
        {
            size_t rtp_size = 256*m_data[2]+m_data[3];
            if (m_data.size() >= rtp_size + 4)
            {
                data_iterator rtp_begin(m_data.begin());
                std::advance(rtp_begin,4);
                data_iterator rtp_end(rtp_begin);
                std::advance(rtp_end,rtp_size);

                std::copy(rtp_begin, rtp_end, packet);
                m_data.erase(m_data.begin(), rtp_end );
                return rtp_size;
            }
        }
        return 0;
    }

private:
    data_type m_data;
};


class rtp_reciever
{
public:
    rtp_reciever( char const* filename = "./h264" )
    {
#ifdef __WIN32__
        int flag = O_BINARY|O_WRONLY|O_CREAT;
#else
        int flag = O_WRONLY|O_CREAT;
#endif
        m_out = ::open(filename,flag);
        if (m_out<0)
        {
            throw std::runtime_error("cannot create out file!");
        }
    }
    ~rtp_reciever()
    {
        close(m_out);
    }

    template<typename InIt>
    void parse(InIt first, InIt last)
    {
        assert(first!=last);

        std::vector<uint8_t> buf(first,last);

        assert(buf.size()>12);
        parse_payload(&buf[12], size_t(buf.size()-12));
    }

private:
    void parse_payload(uint8_t const* playload, size_t len)
    {
        static uint8_t const start_code[] = { 0, 0, 1 };

        uint8_t h1 = playload[0];
        uint8_t h2 = playload[1];

        if ( (h1&0x9C) == 0x1C )    ///< 是NAL片段
        {
            uint8_t head = (h1&0x60)|(h2&0x1F);
            if ( (h2&0xE0) == 0x80 )    ///< 首个NAL片段
            {
                ::write(m_out,start_code,sizeof(start_code));
                ::write(m_out,&head,sizeof(head));
            }
            ::write(m_out,&playload[2],len-2);
        }
        else
        {
            ::write(m_out,start_code,sizeof(start_code));
            ::write(m_out,&playload[0],len);
        }
    }

private:
    int m_out;
};



int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cout << "Usage: rtsp_client <server> <port>\n";
            std::cout << "Example:\n";
            std::cout << "  rtsp_client 10.6.5.56 8812\n";
            return 1;
        }

        io_service io_service;

        // Get a list of endpoints corresponding to the server name.
        ip::tcp::resolver resolver(io_service);
        ip::tcp::resolver::query query(ip::tcp::v4(), argv[1], argv[2]);
        ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        ip::tcp::resolver::iterator end;

        // Try each endpoint until we successfully establish a connection.
        ip::tcp::socket socket(io_service);
        error_code error = error::host_not_found;
        while (error && endpoint_iterator != end)
        {
            socket.close();
            socket.connect(*endpoint_iterator++, error);
        }
        if (error)
            throw system_error(error);


        char* requests[] = {
            "OPTIONS rtsp://server/h264 RTSP/1.0\r\n"
            "CSeq: 1\r\n"
            "\r\n",

            "DESCRIBE rtsp://server/h264 RTSP/1.0\r\n"
            "CSeq: 2\r\n"
            "Accept: application/sdp\r\n"
            "\r\n",

            "SETUP rtsp://server/h264/trackID=0 RTSP/1.0\r\n"
            "CSeq: 3\r\n"
            "Transport: RTP/AVP/TCP;unicast;interleaved=0-1\r\n"
            "\r\n",

            "PLAY rtsp://server/h264 RTSP/1.0\r\n"
            "CSeq: 4\r\n"
            "Session: 111111\r\n"
            "Range: npt=0.000-\r\n"
            "\r\n"
        };

        dhnet::http::packet_helper http_helper;

        for (size_t i=0; i<sizeof(requests)/sizeof(requests[0]); ++i)
        {
            int req_len = strlen(requests[i]);
            asio::write(socket,asio::buffer(requests[i],req_len));

            std::vector<uint8_t> reply;
            reply.reserve(2048);
            do {
                std::vector<uint8_t> buf;
                buf.resize(2048);
                size_t bytes =
                    socket.read_some(asio::buffer(&buf[0],buf.size()));
                buf.resize(bytes);

                http_helper.consume(buf.begin(),buf.end());
                reply.clear();
                http_helper.product(std::back_inserter(reply));
            } while (reply.empty());

            copy(reply.begin(),reply.end(),ostream_iterator<char>(cout));
        }

        std::cout<<"start read rtp packet..."<<std::endl;

        rtp_packet_helper rtp_helper;
        rtp_reciever reciever;
        while (1)
        {
            std::vector<uint8_t> buf;
            buf.resize(2048);
            size_t bytes =
                socket.read_some(asio::buffer(&buf[0],buf.size()));
            buf.resize(bytes);

            if (buf.empty()) break;

            rtp_helper.consume(buf.begin(),buf.end());

            vector<uint8_t> rtp_packet;
            rtp_packet.reserve(1600);
            rtp_helper.product(back_inserter(rtp_packet));
            if (!rtp_packet.empty())
            {
                //std::cout<<"one rtp packet reached!"<<std::endl;
                reciever.parse(rtp_packet.begin(),rtp_packet.end());
                std::cout<<".";
            }
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;

}
