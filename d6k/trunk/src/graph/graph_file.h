/*! @file graph_file.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  graph_file.h
�ļ�ʵ�ֹ��� :  ͼ���ļ�
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief     ͼ���ļ�
*  @author    LiJin
*  @version   1.0
*  @date      2014.09.11
*******************************************************************************/
#ifndef D_GRAPH_FILE_H
#define D_GRAPH_FILE_H
 
//#include "base_widget.h"
#include "graphics_layer.h"
#include "graphbasedata.h"

#include <QSize>
#include <vector>
#include <string>
#include <memory>
#include <array> 
#include <QPainter>
#include <QObject>

class CBaseWidget;
class CGraphFolder;
class CGraphicsLayer;

/*! \class CGraphFileVerInfo  graph_file.h  "graph_file.h"
*  \brief �ļ��İ汾��Ϣ.
*/
class CGraphFileVerInfo 
{
public:
	CGraphFileVerInfo();
	~CGraphFileVerInfo();
public:
	bool SaveFile(QXmlStreamWriter *pWriter) const;

	unsigned int GetMajorVer()const
	{
		return m_nMajorVer;
	}
	unsigned int GetMinorVer()const
	{
		return m_nMinorVer;
	}
	unsigned int GetBuild()const
	{
		return m_nBuild;
	}

	void SetMajorVer(unsigned int nVer)
	{
		m_nMajorVer = nVer;
	}

	void SetMinorVer(unsigned int nVer)
	{
		m_nMinorVer = nVer;
	}

	void SetBuild(unsigned int nVer)
	{
		m_nBuild = nVer;
	}

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
	std::string   m_szCreaterName; //! �ĵ��Ĵ�����
	std::string   m_szEditorName;  //! ��󱣴���û���
};
 

class CBackground;
class CGraphScene;
/*! \class CGraphFile  graph_file.h  "graph_file.h"
*  \brief ͼ���ļ�.
*/
class CGraphFile :public QObject, public CGraphBaseData
{
	Q_OBJECT 
public:
	CGraphFile();
	CGraphFile(CGraphScene *pScene);
	CGraphFile(CGraphFolder *pFolder);

	~CGraphFile();
    //��д��ֵ����  
	const CGraphFile& operator=(const CGraphFile& src);

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
	//
	void SetGraphcsSize(QSize &sizeGraph);
	QRectF GetGraphRect()const;
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

	size_t GetLayerCount()
	{
	//	return MAX_LAYER_CNT;
		return m_arrLayers.size();
	}

	std::shared_ptr<CGraphicsLayer> GetLayer(unsigned int nIdx);

	std::shared_ptr<CGraphicsLayer> GetLayer(unsigned int idx) const;
//	CGraphicsLayer* GetLayer(unsigned int nIdx);
	bool IsLayerVisible(unsigned int nLayerIdx);

	void SetLayerVisible(unsigned int nLayerIdx, bool bDisp, unsigned int & nCurEditLayer);

	std::vector <CBaseWidget *>& GetSelectedWidgets(unsigned int nLayIdx);

	void SelectMultiWidgets(unsigned int nLayerIdx,CBaseWidget *pWidget);

	bool SelectAll(unsigned int nLayerIdx);
	bool SelectNone(unsigned int nLayerIdx);
	bool SelectWidget(unsigned int nLayerIdx, CBaseWidget *pWidget);
	void SelectWidgetsInRect(unsigned int nLayerIdx, const QRectF &rcSel);
	bool ReSelectWidget(unsigned int nLayerIdx, CBaseWidget *pWidget);
	void Deselect(unsigned int nLayerIdx, CBaseWidget* pObj);
	
	CBaseWidget *GetCurrentWidget(unsigned int nLayerIdx);
	void SetCurrentWidget(unsigned int nLayerIdx, CBaseWidget*pWidget);

	std::string GetGraphName()const
	{
		return m_szGraphName;
	}
	void SetGraphName(const std::string & szName)
	{
		m_szGraphName = szName;
	}
	CGraphFolder *GetFolder()
	{
		return m_pFolder;
	}
	void SetScene(CGraphScene *pScene)
	{
		m_pScene = pScene;
		for (auto i:m_arrLayers)
		{
			i->SetScene(pScene);
		}
	}
	//�������
	void ClearLayer();

	CGraphScene * GetScene()
	{
		return m_pScene;
	}

	//����ͼ���ļ�
	bool SaveFile(const QString &szFilePath) ;
	//����ͼ���ļ�
	bool LoadFile(const QString &szFileName);

     CGraphFileVerInfo *GetFileVerInfo() 
	{
		return &m_FileInfo;
	}
// 	const CGraphFileVerInfo& GetFileVerInfo() const
// 	{
// 		return m_FileInfo;
// 	}

	std::pair<int, int> GetSize()
	{
		return std::make_pair(m_Size.height(), m_Size.width());
	}

	
	std::string & GetDescript()
	{
		return m_szDescript;
	}

	void SetFlashFlag(bool bFlag)
	{
		m_bFlashFlag = bFlag;
	}

	bool GetFlashFlag()
	{
		return m_bFlashFlag;
	}

	std::shared_ptr<CBackground> GetBackground()
	{
		return m_pBackground;
	}
	QRectF GetNewRect(const QRectF& rect1,const QRectF& rect2);

protected Q_SLOTS:
	void slot_on_updateScene(CBaseWidget*,const QRectF& rect, const QRectF& rect1);
public:
	//! �������� �������ظ����ļ������ó��� �ֽڳ���
	std::string m_szGraphName;
	//! ����
	std::string m_szDescript;
	//! ͼ��
	std::array<std::shared_ptr<CGraphicsLayer>, MAX_LAYER_CNT> m_arrLayers;
//	CGraphicsLayer* m_arrLayers[MAX_LAYER_CNT];
	//ͼԪ����
	void DownItemLayer(unsigned int nLayer);
	//ͼԪ����
	void UploadItemLayer(unsigned int nLayer);
	//��Ч��ʶ
	bool m_bVaildFlag;

	//�������ű���
	void SetScaled(int nScale)
	{
		m_scaleFactor = nScale;
	}

	//��ȡ���ű���
	int GetSacled()
	{
		return m_scaleFactor;
	}

	//����
	void RenameWidgetItem(unsigned int nLayerIdx, CBaseWidget *pObj);

private:
	//! �ļ��İ汾��Ϣ
	CGraphFileVerInfo m_FileInfo;
	//! ͼԪ�������
	unsigned int m_nMaxWidgetID;
	//! ����Ŀ¼�����Ϊ������root�����
	CGraphFolder *m_pFolder;
	//! ͼ���ļ��ĳߴ磬ÿ��ͼ�εĳߴ���Բ�һ��
	QSize  m_Size;

	//! ˢ������ ����Ϊ��λ
	unsigned int m_nPeriod;

	std::shared_ptr<CBackground> m_pBackground;

	CGraphScene *m_pScene;
	//! ��ǰ���ڱ༭��ͼ����
//	unsigned int m_nCurEditingLayer;
//	std::vector <CBaseWidget*> m_arrWidgets;
	//�Ƿ�ˢ�±�־
	bool m_bFlashFlag;
	//���ű���
	int m_scaleFactor;

};

#endif // D_GRAPH_FILE_H

/** @}*/

