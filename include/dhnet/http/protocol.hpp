#ifndef HTTP_PROTOCOL_HPP
#define HTTP_PROTOCOL_HPP

#include <string>

namespace dhnet {
namespace http {


// 状态码
struct status_code
{
    enum type
    {
        ok = 200,
        created = 201,
        accepted = 202,
        no_content = 204,
        multiple_choices = 300,
        moved_permanently = 301,
        moved_temporarily = 302,
        not_modified = 304,
        bad_request = 400,
        unauthorized = 401,
        forbidden = 403,
        not_found = 404,
        internal_server_error = 500,
        not_implemented = 501,
        bad_gateway = 502,
        service_unavailable = 503
    };
};

namespace status_string {

    const std::string ok =
        "200 OK";
    const std::string created =
        "201 Created";
    const std::string accepted =
        "202 Accepted";
    const std::string no_content =
        "204 No Content";
    const std::string multiple_choices =
        "300 Multiple Choices";
    const std::string moved_permanently =
        "301 Moved Permanently";
    const std::string moved_temporarily =
        "302 Moved Temporarily";
    const std::string not_modified =
        "304 Not Modified";
    const std::string bad_request =
        "400 Bad Request";
    const std::string unauthorized =
        "401 Unauthorized";
    const std::string forbidden =
        "403 Forbidden";
    const std::string not_found =
        "404 Not Found";
    const std::string internal_server_error =
        "500 Internal Server Error";
    const std::string not_implemented =
        "501 Not Implemented";
    const std::string bad_gateway =
        "502 Bad Gateway";
    const std::string service_unavailable =
        "503 Service Unavailable";
}

inline
std::string to_string(status_code::type const& code)
{
    using namespace status_string;
    switch (code)
    {
    case status_code::ok:
        return ok;
    case status_code::created:
        return created;
    case status_code::accepted:
        return accepted;
    case status_code::no_content:
        return no_content;
    case status_code::multiple_choices:
        return multiple_choices;
    case status_code::moved_permanently:
        return moved_permanently;
    case status_code::moved_temporarily:
        return moved_temporarily;
    case status_code::not_modified:
        return not_modified;
    case status_code::bad_request:
        return bad_request;
    case status_code::unauthorized:
        return unauthorized;
    case status_code::forbidden:
        return forbidden;
    case status_code::not_found:
        return not_found;
    case status_code::internal_server_error:
        return internal_server_error;
    case status_code::not_implemented:
        return not_implemented;
    case status_code::bad_gateway:
        return bad_gateway;
    case status_code::service_unavailable:
        return service_unavailable;
    default:
        return internal_server_error;
    }
}


namespace stock_body {

