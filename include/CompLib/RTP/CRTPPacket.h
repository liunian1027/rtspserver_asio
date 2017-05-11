#ifndef __FE_RTP_PACKET_H__
#define __FE_RTP_PACKET_H__

#include "RTPProtocol.h"
#include <vector>

namespace CompLib
{
    /* 
    * RFC3550.txt
    * RTP: A Transport Protocol for Real-Time Applications
    *
    * The RTP header has the following format:
    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |V=2|P|X|  CC   |M|     PT      |       sequence number         |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                           timestamp                           |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |           synchronization source (SSRC) identifier            |
    +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
    |            contributing source (CSRC) identifiers             |
    |                             ....                              |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    * version (V): 2 bits
    * padding (P): 1 bit
    * extension (X): 1 bit
    * CSRC count (CC): 4 bits
    * marker (M): 1 bit
    * payload type (PT): 7 bits
    * sequence number: 16 bits
    * timestamp: 32 bits
    * SSRC: 32 bits
    * CSRC list: 0 to 15 items, 32 bits each
    */
    class CRTPPacket
    {
    public:
		/*
		功能: 默认构造, 指定缓冲大小
		参数: size, 缓冲大小; _interleaved, RTSP over TCP模式下的通道标识
		*/
        CRTPPacket(int size = 0, int _interleaved = 0);

		/*
		功能: 用已有的包拷贝构造
		参数: packet, 已有包的引用
		*/
        CRTPPacket(const CRTPPacket& packet);

		/*
		功能: 用外部指针初始化缓冲的构造, 
		      如果给定的数据流不是一个合法的RTP数据包，行为不正确
	    参数: data, 指向数据内容, len, 数据长度
		*/
        CRTPPacket(char* data, size_t len);

		/*
		功能: 用外部指针初始化缓冲的构造, 
		      如果给定的数据流不是一个合法的RTP数据包，行为不正确
			  (重载函数, 为了和中兴接口保持一致)
        参数: data, 指向数据内容, len, 数据长度
		*/
		CRTPPacket(char* data, int len);
        ~CRTPPacket();

        /*
		功能: 从给定的比特流解析RTP包
		参数: data, 指向RTP包; len, RTP包长度
		返回值: 如果版本和长度不符, 返回false; 否则, 返回true
		*/
        static bool ParseFromStream(const char*data, size_t len);

		/*
		功能: 设定RTP包的CSRC
		参数: csrc, RTP包的CSRC值
		*/
        void AddCSRC(int csrc);

		/*
		功能: 得到RTP包版本号
		返回值: 根据RFC3550定义, 版本号为2
		*/
        int  GetVersion() const;

		/*
		功能: 设定RTP包版本号
		参数: version, RTP版本号
		*/
        void SetVersion(int version);

		/*
		功能: 确定RTP包是否有填充
		返回值: 如果RTP包有填充, 
		*/
		bool GetPadding() const;

		/*
		功能: 设定RTP包是否有填充
		参数: pad, true, 包含填充; false, 不包含填充
		*/
		void SetPadding(bool pad);

		/*
		功能: 确定RTP包是否有扩展位(目前支持)
		返回值: false, 不包含填充位; true, 包含填充位
		*/
        bool GetExtension()const;

		/*
		功能: 设定RTP包是否有扩展
		参数: ext, true, 包含扩展; false, 不包含扩展
		*/
        void SetExtension(bool ext);

		/*
		功能: 确定RTP包是否是未经分包或者是分包后的最后一个
		返回值: 如果RTP包未经分包或者是分包后的最后一个, 返回true;
		        如果RTP包不是分包后的最后1个, 返回false
		*/
        bool GetMarker()const;

		/*
		功能: 设定RTP包的marker位 
		参数: mark, false, marker位为0, true, marker位为1
		*/
        void SetMarker(bool mark);

		/*
		功能: 得到RTP包的载荷类型
		返回值: 返回值定义如下
		enum PayloadTypes 
		{
			PCMU,         // G.711 u-Law *  
			FS1016,       // Federal Standard 1016 CELP 
			G721,         // ADPCM - Subsumed by G.726  
			G726 = G721,
			GSM,          // GSM 06.10  
			G7231,        // G.723.1 at 6.3kbps or 5.3 kbps  
			DVI4_8k,      // DVI4 at 8kHz sample rate 
			DVI4_16k,     // DVI4 at 16kHz sample rate  
			LPC,          // LPC-10 Linear Predictive CELP  
			PCMA,         // G.711 A-Law  
			G722,         // G.722  
			L16_Stereo,   // 16 bit linear PCM stereo  
			L16_Mono,     // 16 bit linear PCM monotone 
			G723,         // G.723  
			CN,           // Confort Noise  
			MPA,          // MPEG1 or MPEG2 audio  
			G728,         // G.728 16kbps CELP  
			DVI4_11k,     // DVI4 at 11kHz sample rate  
			DVI4_22k,     // DVI4 at 22kHz sample rate  
			G729,         // G.729 8kbps  
			Cisco_CN,     // Cisco systems comfort noise (unofficial)  
			CelB = 25,    // Sun Systems Cell-B video  
			JPEG,         // Motion JPEG 
			H261 = 31,    // H.261  
			MPV,          // MPEG1 or MPEG2 video  
			MP2T,         // MPEG2 transport system 
			H263,         // H.263  
			////Add official types here.
			LastKnownPayloadType,
			////Dynamic types that aren't official
			DynamicBase = 96,
			DynamicVideo = DynamicBase,
			DynamicAudio = 97,
			MaxPayloadType = 127,
			IllegalPayloadType
		};
		DVR的流媒体数据类型设为DynamicBase
		*/
        PayloadTypes GetPayloadType()const;

