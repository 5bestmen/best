
#include "param104.h"


string CParam104::Dump()
{
        char buf[64];
        sprintf(buf,"ParamID:%d AnalogStartAddr:%d BinaryStartAddr:%d,KwhStartAddr:%d,AnalogTelectrlStartAddr:%d,BinaryTelectrlStartAddr:%d",
                m_nParamID,m_nAnalogStartAddr,m_nBinaryStartAddr,m_nKwhStartAddr,m_nAnalogTelectrlStartAddr,m_nBinaryTelectrlStartAddr);

        return buf;
}


