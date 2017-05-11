
#ifndef HTTP_REPLY_H__
#define HTTP_REPLY_H__

#include <cassert>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <boost/lexical_cast.hpp>
#include <dhlib/helper.hpp>
#include "header.hpp"
#include "protocol.hpp"


namespace dhnet {
namespace http {


class reply
{
public:
    reply()
    {
    }

    /// 构造无BODY的响应
    explicit reply( std::string const& protocol, status_code::type status )
        : m_protocol(protocol)
        , m_status_code(status)
    {
        std::string const& body = to_stock_body(status);
        if (body.size()>0)
        {
            m_body.reserve(body.size());
            std::copy(body.begin(),body.end(),std::back_inserter(m_body));
            m_headers.push_back(header("Content-type", "text/html"));
            m_headers.push_back(
                header("Content-length",
                    boost::lexical_cast<std::string>(m_body.size())));
        }
    }

    /// 构造成功的响应
    template<typename BodyIt>
    reply( std::string const& protocol, std::string const& mime_type,
        BodyIt first, BodyIt last )
        : m_protocol(protocol)
        , m_status_code(status_code::ok)
    {
        m_body.reserve(1600);
        std::copy(first,last,std::back_inserter(m_body));

        if (m_body.size()>0)
        {
            m_headers.push_back(header("Content-type", mime_type));
            m_headers.push_back(
                header("Content-length",
                    boost::lexical_cast<std::string>(m_body.size())));
        }
    }

    /// 直接从文件构造响应
    reply( std::string const& protocol, std::string const& filename )
        : m_protocol(protocol)
        , m_status_code(status_code::ok)
    {
        FILE* fp = fopen(filename.c_str(), "rb");
        if ( NULL == fp )
        {
            reply tmp(protocol, status_code::not_found);
            swap(tmp);
        }
        else
        {
            m_body.resize(dhlib::get_file_size(filename));
            fread(&m_body[0], sizeof(m_body[0]), m_body.size(), fp);

            std::string extension(dhlib::get_extension(filename));
            std::string mime_type(extension_to_mime_type(extension));

            m_headers.push_back(header("Content-type", mime_type));
            m_headers.push_back(
                header("Content-length",
                    boost::lexical_cast<std::string>(m_body.size())));

            fclose(fp);
        }
    }

    void push_header(header const& h)
    {
        m_headers.push_back(h);
    }

    std::string const& protocol() const { return m_protocol; }
    status_code::type status() const { return m_status_code; }
    headers_type const& headers() const { return m_headers; }
    std::vector<uint8_t> const& body() const { return m_body; }

    bool empty() const { return m_protocol.empty(); }

    void swap(reply& rhs)
    {
        std::swap(m_protocol, rhs.m_protocol);
        std::swap(m_status_code, rhs.m_status_code);
        std::swap(m_headers, rhs.m_headers);
        std::swap(m_body, rhs.m_body);
    }

private:
    std::string          m_protocol;
    status_code::type    m_status_code;
    headers_type         m_headers;
    std::vector<uint8_t> m_body;
};



} // namespace http
} // namespace dhnet

#endif // HTTP_REPLY_H__
