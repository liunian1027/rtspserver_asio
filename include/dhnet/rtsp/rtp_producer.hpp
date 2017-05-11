
#ifndef RTSP_RTP_PRODUCER_H__
#define RTSP_RTP_PRODUCER_H__

#include "nal_fragment.hpp"

#ifdef _DEBUG
extern FILE* dbg_fp;
#endif

namespace dhnet {
namespace rtsp {


/// 由NAL片段生成 RTP Payload
template<typename OutIt>
inline
void to_rtp_payload(
    nal_fragment const& fragment,
    OutIt buf
)
{
    switch (fragment.kind)
    {
    case nal_fragment_kind::whole:
        *buf++ = fragment.head;
        break;
    case nal_fragment_kind::head:
        *buf++ = 0x1C | ( fragment.head & 0x60 );
        *buf++ = 0x80 | ( fragment.head & 0x1F );
        break;
    case nal_fragment_kind::mid:
        *buf++ = 0x1C | ( fragment.head & 0x60 );
        *buf++ = ( fragment.head & 0x1F );
        break;
    case nal_fragment_kind::tail:
        *buf++ = 0x1C | ( fragment.head & 0x60 );
        *buf++ = 0x40 | ( fragment.head & 0x1F );
        break;
    default:
        return;
    }
    std::copy(fragment.begin,fragment.end,buf);
}


/// 由NAL片段生成完整RTP报文(包括TCP头$0ll)
template<typename OutIt>
inline
size_t to_rtp_packet(
    size_t seq,
    size_t frame_id,
    bool last_mark,
    nal_fragment const& fragment,
    OutIt buf
)
{
    CompLib::CRTPPacket rtp;

    rtp.SetVersion(2);
    rtp.SetSequenceNumber(seq);
    rtp.SetPayloadType(CompLib::DynamicVideo);
    rtp.SetTimestamp(frame_id*3600);
    rtp.SetMarker(last_mark);
    rtp.SetSyncSource(0x1e);

#ifdef _DEBUG
    if (last_mark) std::cerr<<'M';
#endif

    std::vector<uint8_t> payload;
    payload.reserve(1400);
    to_rtp_payload(fragment,std::back_inserter(payload));
    char const* data = reinterpret_cast<char const*>(&payload[0]);
    rtp.AddPayload( data, payload.size() );

    char const* packet = rtp.GetStream();
    int len = rtp.GetStreamLen();


    *buf++ = '$';
    *buf++ = 0;
    *buf++ = ((uint16_t)len&0xFF00)>>8;
    *buf++ = (len&0xFF);

    std::copy(&packet[0], &packet[len], buf);

#ifdef _DEBUG
    for (int i=0; i<len; ++i)
    {
        fprintf(dbg_fp,"%02X ", (uint8_t)packet[i]);
    }
    fprintf(dbg_fp, "\n");
#endif

    return len+4;
}


/// RTP 数据包生成类
class rtp_producer
{
public:
    /// 由码流文件构造
    rtp_producer(char const* filename)
        : m_seq(0)
    {
        std::cerr<<filename<<std::endl;

        m_frame_stream.open(filename);
        m_frame_stream.read_frame(m_frame_raw);
        dispart_nal(m_frame_raw, m_fragments);

        m_frame_id = 0;
        m_section_id = 0;
    }

    /// 取一个RTP包
    template<typename OutIt>
    size_t product(OutIt buf)
    {
        /// 取下一帧，并跳过空帧
        while ( m_section_id >= m_fragments.size() )
        {
#ifdef _DEBUG
            std::cerr<<":"<<m_frame_id<<std::endl;
#endif
            if (m_frame_stream)
            {
                load_next_frame();
                m_section_id = 0;
            }
            else
            {
                return 0;
            }
        }

        bool last_mark = ( m_section_id == m_fragments.size()-1 );
        nal_fragment const& fragment = m_fragments[m_section_id];

        ++m_section_id;

        return to_rtp_packet(m_seq++, m_frame_id, last_mark, fragment, buf);
    }

private:
    void load_next_frame()
    {
        m_frame_stream.read_frame(m_frame_raw);
        dispart_nal(m_frame_raw, m_fragments);
        ++m_frame_id;
    }

private:
    dhfile::dhav::frame_stream  m_frame_stream; ///< 帧流
    dhfile::dhav::frame_raw     m_frame_raw;    ///< 当前帧数据
    nal_fragment_vector         m_fragments;    ///< 当前NAL片段容器

    size_t m_frame_id;
    size_t m_section_id;
    size_t m_seq;
};


} // namespace rtsp
} // namespace dhnet

#endif // RTSP_RTP_PRODUCER_H__
