
#ifdef __WIN32__
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>	/* inet(3) functions */
#endif

#include <fcntl.h>		/* for nonblocking */
#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include "CRTPPacket.h"

namespace CompLib
{
	//默认构造, 指定缓冲大小
	CRTPPacket::CRTPPacket(int size, int interleaved)
	{
		if (size <= 0)
		{
			_buffer = new char[RTP_BUFFER_SIZE];
			memset(_buffer, 0, RTP_BUFFER_SIZE);
			_length = RTP_BUFFER_SIZE;
			_wholeLen = RTP_BUFFER_SIZE;
		}
		else
		{
			_buffer = new char[size];
			memset(_buffer, 0, size);
			_length = 0;
			_wholeLen = size;
		}
		_interleaved = interleaved;
		/*
		*	yehao 2007-08-22:
		*	初始化的RTP VER为10
		*/
		_buffer[0 + _interleaved] |= 0x80;

		_bCopy = 0;
		_ref = 0;
		_extPayload = 0;
		_bExtPayload = false;
	}

	//从给定的比特流解析RTP包
    bool CRTPPacket::ParseFromStream(const char*data, size_t len)
    {
        ///先检查版本，如果版本错误，直接返回错误
        unsigned char byte = (unsigned char)(*data);
        if (!(byte & 0x80))
        {
            printf("====RTP Parse error, bad RTP version!====\n");
            return false;
        }

        ///检验长度
        if (len < MIN_HEAD_LENGTH)
        {
            printf("===RTP parase error, len < 12!====\n");
        }

        ///TODO:检验扩展字段,len>=12
        return true;
    }

	//用外部指针初始化缓冲的构造, 如果给定的数据流不是一个合法的RTP数据包，行为不正确
    CRTPPacket::CRTPPacket(char* data, size_t len)
    {
        if (!ParseFromStream(data, len))
        {
            assert(!"====Bad RTP Stream!===\n");
        }
        _buffer = data;
        _wholeLen = len;
        _length = len - GetHeaderSize();
        assert(_length > 0 && _length < len);

        /// 析构函数不能删除本只读内存
        _bCopy = true;
        _ref = 0;

        _extPayload = 0;
        _bExtPayload = false;
		_interleaved = 0;
    }

	//用外部指针初始化缓冲的构造
	CRTPPacket::CRTPPacket(char* data, int len)
	{
		_buffer = data;
		_interleaved = 0;
		_length = len - GetHeaderSize() - _interleaved;

		_wholeLen = len;
		_bCopy = 1;

		_ref = 0;
	}

    CRTPPacket::CRTPPacket(const CRTPPacket& packet)
    {
		_interleaved = 0;
        _length = packet._length;
        _buffer = new char[packet._wholeLen];
        _wholeLen = packet._wholeLen;
        memcpy(_buffer, packet._buffer, packet._wholeLen);
        _bCopy = false;

        _extPayload = 0;
        _bExtPayload = false;

        _ref = 0;
    }

    int CRTPPacket::Release(void)
    {
        if(--_ref == 0)
        {
            delete this;
            return 0;
        }
        else
        {
            return _ref;
        }
    }

    int CRTPPacket::AddRef(void)
    {
        return ++_ref;
    }

    CRTPPacket::~CRTPPacket()
    {
        ///这里需要删除自己申请的内存
        if (!_bCopy)
        {
            delete []_buffer;
        }
    }

	//获取RTP包版本号
    int CRTPPacket::GetVersion()const
    {
        return (_buffer[0 + _interleaved] >> 6) & 3;
    }

	//设定RTP包版本号
    void CRTPPacket::SetVersion(int version)
    {
        _buffer[0 + _interleaved] |= ((version<<6) & 0xc0);
    }

	//确定RTP包是否有填充
	bool CRTPPacket::GetPadding()const
	{
		return (_buffer[0] & 0x20) ? 1 : 0;
	}

	//设定RTP包是否有填充
	void CRTPPacket::SetPadding(bool pad)
	{
		pad ? (_buffer[0] |= 0x20) : (_buffer[0] &= (~0x20));
	}

    void CRTPPacket::AddCSRC(int csrc)
    {

    }

	//确定RTP包是否有扩展位(目前支持)
    bool CRTPPacket::GetExtension()const
    {
        return (_buffer[0 + _interleaved] & 0x10) != 0;
    }

