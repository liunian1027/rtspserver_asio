
#include <stdio.h>
#include "RTPStructs.h"
#include "CRTPPacket.h"

namespace CompLib
{
    ///设定RTP配置的默认值
    void RTPSessConfig::SetDefault()
    {
        basicCfg.frameRate = 25;
        basicCfg.payloadType = DynamicBase;
        basicCfg.pktSize = 1024 + 12;
        basicCfg.sample = 1000;
        basicCfg.ssrc = 11111111;
        basicCfg.startSeq = 0;
        basicCfg.startTs = 0;

        //默认无扩展，不重传,不支持RTCP
        extCfg.hasExtension = false;
        rtcpCfg.enable = false;
        retranCfg.enable = false;

        profileCfg.id = 0;
        profileCfg.maxSendNum = 15;     ///一次最多15个
        profileCfg.maxSendOctets = 32*1024;
        profileCfg.maxSendOctetsThreshold = 40*1024;
    }


	//核对RTP配置的有效性
    bool CheckConfig(RTPSessConfig& cfg)
    {
        if (cfg.basicCfg.frameRate > 30 || cfg.basicCfg.frameRate == 0 )
        {
            //printf("===CheckConfig=== Bad FrameRate!\n");
            return false;
        }

        size_t len = CRTPPacket::MIN_HEAD_LENGTH + cfg.basicCfg.pktSize;
        if(cfg.extCfg.hasExtension)
        {
            if (cfg.extCfg.vecExt.empty())
            {
                printf("===CheckConfig=== Empty extVec!\n");
                return false;
            }
            len += cfg.extCfg.vecExt.size() + 4;
        }


        if (len < 484 || len > 1472)
        {
            printf("===CheckConfig=== Bad fragment size!\n");
            return false;
        }

        if (cfg.retranCfg.enable && (cfg.retranCfg.pktsNum == 0 || cfg.retranCfg.pktsNum > 300))
        {
            printf("===CheckConfig=== Retrans pktNum too large (> 300)!\n");
            return false;
        }

        if (0 == cfg.profileCfg.maxSendNum || cfg.profileCfg.maxSendNum > 15 )
        {
            printf("===CheckConfig=== Max send num beyond (1, 15] is invalid!\n");
            return false;
        }

        if (cfg.profileCfg.maxSendOctets < 20000 || cfg.profileCfg.maxSendOctets > 128000)
        {
            printf("===CheckConfig=== Max send octets limit beyond [20k, 128k] is invalid!\n");
            return false;
        }

        if (cfg.profileCfg.maxSendOctetsThreshold < cfg.profileCfg.maxSendOctets
            || cfg.profileCfg.maxSendOctetsThreshold > 256000)
        {
            printf("===CheckConfig=== Max send octets limit beyond [maxSendOctets, 256k] is invalid!\n");
            return false;
        }

        return true;
    }
}
