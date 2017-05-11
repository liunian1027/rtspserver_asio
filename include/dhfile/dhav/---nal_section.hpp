#ifndef RTSP_RTP_PRODUCER_H__
#define RTSP_RTP_PRODUCER_H__


#include <boost/shared_ptr.hpp>
#include <frame_stream.hpp>
#include <nal_stream.hpp>


namespace dhfile {
namespace dhav {


struct nal_section_kind
{
    typedef enum {
        null = 0,
        begin,
        middle,
        end
    } type;
};


class nal_section_stream
{
public:
    typedef frame_raw::data_t                   raw_data_t;
    typedef raw_data_t::const_iterator          nal_iterator;
    typedef nal_stream_wrapper<nal_iterator>    nal_stream_t;
    typedef boost::shared_ptr<nal_stream_t>     nal_stream_ptr;
    typedef nal_stream_t::raw_range_t           raw_range_t;

public:
    nal_section_stream(frame_stream& fs)
        : m_frame_stream(fs)

    {
        assert(m_frame_stream.is_open());
        //read_one_frame();
    }

    nal_section_kind::type read_section(raw_range_t& section )
    {
        if ( !m_nal_stream )
        {
            read_one_frame();
        }

        section = m_nal_stream->read_nal();
    }

private:
    void load_next_frame()
    {
        assert(m_frame_stream.is_open());

        if (m_frame_stream)
        {
            nal_stream_ptr(nal_stream_wrapper(m_frame_raw)).swap(m_nal_stream);
            m_nal_remain = m_nal_stream->read_nal();
        }
    }

    void read_next_section()

    void read_nal_section()
    {
        if ( m_frame_stream && )
    }

private:
    frame_stream&   m_frame_stream;
    frame_raw       m_frame_raw;

    nal_stream_ptr  m_nal_stream;
    raw_range_t     m_nal_remain;

    nal_section_kind::type m_section_kind;

};


} // namespace dhav
} // namespace dhfile

#endif // RTSP_RTP_PRODUCER_H__