	//设定RTP包是否有扩展
    void CRTPPacket::SetExtension(bool ext)
    {
        ext ? _buffer[0 + _interleaved] |= 0x10 : _buffer[0 + _interleaved] &= 0xef;
    }

	//确定RTP包是否是未经分包或者是分包后的最后一个
    bool CRTPPacket::GetMarker()const
    {
        return (_buffer[1 + _interleaved] & 0x80) != 0;
    }

	//设定RTP包的marker位
    void CRTPPacket::SetMarker(bool mark)
    {
        mark ? (_buffer[1 + _interleaved] |= 0x80) : (_buffer[1 + _interleaved] &= 0x7f);
    }

	//获取RTP包的载荷类型
    PayloadTypes CRTPPacket::GetPayloadType()const
    {
        return (PayloadTypes)(_buffer[1 + _interleaved] & 0x7f);
    }

	//设定RTP包的载荷类型
    void CRTPPacket::SetPayloadType(PayloadTypes t)
    {
        _buffer[1 + _interleaved] = _buffer[1 + _interleaved] & 0x80 | t;
    }

	//获取RTP包序号
    unsigned short CRTPPacket::GetSequenceNumber()const
    {
        unsigned short value = *(unsigned short *)&_buffer[2 + _interleaved];
        return ntohs(value);
    }

	//设定RTP包序号
    void CRTPPacket::SetSequenceNumber(unsigned short n)
    {
        *(unsigned short *)&_buffer[2 + _interleaved] = htons(n);
    }

	//获取RTP包事件戳
    unsigned long CRTPPacket::GetTimestamp()const
    {
        return ntohl(*(unsigned long *)&_buffer[4 + _interleaved]);
    }

	//设定RTP包时间戳
    void CRTPPacket::SetTimestamp(unsigned long time)
    {
        *(unsigned int *)&_buffer[4 + _interleaved] = htonl(time);
    }

	//获取RTP包SSRC源
    unsigned long CRTPPacket::GetSyncSource()const
    {
        return ntohl(*(unsigned long *)&_buffer[8 + _interleaved]);
    }

	//设定RTP包SSRC源
    void CRTPPacket::SetSyncSource(unsigned long ssrc)
    {
        *(unsigned int *)&_buffer[8 + _interleaved] = htonl(ssrc);
    }

	//设定可选同步源标识的个数
	void CRTPPacket::SetContribSrcCount(unsigned char cc)
	{
		if(cc > 15)
		{
			printf("Set ContribSrcCount error,cc = 0x%X\n");
		}
		else
		{
			_buffer[0 + _interleaved] |= (cc & 0x0F);
		}
	}

	//获取可选同步源标识的个数
    unsigned char CRTPPacket::GetContribSrcCount()const
    {
        return _buffer[0 + _interleaved] & 0xf;
    }

	//获取RTP包第index源的CSRC
    unsigned long CRTPPacket::GetContribSource(unsigned char index)const
    {
        ///这里需要确保地址有效
        unsigned char cnt = GetContribSrcCount();
        if (index >= cnt)
        {
            printf("===CRTPPacket===, GetCSRC index error!\n");
            return 0;
        }
        assert(_wholeLen > MIN_HEAD_LENGTH + cnt * 4);

        return ntohl(((unsigned long *)&_buffer[MIN_HEAD_LENGTH + _interleaved])[index]);
    }

	//设定RTP包第index源的CSRC
    void CRTPPacket::SetContribSource(int index, unsigned long csrc)
    {
        ///确保前边字段正确，地址正确，防止读取垃圾内存
        unsigned char cnt = GetContribSrcCount();
        if (index >= cnt)
        {
            printf("===CRTPPacket===, SetCSRC index error!\n");
            return ;
        }
        assert(_wholeLen > MIN_HEAD_LENGTH + cnt * 4);
        ((unsigned int *)&_buffer[MIN_HEAD_LENGTH])[index + _interleaved] = htonl(csrc);
    }

	//获取RTP包头大小
    int CRTPPacket::GetHeaderSize()const
    {
        return MIN_HEAD_LENGTH + 4 * GetContribSrcCount() + GetExtensionSize();
    }

	//获取RTP包的扩展类型
    unsigned short CRTPPacket::GetExtensionType()const
    {
        return GetExtension() ? *(unsigned short *)&_buffer[MIN_HEAD_LENGTH + _interleaved + 4 * GetContribSrcCount()] : (-1);
        //return GetExtension() ? *(unsigned short *)&_buffer[MIN_HEAD_LENGTH] : (-1);
    }

