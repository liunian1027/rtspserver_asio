
#ifndef HTTP_REQUEST_H__
#define HTTP_REQUEST_H__

#include <iterator>
#include <algorithm>
#include <dhlib/helper.hpp>
#include "header.hpp"

namespace dhnet {
namespace http {

class request
{
public:
    /// 构造空请求
    request()
    {
    }

    /// 从请求报文构造
    template<typename InIt>
    request(InIt first, InIt last)
    {
        /// read method
        InIt method_end = std::find(first,last,' ');
        std::copy(first,method_end,std::back_inserter(m_method));

        /// read url
        InIt url_begin = method_end+1;
        InIt url_end = std::find(url_begin, last, ' ');
        decode_url(url_begin, url_end, std::back_inserter(m_url));

        /// read version
        InIt version_begin = url_end+1;
        InIt version_end = std::find(version_begin, last, '\r');
        std::copy(version_begin, version_end, std::back_inserter(m_protocol));

        /// parser headers
        InIt headers_begin = version_end;
        std::advance(headers_begin,2);
        parse_headers(headers_begin,last,std::back_inserter(m_headers));

        /// read body
        static char const sep[] = "\r\n\r\n";
        InIt body_begin = std::search(first,last,&sep[0],&sep[4]);
        std::advance(body_begin,4);
        std::copy(body_begin,last,std::back_inserter(m_body));
    }

    std::string const& method() const { return m_method; }
    std::string const& url() const { return m_url; }
    std::string const& protocol() const { return m_protocol; }
    headers_type const& headers() const { return m_headers; }
    std::vector<uint8_t> const& body() const { return m_body; }

    bool empty() const { return m_method.empty(); }

    void swap(request& rhs)
    {
        std::swap(m_method, rhs.m_method);
        std::swap(m_url, rhs.m_url);
        std::swap(m_protocol, rhs.m_protocol);
        std::swap(m_headers, rhs.m_headers);
        std::swap(m_body, rhs.m_body);
    }

private:
    std::string  m_method;
    std::string  m_url;
    std::string  m_protocol;
    headers_type m_headers;
    std::vector<uint8_t> m_body;
};

} // namespace http
} // namespace dhnet

#endif // HTTP_REQUEST_H__
