#ifndef _RTP_PROTOCOL_H_
#define _RTP_PROTOCOL_H_

namespace CompLib
{
    /** RTP收发应用模式 */
    enum Model
    {
        SENDER = 1,  /**< 发送数据模式 */
        RECEIVER     /**< 接收数据模式 */
    };

    /** RTP传输层模式 */
    enum TransModel
    {
        RTPOverUDP = 0,    /**< RTP/UDP传输模式 */
        RTPOverTCP = 1,    /**< RTP/TCP模式 */
        RTPOverRTSP        /**< RTP/RTSP模式 */
    };

    /** RTP负载类型 */
    enum PayloadTypes 
    {
        PCMU,         /**< G.711 u-Law */  
        FS1016,       /**< Federal Standard 1016 CELP */
        G721,         /**< ADPCM - Subsumed by G.726 */ 
        G726 = G721,
        GSM,          /**< GSM 06.10 */ 
        G7231,        /**< G.723.1 at 6.3kbps or 5.3 kbps */ 
        DVI4_8k,      /**< DVI4 at 8kHz sample rate */ 
        DVI4_16k,     /**< DVI4 at 16kHz sample rate */ 
        LPC,          /**< LPC-10 Linear Predictive CELP */ 
        PCMA,         /**< G.711 A-Law */ 
        G722,         /**< G.722 */ 
        L16_Stereo,   /**< 16 bit linear PCM stereo */ 
        L16_Mono,     /**< 16 bit linear PCM monotone*/ 
        G723,         /**< G.723 */ 
        CN,           /**< Confort Noise */ 
        MPA,          /**< MPEG1 or MPEG2 audio */ 
        G728,         /**< G.728 16kbps CELP */ 
        DVI4_11k,     /**< DVI4 at 11kHz sample rate */ 
        DVI4_22k,     /**< DVI4 at 22kHz sample rate */ 
        G729,         /**< G.729 8kbps */ 
        Cisco_CN,     /**< Cisco systems comfort noise (unofficial) */ 

        CelB = 25,    /**< Sun Systems Cell-B video */ 
        JPEG,         /**< Motion JPEG */ 
        H261 = 31,    /**< H.261 */ 
        MPV,          /**< MPEG1 or MPEG2 video */ 
        MP2T,         /**< MPEG2 transport system */
        H263,         /**< H.263 */ 

        ////Add official types here.
        LastKnownPayloadType,

        ////Dynamic types that aren't official
        DynamicBase = 96,
        DynamicVideo = DynamicBase,
        DynamicAudio = 97,
        MaxPayloadType = 127,
        IllegalPayloadType
    };

    /** RTCP消息类型 */
    enum RTCPTYPE
    {
        SR		= 200,  /**< Sender Report */
        RR		= 201,  /**< Receiver Report*/
        SDES	= 202,  /**< Sender Description */
        BYE		= 203,  /**< Bye */
        APP		= 204,  /**< Application specification */
    };

    /** 媒体数据帧类型 */
	/** bit7 ~ bit2表示编码类型 */
	/** 0:MPEG-4 */
	/** 1:H.264 */
    enum MediaFrameType
    {
        KeyFrame = 1,       /**< 关键帧 */
        IFrame = KeyFrame,  /**< I帧 */
        PFrame = 2,         /**< P帧 */
        BFrame = 3,         /**< B帧 */
		H264Frame = 0x04,	/**< H.264帧 */
    };

    typedef struct
    {
        unsigned long ntpTimestampHigh; /**< NTP time high word*/
        unsigned long ntpTimestampLow;  /**< NTP time low word*/
        unsigned long rtpTimestamp;     /**< RTP timestamp */
        unsigned long packetCount;      /**< total packets sent till now */
        unsigned long octetCount;       /**< total octets sent*/
    } SSenderInfo;

    typedef struct
    {
        unsigned long fractionLost : 8;    //SR报告间隔丢包数
        unsigned long packetsLost  : 24;   //总共丢包数
    } SLost;

    typedef struct
    {
        SLost lost;
        unsigned long highestSequence;     //RR报告中的最大RTP序号
        unsigned long jitter;              //网络抖动量
        unsigned long lastSR;              //上一个SR情况报告时间戳
        unsigned long delaySR;             //最近的两个SR报告的时间间隔
    } SReportBlock;

    typedef struct 
    {
        unsigned char type;               //type类型 
        unsigned char length;             //数据长度
        unsigned char data[64];           //数据内容
    }SCname;

    typedef struct 
    {
        unsigned long ssrc;   //源SSRC标示
        SCname scname;        //源端机器标示
    }SChunk;

    typedef struct 
    {
        unsigned long name;        //源端标示
        unsigned short seqence1;   //重传包开始序号
        unsigned short seqence2;   //重传包结束序号
    }SAppResend;

}

#endif//_RTP_PROTOCOL_H_