	//设定RTP包的扩展类型
    void CRTPPacket::SetExtensionType(unsigned short type)
    {
        GetExtension() ? *(unsigned short *)&_buffer[MIN_HEAD_LENGTH + _interleaved + 4 * GetContribSrcCount()] = type : 0;
        //GetExtension() ? *(unsigned short *)&_buffer[MIN_HEAD_LENGTH] = htons(type) : 0;
    }

	//设定RTP包头扩展大小
    void CRTPPacket::SetExtensionLen(unsigned short len)
    {
        GetExtension() ? (*(unsigned short *)&_buffer[MIN_HEAD_LENGTH + _interleaved + 2]) = htons(len) : 0 ;
    }

	//获取RTP包头扩展大小
    unsigned short CRTPPacket::GetExtensionSize()const
    {
        //return GetExtension() ? *(unsigned short *)&_buffer[MIN_HEAD_LENGTH + 4 * GetContribSrcCount() + 2] : 0;
        if (GetExtension())
        {
            int extCnt = ntohs(*(unsigned short *)&_buffer[MIN_HEAD_LENGTH  +_interleaved + 4 * GetContribSrcCount() + 2]);
            return extCnt * 4 + 4;
        }
        else
        {
            return 0;
        }
    }

	//获取RTP包头扩展指针
    const char* CRTPPacket::GetExtensionPtr()const
    {
        return GetExtension() ? &_buffer[MIN_HEAD_LENGTH + _interleaved + 4 * GetContribSrcCount() + 4] : (char *)0;
        //return GetExtension() ? &_buffer[MIN_HEAD_LENGTH + 4] : (char *)0;
    }

	//设定RTP包头扩展
    void CRTPPacket::SetExtensionContent(const std::vector<long>& vecExt)
    {
        if (GetExtension())
        {
            long* pItem = (long*)GetExtensionPtr();
            for (std::vector<long>::const_iterator it = vecExt.begin();
                    it != vecExt.end(); ++it)
            {
                *pItem = htonl(*it);
                pItem += 4;
            }
        }
    }

	//设定RTP包头扩展
	int CRTPPacket::SetExtensionBuf(unsigned short type, void *pbuf, int len)
	{
		SetExtension(true);
		SetExtensionType(type);
		unsigned short *ptrlen = (unsigned short *)(_buffer + _interleaved + MIN_HEAD_LENGTH+4*GetContribSrcCount()+2);
		unsigned short tmplen = len%4;
		if (len/4)
		{
			tmplen += 1;
		}
		*ptrlen = htons(tmplen);
		char *ptrExt = (char*)GetExtensionPtr();
		if (ptrExt)
		{
			memcpy(ptrExt, pbuf, len);
		}
		else
		{
			return -1;
		}
		return 0;
	}

	//获取RTP包负载长度
    int CRTPPacket::GetPayloadSize()const
    {
        return _length;
    }

	//获取RTP包实际数据负载长度，不包括填充字节
    int CRTPPacket::GetPayloadNoPaddingSize()const
    {
    	int PayloadSize = _length;
    	if(GetPadding())
    	{
    		//printf("------------padding len = %d,_wholeLen = %d\n",_buffer[_wholeLen-1],_wholeLen);
			if(_buffer[_wholeLen-1] > 3)
			{
				printf("CRTPPacket::GetPayloadSize() error!!!!!!!!!!!!!!!!!!!!!\n");
			}
			PayloadSize = PayloadSize - _buffer[_wholeLen-1];
    	}
        return PayloadSize;
    }
	//添加负载数据, 传入的指针数据被拷贝组包, 效率要差一点, 但更安全
    int CRTPPacket::AddPayload(const char* data, int len)
    {
        int hdrlen = GetHeaderSize();
        int padLen = len%4;


        if((hdrlen + len) > _wholeLen)
        {
            printf("===RTPPacket=== Add payload error!\n");
            return -1;
        }

        ///重置总长度
        memcpy(_buffer + hdrlen, data, len);
        _wholeLen = hdrlen + len;
        _length = len;

        if (GetPadding() && (0 != padLen))
        {
            //设置Padding
            _wholeLen += padLen;
            _length += padLen;
            memset(_buffer + hdrlen + len, 0, padLen);
            _buffer[_wholeLen - 1] = padLen;
            _buffer[0] |= 0x20;
        }
        return 0;
    }

