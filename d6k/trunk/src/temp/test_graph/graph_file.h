/*! @file graph_file.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       : graph_file.h
�ļ�ʵ�ֹ��� : ͼ���ļ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief  ��������
*  @author LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/
#ifndef D_GRAPH_FILE_H
#define D_GRAPH_FILE_H
 
//#include "base_widget.h"

#include "QSize"
#include <vector>
#include <string>
#include <memory>
#include <array> 

#include <QPainter>

#include "graphics_layer.h"

class CBaseWidget;
class CGraphFolder;
class CGraphicsLayer;


/*! \class CGraphFileVerInfo  graph_file.h  "graph_file.h"
*  \brief �ļ��İ汾��Ϣ.
*/
class CGraphFileVerInfo
{
public:
	CGraphFileVerInfo(){};
	~CGraphFileVerInfo(){};
private:
	unsigned int  m_nMajorVer; //���汾��  �磺1.00 :wMajorVer = 1 wMinorVer= 0��2.01 = wMajorVer = 2 wMinorVer= 1 ���汾�ź�Ӧ�ó����й� 
	unsigned int  m_nMinorVer; //�Ӱ汾�ţ�
	unsigned int  m_nBuild;    //���ļ�����ʱ������ÿ����һ�Σ��汾������1	

	unsigned int  m_nYear;
	unsigned int  m_nMonth;
	unsigned int  m_nDay;
	unsigned int  m_nHour;
	unsigned int  m_nMinute;
	unsigned int  m_nSecond;
	unsigned int  m_nMilliseconds;
	std::string   m_szEditorName;  //! ��󱣴���û���
};
 

class CBackground;
class CGraphScene;
/*! \class CGraphFile  graph_file.h  "graph_file.h"
*  \brief ͼ���ļ�.
*/
class CGraphFile
{
public:
	CGraphFile();
	CGraphFile(CGraphScene *pScene);
	CGraphFile(CGraphFolder *pFolder);

	~CGraphFile();
public:
	enum LayerNumber
	{
		MAX_LAYER_CNT = 16  //!< ���ͼ��������ͼ���˳��Ŵ�0��ʼ
	};
	void SetPeriod(unsigned int nPeriod)
	{
		if (nPeriod < 200)
		{
			nPeriod = 200;
		}
		m_nPeriod = nPeriod;
	}

	void SetGraphSize(QSize &sizeGraph);
	// ���һ������
	void AddWidget(unsigned int nLayerIdx, CBaseWidget *pObj);
	// ��ͼ����ɾ��һ��ָ�����󣬲�������Դ
	bool EraseWidget(unsigned int nLayerIdx, CBaseWidget *pObj);
	// ��ͼ����ɾ��һ��ָ�����󣬲�������Դ
	bool RemoveWidget(unsigned int nLayerIdx, CBaseWidget *pObj);
	// ��ȡ�����ı����ߴ�
	QRectF& GetBackgroundRect() const;
	// ���Ʊ���
	void DrawBackground(QPainter *painter, const QRectF &rect);
	
	void DrawMovingWidgets(QPainter *pPainter,unsigned int nEditingLayerIdx);
	//
	bool IsWidgetsCanMove(const QPointF & delta, const std::vector <CBaseWidget *>& arr) const;

	unsigned int GetLayerCount()
	{
	//	return MAX_LAYER_CNT;
		return m_arrLayers.size();
	}

	std::shared_ptr<CGraphicsLayer> GetLayer(unsigned int nIdx);
//	CGraphicsLayer* GetLayer(unsigned int nIdx);
	bool IsLayerVisible(unsigned int nLayerIdx);

	void SetLayerVisible(unsigned int nLayerIdx, bool bDisp, unsigned int & nCurEditLayer);

	std::vector <CBaseWidget *> GetSelectedWidgets(unsigned int nLayIdx);

	void SelectMultiWidgets(unsigned int nLayerIdx,CBaseWidget *pWidget);

	bool SelectAll(unsigned int nLayerIdx);
	bool SelectNone(unsigned int nLayerIdx);
	bool SelectWidget(unsigned int nLayerIdx, CBaseWidget *pWidget);
	void SelectWidgetsInRect(unsigned int nLayerIdx, const QRectF &rcSel);
	bool ReSelectWidget(unsigned int nLayerIdx, CBaseWidget *pWidget);
	void Deselect(unsigned int nLayerIdx, CBaseWidget* pObj);


	CBaseWidget *GetCurrentWidget(unsigned int nLayerIdx);
	void SetCurrentWidget(unsigned int nLayerIdx, CBaseWidget*pWidget);
public:
	// �������� �������ظ����ļ������ó��� �ֽڳ���
	std::string m_szGraphName;
	// ����
	std::string m_szDescript;
	//! ͼ��
	std::array<std::shared_ptr<CGraphicsLayer>, MAX_LAYER_CNT> m_arrLayers;
//	CGraphicsLayer* m_arrLayers[MAX_LAYER_CNT];
private:
	//! ͼ���ļ��ĳߴ磬ÿ��ͼ�εĳߴ���Բ�һ��
	QSize  m_Size;
	//! �ļ��İ汾��Ϣ
	CGraphFileVerInfo m_FileInfo;
	//! ͼԪ�������
	unsigned int m_nMaxWidgetID;
	//! ����Ŀ¼�����Ϊ������root�����
	CGraphFolder *m_pFolder;

	//! ˢ������ ����Ϊ��λ
	unsigned int m_nPeriod;

	std::shared_ptr<CBackground> m_pBackground;

	CGraphScene *m_pScene;
	//! ��ǰ���ڱ༭��ͼ����
//	unsigned int m_nCurEditingLayer;
//	std::vector <CBaseWidget*> m_arrWidgets;
};

#endif // D_GRAPH_FILE_H

/** @}*/

