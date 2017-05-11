
#ifndef __FE_RTP_STRUCTS_H__
#define __FE_RTP_STRUCTS_H__

#include <string.h>
#include <vector>
#include "RTPProtocol.h"

namespace CompLib
{

    /** 传输层配置 */
    typedef struct tagTransPara
    {
        char ip[16];              /**< ip地址 */
        unsigned short rtpPort;   /**< RTP端口 */
        unsigned short rtcpPort;  /**< RTCP端口 */

        ///构造
        tagTransPara(const char* ipAddr, unsigned short port)
        {
            strncpy(ip, ipAddr, sizeof(ip) - 1);
            ip[15] = 0;
            rtpPort = port;
            rtcpPort = port + 1;
        }
    }TransPara;

    /** RTP基本配置 */
    typedef struct tagRTPBasicConfig
    {
        size_t	       frameRate;       /**< 帧率 */
        size_t         pktSize;         /**< 每一个分组大小,不包含头长度 */
        PayloadTypes   payloadType;     /**< 负载类型PT */
        unsigned short startSeq;        /**< 第一个载荷包的序列号 */
        unsigned long  ssrc;            /**< 同步源 */
        unsigned long  startTs;         /**< 起始时间戳 */
        unsigned int   sample;          /**< 对应媒体的抽样频率，单位Hz */
    }RTPBasicConfig;

    /** RTP扩展配置,NPOD类型 */
    typedef struct tagRTPExtConfig
    {
        bool                hasExtension;            /**< 是否有扩展 */
        unsigned short      extProfile;              /**< extension profile */
        std::vector<long>   vecExt;                  /**< 扩展数据 */
    }RTPExtConfig;

    /** RTCP配置 */
    typedef struct tagRTCPConfig
    {
        bool            enable;        /**< 是否启用RTCP */
        unsigned int    sendInterval;  /**< 发送SR间隔 */
    }RTCPConfig;

    /** 重传参数 */
    typedef struct tagRTPRetransCfg
    {
        bool           enable;   /**< 是否支持重传 */
        unsigned int   pktsNum;  /**< 重传多少个帧或包 */
    }RTPRetransCfg;

    /** 性能、收发控制参数 */
    typedef struct tagRTPProfileCfg
    {
        int      id;                    /**< 会话ID */
        size_t   maxSendNum;            /**< 一次心跳最多发送多少个RTP分组 */
        size_t   maxSendOctets;         /**< 一次心跳最多发送多少个字节数据(传输层) */
        size_t   maxSendOctetsThreshold;/**< 如果其他约束满足，传输层最多允许堆积的字节数 */
    }RTPProfileCfg;

    /** 会话基本控制 */
    typedef struct tagRTPSessionConfig
    {
        RTPBasicConfig basicCfg;
        RTPExtConfig   extCfg;
        RTCPConfig     rtcpCfg;

        RTPRetransCfg  retranCfg;
        RTPProfileCfg  profileCfg;

        ///默认
        void SetDefault();

        ///默认构造
        tagRTPSessionConfig(){SetDefault();}
    }RTPSessConfig;

    /** RTCP统计信息 */
    typedef struct tagRTCPStats
    {
        unsigned long rec_fraction_lost;
        unsigned long rec_packet_lost;
        unsigned long snd_octet;
        unsigned long snd_packet;
        unsigned long hight_sequence;
        unsigned long lastreport_time;    /**< 上次发送SR报告的时间 */
    }RTCPStats;

    /** RTP会话状态信息 */
    typedef struct tagRTPStatus
    {
        unsigned short lastSeq;         /**< 上一次发送的序列号 */
        unsigned short fracsCnt;        /**< 一秒数据帧内的帧片数,用于修正时间戳 */
        unsigned long  lastTimestamp;   /**< 上一个帧数据的时间戳 */
        unsigned int   lastUnsentNum;   /**< 上一个调度周期没有发送完毕的传输层数据单元个数 */
    }RTPStatus;

    //////////////////////////////////////////////////////////////////////////////////

    /**
    * 检查@c cfg 是否是一个有效的配置结构参数，如果合法，返回true
    * 检查约束如下：
    *  RTPBasicConfig  1 <= frameRate <= 30
    *                  484 <= pktSize + rtp  hdrlen <= 1472
    *  RTCPConfig      if enable , 2 <= sendInterval <= 60
    *  RTPRetransCfg   if enable,  0 < pksNum < 300
    *  RTPProfileCfg   if enable,  0 < maxSendNum < 15; 20 000 < maxSendOctets < 128 000
    *                              maxSendOctets < maxSendOctetsThreshold < 256 000
    */
    bool CheckConfig(RTPSessConfig& cfg);
}

#endif
