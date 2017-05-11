#ifndef HTTP_HEADER_H__
#define HTTP_HEADER_H__

#include <string>

namespace dhnet {
namespace http {

/// HTTP ͷ�ṹ
struct header
{
    std::string name;
    std::string value;

    header()
    {
    }
    header(char const* n, char const* v)
        : name(n), value(v)
    {
    }
    header(std::string const& n, std::string const& v)
        : name(n), value(v)
    {
    }
};


typedef std::vector<header> headers_type;


/// �� headers �в��� name ��Ӧ�� value
inline
std::string const& find_value(
                headers_type const& headers,
                char const* name)
{
    headers_type::const_iterator it = headers.begin();
    for ( ; it!= headers.end(); ++it )
    {
        if ( it->name == name )
        {
            return it->value;
        }
    }

    //static std::string const empty;
    return std::string(); // ������ֵ������
}


/// HTTPͷת��Ϊ�ַ���
inline
std::string to_string(header const& h)
{
    std::string str;
    str.append(h.name);
    str.append(": ");
    str.append(h.value);
    str.append("\r\n");
    return str;
}

/// \brief �������в����н�����
/// \return ָ���н�������λ�õĵ�����
template<typename InIt>
inline
InIt search_crlf( InIt first, InIt last )
{
    static char const crlf[] = "\r\n";
    return std::search( first, last, &crlf[0], &crlf[2]);
}

/// ��һ������ȡHTTPͷ
template<typename InIt>
inline
header parse_header( InIt first, InIt last )
{
    InIt name_end = std::find(first, last, ':');
    if (name_end!=last)
    {
        InIt value_begin = name_end;
        std::advance(value_begin,2);
        return header(std::string(first, name_end),
                std::string(value_begin, last));
    }
    return header();
}

/// �ڶ�������ȡ���HTTPͷ
template<typename InIt, typename OutIt>
inline
void parse_headers(InIt first, InIt last, OutIt headers)
{
    InIt line_begin = first;
    InIt line_end = search_crlf(first,last);
    while (line_begin!=line_end) ///< ���������˳�
    {
        *headers++ = parse_header(line_begin,line_end);
        line_begin = line_end;
        std::advance(line_begin,2);
        line_end = search_crlf(line_begin,last);
    }
}

} // namespace http
} // namespace dhnet

#endif // HTTP_HEADER_H__

