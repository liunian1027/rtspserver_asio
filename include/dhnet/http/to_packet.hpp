#ifndef HTTP_TO_PACKET_HPP__
#define HTTP_TO_PACKET_HPP__


#include "reply.hpp"
#include "request.hpp"


namespace dhnet {
namespace http {


/// 格式化响应对象
template<typename OutIt>
inline
size_t to_packet(reply const& rep, OutIt buf)
{
    assert(!rep.empty());

    size_t buffer_size = 0;

    /// 状态行
    std::string const& protocol = rep.protocol();
    buf = std::copy(protocol.begin(), protocol.end(), buf);
    buffer_size += protocol.size();
    *buf++ = ' ';
    ++buffer_size;
    std::string const& status = to_string(rep.status());
    buf = std::copy(status.begin(),status.end(),buf);
    buffer_size += status.size();
    *buf++ = '\r';
    *buf++ = '\n';
    buffer_size += 2;

    /// 首部
    std::vector<header> const& headers = rep.headers();
    std::vector<header>::const_iterator it;
    for (it=headers.begin(); it!=headers.end(); ++it)
    {
        std::string const& header = to_string(*it);
        buf = std::copy(header.begin(),header.end(),buf);
        buffer_size += header.size();
    }

    /// 空行
    *buf++ = '\r';
    *buf++ = '\n';
    buffer_size += 2;

    /// 主体
    buf = std::copy(rep.body().begin(),rep.body().end(),buf);
    buffer_size += rep.body().size();

    return buffer_size;
}

/// 格式化请求对象
template<typename OutIt>
inline
size_t to_packet(request const& req, OutIt buf)
{
    assert(!req.empty());

    size_t buffer_size = 0;

    /// 请求行
    std::string const& method = req.method();
    buf = std::copy(method.begin(), method.end(), buf);
    *buf++ = ' ';
    ++buffer_size;
    std::string const& url = req.url();
    buf = std::copy(url.begin(),url.end(),buf);
    buffer_size += url.size();
    std::string const& protocol = req.protocol();
    buf = std::copy(protocol.begin(), protocol.end(), buf);
    buffer_size += protocol.size();
    *buf++ = '\r';
    *buf++ = '\n';
    buffer_size += 2;

    /// 首部
    std::vector<header> const& headers = req.headers();
    std::vector<header>::const_iterator it;
    for (it=headers.begin(); it!=headers.end(); ++it)
    {
        std::string const& header = to_string(*it);
        buf = std::copy(header.begin(),header.end(),buf);
        buffer_size += header.size();
    }

    /// 空行
    *buf++ = '\r';
    *buf++ = '\n';
    buffer_size += 2;

    /// 主体
    buf = std::copy(req.body().begin(),req.body().end(),buf);
    buffer_size += req.body().size();

    return buffer_size;
}

} // namespace http
} // namespace dhnet

#endif // HTTP_TO_PACKET_HPP__