	//添加负载数据, 传入的指针数据被拷贝组包, 效率要差一点, 但更安全
	int CRTPPacket::AddPayload(char* data, int len)
	{
		if((GetHeaderSize() + len + _interleaved) > _wholeLen)
			return -1;
		memcpy(_buffer + _interleaved + GetHeaderSize(), data, len);
		_length = len;

#if 0
		printf("_length: %d\n", _length);
		for (int i = 0; i < 20; i++)
		{
			if (i%4 == 0)
			{
				printf("\n");
			}
			printf("0x%02x ", (unsigned char)_buffer[i]);
		}
		printf("\n");
#endif

		return 0;
	}


    //读取负载数据, 不包括RTP头数据
    const char* CRTPPacket::ReadPayload(int& len)const
    {
        len = _length;
        return _buffer + _interleaved + GetHeaderSize();
    }

	//读取负载数据, 不包括RTP头数据,不包括填充字节
    const char* CRTPPacket::ReadPayloadNoPadding(int& len)const
    {
		if(GetPadding())
    	{
    		//printf("------------padding len = %d,_wholeLen = %d\n",_buffer[_wholeLen-1],_wholeLen);
			len = _length - _buffer[_wholeLen-1];

		}
		else
		{
			len = _length;
		}
        return _buffer + _interleaved + GetHeaderSize();
    }

	//设置负载缓冲，但不拷贝内存
	//如果需要打包网络化流，则传入指针的生存期必须比该类对象的生存期长，否则会读取垃圾内存
	//如果需要获取网络化流，则后续对负载的拷贝必不可少，因为必须串接头和负载形成比特流
    int CRTPPacket::SetPayloadPtr(const char* data, size_t len)
    {
        size_t padLen = len%4;
        assert(data);
        ///确保原来有内存缓冲，并且缓冲足够大，防止后续打包流出错
        assert(!_bCopy);
        assert(_wholeLen > len + GetHeaderSize() + padLen);

        _bExtPayload = true;
        _extPayload = data;
        _length = len;
        _wholeLen = len + GetHeaderSize();

        ///探知是否需要加填充
        if (padLen > 0)
        {
            _wholeLen += padLen;
        }

        return 0;
    }

	//设置负载缓冲，但不拷贝内存
	//如果需要打包网络化流，则传入指针的生存期必须比该类对象的生存期长，否则会读取垃圾内存
	//如果需要获取网络化流，则后续对负载的拷贝必不可少，因为必须串接头和负载形成比特流
	int CRTPPacket::SetPayloadPtr(char* data, int len)
	{
		if ((len + GetHeaderSize() + _interleaved) >  _wholeLen)
			return -1;

		if (data != NULL)
		{
			if (_buffer != NULL)
				delete _buffer;

			_buffer = data;
		}

		if (len > 0)
			_length = len;

		return 0;
	}

	//得到序列化网络流
    const char* CRTPPacket::GetStream(void)const
    {
        ///如果负载是外部指针，这里就不得不拷贝了！
        if (_bExtPayload)
        {
            int hdrlen = GetHeaderSize();
            memcpy(_buffer + hdrlen, _extPayload, _length);

            ///检查是否要填充位
            int padLen = _length%4;
            if (padLen != 0)
            {
                assert(hdrlen + _length + padLen == _wholeLen);
                memset(_buffer + hdrlen + _length, 0, padLen);
                _buffer[_wholeLen - 1] = padLen;
                _buffer[0] |= 0x20;
            }
        }
        return _buffer;
    }

	//得到序列化网络流
	char* CRTPPacket::GetStream()
	{
		return _buffer;
	}

	//得到序列化网络流长度
    int CRTPPacket::GetStreamLen(void)const
    {
        return _length + GetHeaderSize() + _interleaved;
    }

	//设定RTSP OVER TCP方式下, RTP包通道标识
	int CRTPPacket::AddRtspChn(char chn)
	{
		if (_interleaved < 4)
			return -1;
		_buffer[0] = '$';
		_buffer[1] = chn;
		_buffer[2] = ((_length + GetHeaderSize()) & 0x0000FF00) >> 8;
		_buffer[3] = (_length + GetHeaderSize()) & 0xff;
		printf("len: %d, %d, %d\n", _buffer[2], _buffer[3], _length + GetHeaderSize());

		return 0;
	}
}
