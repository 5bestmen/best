#include "dynamic_factory.h"
#include "dync_moving.h"
#include "dync_color.h"
#include "dync_transparency.h"
#include "dync_visible.h"
#include "dync_value.h"
#include "dync_text.h"


CDynamicFactory::CDynamicFactory()
{
}


CDynamicFactory::~CDynamicFactory()
{
}

CBaseDyncData * CDynamicFactory::CreateDynamicAttr(unsigned int nAttrType)
{
	CBaseDyncData *pNewAttr = nullptr;

	switch (nAttrType)
	{
	case CBaseDyncData::DYNC_FILLING:
	{
		//填充颜色
		pNewAttr = new CDyncFillingData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_FILLING;
		break;
	}
	case CBaseDyncData::DYNC_PEN_COLOR:
	{
		//pen颜色
		pNewAttr = new CDyncClrData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_PEN_COLOR;
		break;
	}
	case CBaseDyncData::DYNC_TEXT_COLOR:
	{
		//textcolor
		pNewAttr = new CDyncClrData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_TEXT_COLOR;
		break;
	}
	case CBaseDyncData::DYNC_HOR_MOVE:
	{
		//水平移动
		pNewAttr = new CDyncMovingData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_HOR_MOVE;

		break;
	}
	case CBaseDyncData::DYNC_VIR_MOVE:
	{
		//竖直移动
		pNewAttr = new CDyncMovingData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_VIR_MOVE;

		break;
	}
	case CBaseDyncData::DYNC_ROTATION:
	{
		//旋转
		pNewAttr = new CDyncMovingData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_ROTATION;
		break;
	}
	case CBaseDyncData::DYNC_TRANSPARENCY:
	{
		//动态透明
		pNewAttr = new CDyncTransparencyData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_TRANSPARENCY;
		break;
	}
	case CBaseDyncData::DYNC_VISIBLE:
	{
		//动态可见
		pNewAttr = new CDyncVisibleData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_VISIBLE;
		break;
	}
	case CBaseDyncData::DYNC_VALUE:
	{
		//动态数值
		pNewAttr = new CDyncValueData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_VALUE;
		break;
	}
	case CBaseDyncData::DYNC_STRING:
	{
		//动态文本
		pNewAttr = new CDyncText;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_STRING;
		break;
	}
	case CBaseDyncData::DYNC_MAP:
	{
		//动态图片
		pNewAttr = new CDyncText;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_MAP;
		break;
	}
	case CBaseDyncData::DYNC_GROUP_MOVE:
	{
		//组合移动
		pNewAttr = new CDyncGroupMovingData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_GROUP_MOVE;

		break;
	}
	default:
		break;
	}

	return pNewAttr;
}