		/*
		功能: 设定RTP包的载荷类型
		参数: t 载荷类型
		enum PayloadTypes 
		{
		PCMU,         // G.711 u-Law *  
		FS1016,       // Federal Standard 1016 CELP 
		G721,         // ADPCM - Subsumed by G.726  
		G726 = G721,
		GSM,          // GSM 06.10  
		G7231,        // G.723.1 at 6.3kbps or 5.3 kbps  
		DVI4_8k,      // DVI4 at 8kHz sample rate 
		DVI4_16k,     // DVI4 at 16kHz sample rate  
		LPC,          // LPC-10 Linear Predictive CELP  
		PCMA,         // G.711 A-Law  
		G722,         // G.722  
		L16_Stereo,   // 16 bit linear PCM stereo  
		L16_Mono,     // 16 bit linear PCM monotone 
		G723,         // G.723  
		CN,           // Confort Noise  
		MPA,          // MPEG1 or MPEG2 audio  
		G728,         // G.728 16kbps CELP  
		DVI4_11k,     // DVI4 at 11kHz sample rate  
		DVI4_22k,     // DVI4 at 22kHz sample rate  
		G729,         // G.729 8kbps  
		Cisco_CN,     // Cisco systems comfort noise (unofficial)  
		CelB = 25,    // Sun Systems Cell-B video  
		JPEG,         // Motion JPEG 
		H261 = 31,    // H.261  
		MPV,          // MPEG1 or MPEG2 video  
		MP2T,         // MPEG2 transport system 
		H263,         // H.263  
		////Add official types here.
		LastKnownPayloadType,
		////Dynamic types that aren't official
		DynamicBase = 96,
		DynamicVideo = DynamicBase,
		DynamicAudio = 97,
		MaxPayloadType = 127,
		IllegalPayloadType
		};
		DVR的流媒体数据类型设为DynamicBase
		*/
        void SetPayloadType(PayloadTypes t);

		/*
		功能: 得到RTP包序号
		返回值: RTP包序号
		*/
        unsigned short GetSequenceNumber()const;

		/*
		功能: 设定RTP包序号
		参数: n, RTP包序号
		*/
        void SetSequenceNumber(unsigned short n);

		/*
		功能: 得到RTP包时间戳
		返回值: RTP包时间戳
		*/
        unsigned long GetTimestamp()const;

		/*
		功能: 设定RTP包时间戳
		参数: time, RTP包时间戳
		*/
        void SetTimestamp(unsigned long time);

		/*
		功能: 得到RTP包SSRC源
		返回值: RTP包SSRC源
		*/
        unsigned long GetSyncSource()const;

		/*
		功能: 设定RTP包SSRC源
		参数: ssrc, RTP包SSRC源
		*/
        void SetSyncSource(unsigned long ssrc);

		/*
		功能: 得到可选同步源标识的个数
		返回值: 可选同步源标识的个数
		*/
        unsigned char  GetContribSrcCount()const;

		/*
		功能: 设定可选同步源标识的个数
		参数: cc, 可选同步源标识的个数
		*/
		void SetContribSrcCount(unsigned char cc);

		/*
		功能: 得到RTP包第index源的CSRC
		参数: index, 第index源
		返回值: RTP包第index源的CSRC
		*/
        unsigned long GetContribSource(unsigned char index)const;
        
		/*
		功能: 设定RTP包第index源的CSRC
		参数: index, 第index源; csrc, RTP包第index源的CSRC
		*/
        void SetContribSource(int index, unsigned long csrc) ;

		/*
		功能: 得到RTP包头大小
		返回值: RTP包头大小
		*/
        int GetHeaderSize()const;

		/*
		功能: 得到RTP包的扩展类型
		返回值: RTP包扩展类型
		*/
        unsigned short GetExtensionType()const;

		/*
		功能: 设定RTP包的扩展类型
		参数: type, RTP包的扩展类型
		*/
        void SetExtensionType(unsigned short type);

		/*
		功能: 得到RTP包头扩展大小
		返回值: RTP包头扩展大小
		*/
        unsigned short GetExtensionSize()const;

		/*
		功能: 设定RTP包头扩展大小
		参数: len, RTP包头扩展大小
		*/
        void SetExtensionLen(unsigned short len);

		/*
		功能: 得到RTP包头扩展指针
		参数: 指向RTP包头扩展
		*/
        const char* GetExtensionPtr()const;

