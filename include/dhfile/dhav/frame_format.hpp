#ifndef DHFILE_FRAME_FORMAT_HPP__
#define DHFILE_FRAME_FORMAT_HPP__



namespace dhfile {
namespace dhav {


enum { DHAV_tag = 0x56414844 }; // "DHAV"
enum { dhav_tag = 0x76616864 }; // "dhav"

struct frame_kind
{
    enum type {
        ivideo = 0xFD,
        pvideo = 0xFC,
        bvideo = 0xFE,
        image = 0xFB,
        audio = 0xF0,
        assistant = 0xF1,
        custom = 0x80
    };
};

/*
struct data_time_type
{
    uint32_t second :6;
    uint32_t minute :6;
    uint32_t hour   :5;
    uint32_t day    :5;
    uint32_t month  :4;
    uint32_t year   :6;
};
*/

struct frame_header
{
    uint32_t  tag;
    uint8_t   kind;
    uint8_t   sub_kind;
    uint8_t   channel_id;
    uint8_t   subframe_id;
    uint32_t  frame_id;
    uint32_t  frame_size;
    uint32_t  data_time;       //in seconds
    uint16_t  time_stamp; //in miliseconds
    uint8_t   extension_size;
    uint8_t   checksum;
};


struct frame_tail
{
    uint32_t tag;
    uint32_t frame_size;
};


size_t get_frame_data_offset(frame_header const& header)
{
    return sizeof(frame_header) + header.extension_size;
}

size_t get_frame_data_size(frame_header const& header)
{
    return header.frame_size -
        ( get_frame_data_offset(header) + sizeof(frame_tail) );
}


/// Ö¡Êý¾Ý
struct frame_raw
{
public:
    typedef frame_header          header_t;
    typedef std::vector<uint8_t> extension_t;
    typedef std::vector<uint8_t> data_t;
    typedef frame_tail            tail_t;

    header_t    header;
    extension_t extension;
    data_t      data;
    tail_t      tail;

    frame_raw()
    {
    }

    frame_raw(frame_header const& h)
    {
        extension.resize(h.extension_size);
        data.resize(get_frame_data_size(h));
    }

    void swap(frame_raw& rhs)
    {
        std::swap(header, rhs.header);
        std::swap(extension, rhs.extension);
        std::swap(data, rhs.data);
        std::swap(tail, rhs.tail);
    }
};


} //namespace h264
} //namespace dhfile

#endif //DHFILE_FRAME_FORMAT_HPP__
