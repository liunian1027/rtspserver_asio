#ifndef DHFILE_FRAME_STREAM_HPP_
#define DHFILE_FRAME_STREAM_HPP_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>
#include "frame_format.hpp"

namespace dhfile {
namespace dhav {

/// DHAV文件的帧流类
class frame_stream
{
    typedef frame_stream this_class;

public:
    frame_stream()
        : m_file(-1)
        , m_file_size(0)
        , m_is_open(false)
    {
    }

    ~frame_stream()
    {
        close();
    }

    void open(char const* filename)
    {
        close();

        m_file_size = dhlib::get_file_size(filename);
#ifdef __WIN32__
        int flag = O_BINARY|O_RDONLY;
#else
        int flag = O_RDONLY;
#endif
        m_file = ::open(filename, flag);
        if ( m_file>=0 )
        {
            m_is_open = true;
            move_first();
        }
    }

    void close()
    {
        if (m_file>=0)
        {
            ::close(m_file);
            m_file_size = 0;
        }
    }

    bool is_open() const
    {
        return m_is_open;
    }

    this_class& read_frame(frame_raw& frame)
    {
        assert( is_open() && !eof() );

        frame_raw raw(m_header);
        load_frame_extension(raw.extension);
        load_frame_raw(raw.data);
        load_frame_tail(raw.tail);
        frame.swap(raw);

        move_next();

        return *this;
    }

    operator bool() const
    {
        return is_open() && !eof();
    }

private:

    bool eof() const
    {
        return m_header_pos >= m_file_size;
    }

    void move_first()
    {
        if ( is_open() )
        {
            m_header_pos = 0;
            load_frame_header(m_header);
        }
    }

    void move_next()
    {
        m_header_pos += m_header.frame_size;
        if ( is_open() && !eof() )
        {
            load_frame_header(m_header);
        }
    }

    void load_frame_header( frame_raw::header_t& header )
    {
        assert( is_open() && !eof() );

        lseek(m_file, m_header_pos, SEEK_SET);
        read(m_file, &header, sizeof(header));
        if (header.tag != DHAV_tag )
        {
            throw std::runtime_error("dhav file error: unexcepted tag");
        }
    }

    void load_frame_extension( frame_raw::extension_t& ext )
    {
        // not implement
    }

    void load_frame_raw( frame_raw::data_t& data )
    {
        assert( is_open() && !eof() );

        size_t data_pos = m_header_pos + get_frame_data_offset(m_header);
        lseek(m_file, data_pos, SEEK_SET);
        size_t size = read( m_file, &data[0], data.size() );
        if ( size != data.size() )
        {
            throw std::runtime_error("dhav file error: frame data length error");
        }
    }

    void load_frame_tail( frame_raw::tail_t& tail )
    {
        // not implement
    }

private:
    int    m_file;
    size_t m_file_size;
    bool   m_is_open;

    size_t m_header_pos;

    frame_raw::header_t m_header;
};

} //namespace h264
} //namespace dhfile

#endif //DHFILE_FRAME_STREAM_HPP_