		/*
		功能: 设定RTP包头扩展
		参数: vecExt, RTP包头扩展
		*/
        void SetExtensionContent(const std::vector<long>& vecExt);

		/*
		功能: 设定RTP包头扩展
		参数: type, 扩展类型; pBuf, 扩展缓冲; len, 扩展缓冲长度
		返回值: <0, 失败; >=0, 成功
		*/
		int SetExtensionBuf(unsigned short type, void *pbuf, int len);

        /* 包体处理 */
		/*
		功能: 得到RTP包负载长度
		返回值: RTP包负载大小
		*/
        int GetPayloadSize()const;
		/*
		功能:得到RTP包实际负载长度，不包括填充字节
		返回值:不包括填充字节的RTP包负载大小
		*/
		int GetPayloadNoPaddingSize()const;

		/*
		功能: 添加负载数据, 传入的指针数据被拷贝组包,
		      效率要差一点, 但更安全
	    参数: data, 指向传入的数据; len, 传入数据长度
		返回值: <0, 失败; >=0, 成功
		*/
        int AddPayload(const char* data, int len);

		/*
		功能: 添加负载数据, 传入的指针数据被拷贝组包,
		      效率要差一点, 但更安全(重载版本)
			  (中兴接口)
		参数: data, 指向传入的数据; len, 传入数据长度
		返回值: <0, 失败; >=0, 成功
		*/
		int AddPayload(char* data, int len);

		/*
		功能: 读取负载数据, 不包括RTP头数据
		参数: len, 传出参数, 负载数据长度
		返回值: 指向负载数据
		*/
        const char* ReadPayload(int& len)const;

		/*
		功能: 读取负载数据, 不包括RTP头数据,不包括填充字节
		参数: len, 传出参数, 负载数据长度(不包括填充字节)
		返回值: 指向负载数据
		*/
        const char* ReadPayloadNoPadding(int& len)const;

		/*
		功能: 设置负载缓冲，但不拷贝内存,
		      如果需要打包网络化流，则传入指针的生存期必须比该类对象的生存期长，否则会读取垃圾内存
              如果需要获取网络化流，则后续对负载的拷贝必不可少，因为必须串接头和负载形成比特流
	    参数: data, 指向负载数据, len, 负载长度
		返回值: <0, 失败; >=0, 成功
		*/
        int SetPayloadPtr(const char* data, size_t len);

		/*
		功能: 设置负载缓冲，但不拷贝内存,
		如果需要打包网络化流，则传入指针的生存期必须比该类对象的生存期长，否则会读取垃圾内存
		如果需要获取网络化流，则后续对负载的拷贝必不可少，因为必须串接头和负载形成比特流
		(重载版本, 中兴接口)
		参数: data, 指向负载数据, len, 负载长度
		返回值: <0, 失败; >=0, 成功
		*/
		int SetPayloadPtr(char* data, int len);

        /*
		功能: 得到序列化网络流
		返回值: 指向序列化网络流
		*/
        const char* GetStream(void)const;

		/*
		功能: 得到序列化网络流(重载版本, 中兴接口)
		返回值: 指向序列化网络流
		*/
		char* GetStream();

		/*
		功能: 得到序列化网络流长度
		返回值: 序列化网络流长度
		*/
        int GetStreamLen(void)const;

		/*
		功能: 设定RTSP OVER TCP方式下, RTP包通道标识
		参数: chn, RTP包通道标识
		*/
		int AddRtspChn(char chn);

		/*
		功能: 释放对象引用
		返回值: 如果--_ref等于0, delete this, 返回0;
		否则返回--_ref
		*/
        int Release(void);

		/*
		功能: 增加对象引用
		返回值: ++_ref
		*/
        int AddRef(void);

    public:
        static const int MIN_HEAD_LENGTH = 12;
        static const int PROTOCOL_VERSION = 2;
        static const int RTP_BUFFER_SIZE = 8*1024;

    protected:
        int	  _length;	/**< 负载长度 */
        char* _buffer;	/**< 流缓冲 */
        int   _wholeLen;/**< 流总长度，包括RTP头+RTP扩展+RTP负载 */
        int   _ref;     /**< 引用计数 */

        bool  _bCopy;           /**< 对应的内存空间是否是外部内存 */
        bool  _bExtPayload;     /**< 是否使用外部Payload指针 */
        const char* _extPayload;/**< 外部Payload */
		int _interleaved;

    };

    class IRTPListener
    {
    public:
        virtual ~IRTPListener(){};
		/*
		功能: 此处的packet指向的内存为调用方的临时内存，上层如果需要异步存储该数据，则需要自行做深度拷贝
		      比如可以通过CRTPPacket的拷贝构造来完成，后者直接拷贝其缓冲流重构造; 
			  一般情况下，函数返回后，RTPPacket指向的内存空间就不再有效
	    参数: session, 会话标识; connId, 通道ID, packet, 指向预处理的RTP包
		返回值: <0, 上层处理发生错误; >=0, 上层处理完毕, 无异常
		*/
        virtual int onRTPPacket(int session, int connId, CRTPPacket* packet) = 0;
    };

}

#endif
