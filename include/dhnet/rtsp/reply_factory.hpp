
#ifndef RTSP_REPLY_FACTORY_H__
#define RTSP_REPLY_FACTORY_H__

#include <boost/lexical_cast.hpp>
#include <CompLib/RTP/CRTPPacket.h>
#include "http.hpp"

namespace dhnet {
namespace rtsp {

namespace stock_string
{
    static char const rtsp_version[] = "RTSP/1.0";
    static char const cseq_name[] = "CSeq";
    static char const public_name[] = "Public";
    static char const session_name[] = "Session";
    static char const transport_name[] = "Transport";
    static char const cmd_list[] =
        "DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE";
}

inline
reply make_options_reply(int cseq)
{
    using namespace stock_string;

    reply rep(rtsp_version,status_code::ok);

    rep.push_header(header(cseq_name, boost::lexical_cast<std::string>(cseq)));

    return rep;
}

inline
reply make_describe_reply(int cseq)
{
    using namespace stock_string;

    std::string sdp =
        "v=0\r\n"
        "o=StreamingServer 3440750359 111111 IN IP4 10.6.5.56\r\n"
        "s=h264.mp4\r\n"
        //"u=http:///\r\n"
        //"e=admin@\r\n"
        "c=IN IP4 0.0.0.0\r\n"
        "b=AS:2097172\r\n"
        "t=0 0\r\n"
        "a=control:*\r\n"
        "a=isma-compliance:2,2.0,2\r\n"
        "a=range:npt=0-70.00000\r\n"
        "m=video 0 RTP/AVP 96\r\n"
        "a=3GPP-Adaptation-Support:1\r\n"
        "a=rtpmap:96 H264/90000\r\n"
        "a=control:trackID=0\r\n"
        "a=framesize:96 192-242\r\n"
        "a=fmtp:96 packetization-mode=1;"
            "profile-level-id=4D400C;"
            "sprop-parameter-sets=J01ADKkYYELxCA==,KM4JiA==\r\n"
        "a=mpeg4-esid:201\r\n"
        ;

    reply rep(rtsp_version, mime_type::sdp, sdp.begin(), sdp.end());
    rep.push_header(header(cseq_name, boost::lexical_cast<std::string>(cseq)));

    return rep;
}

inline
reply make_setup_reply(int cseq)
{
    using namespace stock_string;

    reply rep(rtsp_version, status_code::ok );

    rep.push_header(header("Server","DH Server/1.1"));
    rep.push_header(header("User-Agent","DH RTSP Server"));
    rep.push_header(header(cseq_name, boost::lexical_cast<std::string>(cseq)));
    rep.push_header(header(session_name, "111111"));
    rep.push_header(header(transport_name, "RTP/AVP/TCP;unicast;interleaved=0-1;ssrc=0c00cdee;mode=PLAY;ssrc=0000001E"));

    return rep;
}

inline
reply make_play_reply(int cseq)
{
    using namespace stock_string;

    reply rep(rtsp_version, status_code::ok );

    rep.push_header(header(cseq_name, boost::lexical_cast<std::string>(cseq)));
    rep.push_header(header("Range","npt:0-"));
    rep.push_header(header(session_name, "111111"));

    return rep;
}

inline
reply make_teardown_reply(int cseq)
{
    using namespace stock_string;

    reply rep(rtsp_version, status_code::ok );

    rep.push_header(header(cseq_name, boost::lexical_cast<std::string>(cseq)));

    return rep;
}

} // namespace rtsp
} // namespace dhnet

#endif // RTSP_REPLY_FACTORY_H__