    const char ok[] = "";
    const char created[] =
      "<html>"
      "<head><title>Created</title></head>"
      "<body><h1>201 Created</h1></body>"
      "</html>";
    const char accepted[] =
      "<html>"
      "<head><title>Accepted</title></head>"
      "<body><h1>202 Accepted</h1></body>"
      "</html>";
    const char no_content[] =
      "<html>"
      "<head><title>No Content</title></head>"
      "<body><h1>204 Content</h1></body>"
      "</html>";
    const char multiple_choices[] =
      "<html>"
      "<head><title>Multiple Choices</title></head>"
      "<body><h1>300 Multiple Choices</h1></body>"
      "</html>";
    const char moved_permanently[] =
      "<html>"
      "<head><title>Moved Permanently</title></head>"
      "<body><h1>301 Moved Permanently</h1></body>"
      "</html>";
    const char moved_temporarily[] =
      "<html>"
      "<head><title>Moved Temporarily</title></head>"
      "<body><h1>302 Moved Temporarily</h1></body>"
      "</html>";
    const char not_modified[] =
      "<html>"
      "<head><title>Not Modified</title></head>"
      "<body><h1>304 Not Modified</h1></body>"
      "</html>";
    const char bad_request[] =
      "<html>"
      "<head><title>Bad Request</title></head>"
      "<body><h1>400 Bad Request</h1></body>"
      "</html>";
    const char unauthorized[] =
      "<html>"
      "<head><title>Unauthorized</title></head>"
      "<body><h1>401 Unauthorized</h1></body>"
      "</html>";
    const char forbidden[] =
      "<html>"
      "<head><title>Forbidden</title></head>"
      "<body><h1>403 Forbidden</h1></body>"
      "</html>";
    const char not_found[] =
      "<html>"
      "<head><title>Not Found</title></head>"
      "<body><h1>404 Not Found</h1></body>"
      "</html>";
    const char internal_server_error[] =
      "<html>"
      "<head><title>Internal Server Error</title></head>"
      "<body><h1>500 Internal Server Error</h1></body>"
      "</html>";
    const char not_implemented[] =
      "<html>"
      "<head><title>Not Implemented</title></head>"
      "<body><h1>501 Not Implemented</h1></body>"
      "</html>";
    const char bad_gateway[] =
      "<html>"
      "<head><title>Bad Gateway</title></head>"
      "<body><h1>502 Bad Gateway</h1></body>"
      "</html>";
    const char service_unavailable[] =
      "<html>"
      "<head><title>Service Unavailable</title></head>"
      "<body><h1>503 Service Unavailable</h1></body>"
      "</html>";
}


inline
std::string to_stock_body(status_code::type const& code)
{
    using namespace stock_body;
    switch (code)
    {
    case status_code::ok:
        return ok;
    case status_code::created:
        return created;
    case status_code::accepted:
        return accepted;
    case status_code::no_content:
        return no_content;
    case status_code::multiple_choices:
        return multiple_choices;
    case status_code::moved_permanently:
        return moved_permanently;
    case status_code::moved_temporarily:
        return moved_temporarily;
    case status_code::not_modified:
        return not_modified;
    case status_code::bad_request:
        return bad_request;
    case status_code::unauthorized:
        return unauthorized;
    case status_code::forbidden:
        return forbidden;
    case status_code::not_found:
        return not_found;
    case status_code::internal_server_error:
        return internal_server_error;
    case status_code::not_implemented:
        return not_implemented;
    case status_code::bad_gateway:
        return bad_gateway;
    case status_code::service_unavailable:
        return service_unavailable;
    default:
        return internal_server_error;
    }
}


namespace mime_type {

    static char const gif[]    = "image/gif";
    static char const jpeg[]   = "image/jpeg";
    static char const png[]    = "image/png";
    static char const html[]   = "text/html";
    static char const plain[]  = "text/plain";
    static char const sdp[]    = "application/sdp";
    static char const rtp[]    = "application/rtp";
}


/// Convert a file extension into a MIME type.
inline
std::string extension_to_mime_type(const std::string& extension)
{
    struct mapping
    {
        const char* extension;
        const char* mime_type;
    };
    static mapping const mappings[] =
    {
        { "gif",    mime_type::gif },
        { "htm",    mime_type::html },
        { "html",   mime_type::html },
        { "jpg",    mime_type::jpeg },
        { "png",    mime_type::png },
        { 0, 0 } // Marks end of list.
    };

    std::string ext_lower;
    std::transform(extension.begin(), extension.end(),
            std::back_inserter(ext_lower), tolower);

    for (mapping const* m = mappings; m->extension; ++m)
    {
        if ( m->extension == ext_lower )
        {
            return m->mime_type;
        }
    }

    return mime_type::plain;
}


/// \brief url解码
template<typename InIt, typename OutIt>
inline
size_t decode_url(InIt first, InIt last, OutIt out)
{
    size_t out_size=0;
    while (first!=last)
    {
        if (*first == '%')
        {
            ++first;
            std::stringstream ss;
            for ( int i=0; i<2; ++i )
            {
                if (first!=last)
                {
                    ss<<static_cast<const char>(*first++);
                }
                else
                {
                    /// url 地址解析出错, 这里暂不做错误处理, 直接跳出循环
                    break;
                }
            }
            int value = 0;
            ss >> std::hex >> value;
            *out++ = static_cast<const char>(value);
            ++out_size;
        }
        else if (*first == '+')
        {
            *out++ = ' ';
            ++out_size;
            ++first;
        }
        else
        {
            *out++ = static_cast<const char>(*first++);
            ++out_size;
        }

    }
    return ++out_size;
}

} // namespace http
} // namespace dhnet

#endif // HTTP_PROTOCOL_HPP

