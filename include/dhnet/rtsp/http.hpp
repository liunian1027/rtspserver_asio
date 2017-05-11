#ifndef RTSP_HTTP_H__
#define RTSP_HTTP_H__


#include "../http/packet_helper.hpp"
#include "../http/request.hpp"
#include "../http/reply.hpp"
#include "../http/to_packet.hpp"
#include "../http/protocol.hpp"

namespace dhnet {
namespace rtsp {


/// 导入http命名空间的相应名称
typedef http::packet_helper packet_helper;
typedef http::header        header;
typedef http::request       request;
typedef http::reply         reply;
typedef http::status_code   status_code;

namespace mime_type = http::mime_type;

} // namespace rtsp
} // namespace dhnet

#endif //RTSP_HTTP_H__
