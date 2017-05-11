
#ifndef RTSP_NAL_FRAGMENT_H__
#define RTSP_NAL_FRAGMENT_H__


#include <dhfile/dhav/frame_stream.hpp>
#include <dhfile/dhav/nal_stream.hpp>


namespace dhnet {
namespace rtsp {



/// NAL 片段类型
struct nal_fragment_kind
{
    typedef enum {
        whole,  ///< 完整NAL
        head,   ///< 首个NAL片段
        mid,    ///< 中间NAL片段
        tail    ///< 末尾NAL片段
    } type;
};

/// NAL 片段
struct nal_fragment
{
    typedef dhfile::dhav::frame_raw::data_t::const_iterator raw_iterator;
    typedef nal_fragment_kind::type kind_t;

    kind_t kind;            ///< 片段类型
    uint8_t      head;      ///< NAL head
    raw_iterator begin;     ///< 数据区间开始
    raw_iterator end;       ///< 数据区间结束

    nal_fragment(kind_t k, uint8_t h, raw_iterator b, raw_iterator e)
        : kind(k)
        , head(h)
        , begin(b)
        , end(e)
    {
    }
};

/// NAL片段容器
typedef std::vector<nal_fragment>   nal_fragment_vector;


/// 将一帧数据分解成nal片段，供RTP封包
/// \param frame_raw [in] 完整NAL数据
/// \param fragments [out] NAL片段容器
void dispart_nal(
        dhfile::dhav::frame_raw const& frame,
        nal_fragment_vector& fragments
)
{
    typedef dhfile::dhav::frame_stream                      frame_stream_t;
    typedef dhfile::dhav::frame_raw                         frame_raw_t;
    typedef frame_raw_t::data_t                             raw_t;
    typedef raw_t::const_iterator                           raw_iterator;
    typedef dhfile::dhav::nal_stream_wrapper<raw_iterator>  nal_stream_t;
    typedef nal_stream_t::raw_range_t                       raw_range_t;

    nal_stream_t nal_stream(frame.data.begin(), frame.data.end());
    nal_fragment_vector nal_fragments;

    raw_range_t nal = nal_stream.read_nal();
    while ( nal.first != nal.second )
    {
        enum { playload_max_size = 1400 };

        /// depart one nal to many fragmets
        uint8_t head = *nal.first;
        ++nal.first;
        if ( std::distance(nal.first,nal.second) <= playload_max_size-1 )
        {
            /// 处理完整NAL片段
            nal_fragments.push_back(
                nal_fragment(
                    nal_fragment_kind::whole, head, nal.first, nal.second));
        }
        else
        {
            /// 处理首个NAL片段
            raw_iterator data_begin = nal.first;
            std::advance(nal.first, playload_max_size-2);
            nal_fragments.push_back(
                nal_fragment(
                    nal_fragment_kind::head, head, data_begin, nal.first));

            /// 处理中间NAL片段
            while (std::distance(nal.first, nal.second) > playload_max_size-2)
            {
                data_begin = nal.first;
                std::advance(nal.first, playload_max_size-2);
                nal_fragments.push_back(
                    nal_fragment(
                        nal_fragment_kind::mid, head, data_begin, nal.first));
            }

            /// 处理末尾NAL片段
            nal_fragments.push_back(
                nal_fragment(
                    nal_fragment_kind::tail, head, nal.first, nal.second));
        }

        /// 读下一个NAL数据
        nal = nal_stream.read_nal();
    }

    /// 返回NAL片段容器
    std::swap(nal_fragments,fragments);
}









} // namespace rtsp
} // namespace dhnet

#endif // RTSP_NAL_FRAGMENT_H__
