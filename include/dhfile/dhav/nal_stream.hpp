#ifndef DHFILE_NAL_STREAM_HPP_
#define DHFILE_NAL_STREAM_HPP_

#include <iterator>
#include <utility>
#include "frame_format.hpp"

namespace dhfile {
namespace dhav {


static uint8_t const nal_start_code[] = { 0, 0, 1 };
static size_t const nal_start_code_size = sizeof(nal_start_code);

template<typename FwdIt>
inline
FwdIt find_nal_start_code(FwdIt first, FwdIt last)
{
    return std::search(first, last,
        &nal_start_code[0], &nal_start_code[nal_start_code_size]);
}


/// 帧数据的NAL流包装类
template<typename FwdIt>
class nal_stream_wrapper
{
public:
    typedef FwdIt raw_iterator;
    typedef std::pair<raw_iterator,raw_iterator> raw_range_t;

public:
    nal_stream_wrapper( raw_iterator first, raw_iterator last )
        : m_remain_range(first,last)
    {
        m_remain_range.first = find_nal_start_code(first,last);
    }

    raw_range_t read_nal()
    {
        if (m_remain_range.first != m_remain_range.second)
        {
            raw_iterator nal_begin = m_remain_range.first;
            std::advance(nal_begin, nal_start_code_size);
            m_remain_range.first =
                find_nal_start_code(nal_begin,m_remain_range.second);
            return make_pair(nal_begin,m_remain_range.first);
        }
        else
        {
            return m_remain_range;  /// empty range
        }
    }

    bool empty() const
    {
        return m_remain_range.first==m_remain_range.second;
    }

private:
    raw_range_t m_remain_range;
};


} //namespace h264
} //namespace dhfile

#endif //DHFILE_NAL_STREAM_HPP_
