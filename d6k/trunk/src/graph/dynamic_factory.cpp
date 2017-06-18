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
		//�����ɫ
		pNewAttr = new CDyncFillingData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_FILLING;
		break;
	}
	case CBaseDyncData::DYNC_PEN_COLOR:
	{
		//pen��ɫ
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
		//ˮƽ�ƶ�
		pNewAttr = new CDyncMovingData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_HOR_MOVE;

		break;
	}
	case CBaseDyncData::DYNC_VIR_MOVE:
	{
		//��ֱ�ƶ�
		pNewAttr = new CDyncMovingData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_VIR_MOVE;

		break;
	}
	case CBaseDyncData::DYNC_ROTATION:
	{
		//��ת
		pNewAttr = new CDyncMovingData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_ROTATION;
		break;
	}
	case CBaseDyncData::DYNC_TRANSPARENCY:
	{
		//��̬͸��
		pNewAttr = new CDyncTransparencyData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_TRANSPARENCY;
		break;
	}
	case CBaseDyncData::DYNC_VISIBLE:
	{
		//��̬�ɼ�
		pNewAttr = new CDyncVisibleData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_VISIBLE;
		break;
	}
	case CBaseDyncData::DYNC_VALUE:
	{
		//��̬��ֵ
		pNewAttr = new CDyncValueData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_VALUE;
		break;
	}
	case CBaseDyncData::DYNC_STRING:
	{
		//��̬�ı�
		pNewAttr = new CDyncText;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_STRING;
		break;
	}
	case CBaseDyncData::DYNC_MAP:
	{
		//��̬ͼƬ
		pNewAttr = new CDyncText;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_MAP;
		break;
	}
	case CBaseDyncData::DYNC_GROUP_MOVE:
	{
		//����ƶ�
		pNewAttr = new CDyncGroupMovingData;
		pNewAttr->m_nDyncType = CBaseDyncData::DYNC_GROUP_MOVE;

		break;
	}
	default:
		break;
	}

	return pNewAttr;
}
