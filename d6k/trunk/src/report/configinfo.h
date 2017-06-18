/*! @file configinfo.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  configinfo.h
�ļ�ʵ�ֹ��� :  ���ݽṹ����
����         :  xingzhibing
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���ݽṹ����
*  @author  xingzhibing
*  @version 1.0
*  @date    2016.08.31
*******************************************************************************/

#ifndef _CONFIG_INFO_H
#define _CONFIG_INFO_H

#include <QDataStream>
#include <QString>
#include <QObject>
#include <QVector>
#include <QColor>
#include <QMap>
#include <QFont>
#include <QFileInfo>


enum CtrlType
{
	ChartType = 1,    //����
	BarType   = 2,    //��״ͼ
	PieType   = 3,    //��״ͼ
	UNKNOWN_TYPE=4,   //δ֪״̬
};

class CPoint
{
public:
	CPoint(float xais, float yais)
	{
		m_fXais = xais;
		m_fYais = yais;
	}
	CPoint(){};
	~CPoint(){};
public:
	float m_fXais;
	float m_fYais;
};

//������¼���ݵ�Ԫ(1����¼)
class CCtrlGernalValue
{
public:
	CCtrlGernalValue()
	{
		m_TagName = "";
		m_szName = "";
		m_TotalXVal = 0.0;
		m_TotalYVal = 0.0;
	}
public:
	QString m_TagName;                             //���ݱ�־
	QString m_szName;                              //��ʾ����
	QVector< CPoint > m_vecValue;                  //�洢����
	float m_TotalXVal;
	float m_TotalYVal;
	QColor m_bgColor;                              //��ɫ����
	QFont  m_Font;                                 //��ʾ����
public:
	float GetXSumData()
	{
		m_TotalXVal = 0.0;
		for (auto i:m_vecValue)
		{
			m_TotalXVal += i.m_fXais;
		}
		return m_TotalXVal;
	}
	float GetYSumData()
	{
		m_TotalYVal = 0.0;
		for (auto i : m_vecValue)
		{
			m_TotalYVal += i.m_fYais;
		}
		return m_TotalYVal;
	}
};

//�����ؼ�
class CCtrlData
{
public:
	CCtrlData(){};
	~CCtrlData()
	{
		qDeleteAll(m_VecValue);
		m_VecValue.clear();
	}
public:
	void SetCtrlPosition(const int& nRowIndex, const int& nColIndex)
	{
		m_nRowIndex = nRowIndex;
		m_nColIndex = nColIndex;
	}
	int  GetCtrlCount()
	{
		return m_VecValue.size();
	}
	void SetCtrlType(const CtrlType& enumType)
	{
		m_CtrlType = enumType;
	}
	CtrlType GetCtrlType()
	{
		return m_CtrlType;
	}
	bool AddCtrlValue(CCtrlGernalValue* pCtrlValue);

	bool RemoveCtrlVaule(CCtrlGernalValue* pCtrlValue);

	QString& GetCtrlTagName()
	{
		return m_TagName;
	}
	void SetCtrlTagName(const QString& tagName)
	{
		m_TagName = tagName;
	}

	QString& GetCtrlName()
	{
		return m_ChartName;
	}
	void SetCtrlName(const QString& tagName)
	{
		m_ChartName = tagName;
	}

	//��ֵ�趨
	typedef QVector< CCtrlGernalValue* > CVecCtrlData;

	void SetCtrlData(CVecCtrlData& vecCtrlData)
	{
		m_VecValue = vecCtrlData;
	}
	CVecCtrlData& GetCtrlVecData()
	{
		return m_VecValue;
	}
	//X ��ֵ�����趨
	void SetXaisContent(const QString& szList)
	{
		m_szXais = szList;
	}
	QString& GetXaisContent()
	{
		return m_szXais;
	}

	//Y ��ֵ�����趨
	void SetYaisContent(const QString& szName)
	{
		m_szYais = szName;
	}
	QString& GetYasiContent()
	{
		return m_szYais;
	}
	//������ֵ��Ϣ
	void SetRptCtrlData(const QString& szData)
	{
		m_szData = szData;
	}
	QString& GetRptCrtlData()
	{
		return m_szData;
	}
	int GetRowIndex()
	{
		return m_nRowIndex;
	}
	int GetColIndex()
	{
		return m_nColIndex;
	}
	
private:	
	//!���б�ʶ��
	int m_nRowIndex;
	int m_nColIndex;
	//!�ؼ�Ψһ��ʶ
	QString m_TagName;

	QString m_ChartName;
	//!�ؼ�����
	CtrlType m_CtrlType;
	//!����ӳ��
	CVecCtrlData m_VecValue;
	//��X���ı�
	QString m_szXais;
	//��Y���ı�
	QString m_szYais;

	QString m_szData;
};


//����洢�����ϲ���Ϣ
class CMergeInfo
{
public:
	CMergeInfo()
	{
		m_nRow      = 0;
		m_nColumn   = 0;
		m_nRowCount = 0;
		m_nColCount = 0;
	};
	CMergeInfo(int nRow, int nCol, int rowCount, int colCount)
	{
		m_nRow      = 0;
		m_nColumn   = 0;
		m_nRowCount = 0;
		m_nColCount = 0;
		m_nRow    = nRow;
		m_nColumn = nCol;
		m_nRowCount = rowCount;
		m_nColCount = colCount;
	};

	CMergeInfo(const CMergeInfo& rhs)
	{
		m_nRow = rhs.m_nRow;
		m_nColumn = rhs.m_nColumn;
		m_nRowCount = rhs.m_nRowCount;
		m_nColCount = rhs.m_nColCount;
	}
public:
	int m_nRow;
	int m_nColumn;
	int m_nRowCount;
	int m_nColCount;
};

//����洢������Ԫ����Ϣ
class  CCellData
{
public:
	CCellData()
	{
		m_CellBgColor = QColor(255, 255, 255);
		m_TextAlign = 0x84;
		m_TextColor = QColor(0, 0, 0);
		m_szDataContent = "";
	};
	CCellData(const CCellData & rhs)
	{
		m_nRowIndex = rhs.m_nRowIndex;
		m_nColIndex = rhs.m_nColIndex;
		m_CellBgColor = rhs.m_CellBgColor;
		m_TextAlign = rhs.m_TextAlign;
		m_TextColor = rhs.m_TextColor;
		m_TextFont = rhs.m_TextFont;
		m_szDataContent = rhs.m_szDataContent;
		m_MergeRecoder = rhs.m_MergeRecoder;
	}
public:
	//��������
	int m_nRowIndex;
	int m_nColIndex;
	//����ʽ
	QColor m_CellBgColor;                   //��Ԫ��ɫ�ʼ�¼
	//�����ʽ
	int m_TextAlign;                        //���뷽ʽ
	QColor m_TextColor;                     //������ɫ
	QFont  m_TextFont;                      //�������ã�����Сsize�����壬б�壬�����壩
	//�������
	QString m_szDataContent;                //�������
	CMergeInfo m_MergeRecoder;              //�ϲ���Ϣ
};

//���������Ϣ
class CTableGerenalInfo
{
public:
	CTableGerenalInfo()
	{
		m_szTableName = "";
		m_nTableType = 0;
		m_szTablePath = "";
		m_RowCount = 0;
		m_ColCount = 0;
	}
	~CTableGerenalInfo()
	{
	
	}
public:
	QString m_szTableName;                   //��������
	int m_nTableType;                        //��������
	QString m_szTablePath;                   //�����ļ�·��
	int m_RowCount;                          //��������
	int m_ColCount;                          //��������
};

//����洢��Ϣ
class CTableInfo:public CTableGerenalInfo
{
public:
	CTableInfo()
	{
		m_nRowHeigth    = 0;
		m_nColumnWidth  = 0;
		m_RowHeight.clear();
		m_ColWidth.clear();
		m_MergeItemList.clear();
	}
	~CTableInfo()
	{
		m_RowHeight.clear();
		m_ColWidth.clear();
		m_MergeItemList.clear();
	}
public:
	int m_nRowHeigth;                        //�����и�
	int m_nColumnWidth;                      //�����п�
	QMap< int, float > m_RowHeight;
	QMap< int, float > m_ColWidth;
	QList< CMergeInfo > m_MergeItemList;     //�ϲ���Ԫ
};

//�����ļ�
class CRptFile
{
public:
	CRptFile()
	{
		m_bDirtyFlag = false;
		m_bLoaded = false;
		m_pTableInfo = Q_NULLPTR;
	}
	~CRptFile()
	{
		qDeleteAll(m_lstCells);
		m_lstCells.clear();

		qDeleteAll(m_lstCtrls);
		m_lstCtrls.clear();

		if (m_pTableInfo)
		{
			delete m_pTableInfo;
			m_pTableInfo = Q_NULLPTR;
		}
	}
	QString GetRecommendCtrlName();

	int GetRptCellCount()const
	{
		return m_lstCells.size();
	}
	CCtrlData* CreateCtrlData(const QString& tagName);

	void DelCtrlData(CCtrlData* pData);

	//! ���ñ���Ϸ�����
	void SetRptFileName(const QString& szFileName)
	{
		//1<�����޸�
		if (QFileInfo(szFileName).suffix()!="xml")
		{
			m_pTableInfo->m_szTableName = szFileName+".xml";
		}
		else
		{
			m_pTableInfo->m_szTableName = szFileName;
		}	
		//2<·���޸�
		QString szPath = QFileInfo(m_pTableInfo->m_szTablePath).absolutePath();

		QFile::rename(QString(m_pTableInfo->m_szTablePath).toStdString().c_str(), QString(szPath + '/' + m_pTableInfo->m_szTableName).toStdString().c_str());
	
		//3<·������
		m_pTableInfo->m_szTablePath = QString(szPath + '/' + m_pTableInfo->m_szTableName);
	}

	//! ��ȡ��������
	QString GetRptFileName()
	{
		return m_pTableInfo->m_szTableName;
	}

	//! ��ȡ���������Ϣ
	CTableInfo* GetRptInfo() 
	{
		return m_pTableInfo;
	}

	typedef QList< CCellData* >   CCellList;
	typedef QList< CCtrlData* >   CCtrlList;
	//��������Ϸ�������
	bool CheckName(const QString & szName);
	//���ñ�����Ϣ
	bool SetRptInfo(CTableInfo Info, CCellList nList);
	bool SetRptBaseInfo(CTableInfo* Info);

	CCellList& GetCellList()
	{
		return m_lstCells;
	}
	CCtrlList& GetCtrlList()
	{
		return m_lstCtrls;
	}
	void SetCellList(CCellList& lstCellData)
	{
		m_lstCells = lstCellData;
	}
	void SetCtrlList(CCtrlList& lstCtrlData)
	{
		m_lstCtrls = lstCtrlData;
	}
public:
	bool WriteXmlFile() ;
	bool ReadXmlFile() ;

	bool GetModifyFlag()
	{
		return m_bDirtyFlag;
	}
	void SetModifyFlag(bool bFlag)
	{
		m_bDirtyFlag = bFlag;
	}
	bool  m_bLoaded;

private:
	CTableInfo* m_pTableInfo;       //! ������Ϣ
	CCellList  m_lstCells;          //! ��Ԫ����Ϣ
	CCtrlList  m_lstCtrls;          //! �ؼ���Ϣ
	bool m_bDirtyFlag;              //! true Ϊ�޸Ĺ���falseΪû���޸Ĺ�
};


//���������
class CRptFileGrp
{
public:
	CRptFileGrp()
	{
	};
	~CRptFileGrp()
	{
		m_arrFiles.clear();
		m_arrGrps.clear();
	};
public:

	//���ñ���������
	void SetGrpName(const QString& szName)
	{
		m_szGroupName = szName;
	}

	//��ȡ����������
	QString &GetGrpName()
	{
		return m_szGroupName;
	}

	//��ȡ�����ļ���Ŀ
	int GetRptFileCount()
	{
		return m_arrFiles.size();
	}

	//��ȡ��������Ŀ
	int GetRptGrpCount()
	{
		return m_arrGrps.size();
	}
	//����ļ����ƺϷ���
	bool CheckName(const QString & szName);

	//��ȡ�����ļ����弯��
	void GetArryFiles( QVector<CRptFile*>& vecRptFile)
	{
		vecRptFile = m_arrFiles;
	}

	//��ȡ�����鼯��
	void GetArryGrps(QVector<CRptFileGrp*>& vecRptGrps)
	{
		vecRptGrps = m_arrGrps;
	}

	//�½�����
	CTableInfo* CreateRptFile(const QString& szName);
	//���������ļ�
	bool AddRptFile(CRptFile* nRptInfo);
	bool AddRptGrp(CRptFileGrp* pRptGrp);
	//ɾ��ָ�������ļ�
	bool DelRptFile(CTableInfo* nRptInfo);
	bool DelRptFile(const QString& szName);

	//���汨���ļ�
	bool SaveFiles(const QString& szRootPath) const ;

	/*! \fn bool GetModifyFlag()
	********************************************************************************************************* 
	** \brief CRptFileGrp::GetModifyFlag 
	** \details ֻҪ��һ���ļ����޸Ĺ�������Ϊ��ģ������ݱ��޸Ĺ�
	** \return bool 
	** \author LiJin 
	** \date 2016��8��18�� 
	** \note 
	********************************************************************************************************/
	bool GetModifyFlag()
	{
		for (auto it : m_arrFiles)
		{
			if (it->GetModifyFlag() == true)
			{
				return true;
			}
		}
		return false;
	}

private:
	QString m_szGroupName;	
	QVector<CRptFile*> m_arrFiles;
	QVector<CRptFileGrp*> m_arrGrps;
};


//����ͳ�����
class CRptFileMgr
{
	enum
	{
		MAX_NAME_LEN  = 128,
		MAX_RPORT_CNT = 2048,
	};
public:
	CRptFileMgr(){};
	~CRptFileMgr();
public:
	

	QString GetRecommandRptGrpName();
	bool IsRptGrpNameValid(QString& szName);

	bool CheckName(const QString & szName);

	CRptFileGrp* CreateRptGrp(const QString& szName);
	//�½�����
	CRptFile* CreateRptFile(CTableInfo* tableInfo);

	//ɾ���ؼ�
	bool DelCtrlFile(CRptFile* pRptFile,CCtrlData* pData);

	//���������ļ�
	bool AddRptFile(CRptFile* nRptInfo);

	//ɾ��ָ�������ļ�
	bool DelRptFile(CRptFile* pRptFile);

	//��ȡָ���ļ������ļ���
	CRptFileGrp *GetRptGrp(const QString& szName);
	CRptFile* GetRptFile(const QString& szName);

	//���汨���ļ�
	bool SaveFiles();

	//��ȡ�ʼۼ���Ŀ
	int GetRptGrpNum()
	{
		return m_arrGrps.size();
	}
	int GetRptFileNum()
	{
		return m_arrFiles.size();
	}

	void Close();

//	void LogMsg(const char * szTxt, int nLevel);

	/*! \fn bool GetModifyFlag()
	*********************************************************************************************************
	** \brief CRptFileGrp::GetModifyFlag
	** \details ֻҪ��һ���ļ����޸Ĺ�������Ϊ��ģ������ݱ��޸Ĺ�
	** \return bool
	** \author LiJin
	** \date 2016��8��18��
	** \note
	********************************************************************************************************/
	bool GetModifyFlag()
	{
		for (auto it:  m_arrFiles )
		{
			if (it->GetModifyFlag() == true)
			{
				return true;
			}
		}

		for (auto it_grp : m_arrGrps)
		{
			if (it_grp->GetModifyFlag() == true)
			{
				return true;
			}
		}

		return false;
	}
private:
	//�����ļ�����
	QVector<CRptFile*> m_arrFiles;

	//�������ļ�����
	QVector<CRptFileGrp*> m_arrGrps;
};





Q_DECLARE_METATYPE(CTableInfo);

inline QDataStream &operator<<(QDataStream& in, const CMergeInfo &recoder)
{
	in << recoder.m_nRow << recoder.m_nColumn << recoder.m_nRowCount << recoder.m_nColCount;
	return in;
}
inline QDataStream & operator<<(QDataStream& in, const  CCellData& str)
{
	in << str.m_CellBgColor << str.m_TextAlign << str.m_TextColor << str.m_TextFont << str.m_szDataContent;
	return in;
};
inline QDataStream &operator>>(QDataStream& out, CMergeInfo& recoder)
{
	out >> recoder.m_nRow >> recoder.m_nColumn >> recoder.m_nRowCount >> recoder.m_nColCount;
	return out;
}
inline QDataStream & operator>>(QDataStream& out, CCellData& str)
{
	out >> str.m_CellBgColor >> str.m_TextAlign >> str.m_TextColor >> str.m_TextFont >> str.m_szDataContent;
	return out;
}
inline QDataStream& operator<<(QDataStream &in, const CTableInfo& recoder)
{
	in << recoder.m_szTableName << recoder.m_nTableType << recoder.m_RowCount << recoder.m_ColCount << recoder.m_nRowHeigth << recoder.m_nColumnWidth << recoder.m_MergeItemList;
	return in;
}
inline QDataStream& operator>>(QDataStream &out, CTableInfo& recoder)
{
	out >> recoder.m_szTableName >> recoder.m_nTableType >> recoder.m_RowCount >> recoder.m_ColCount >> recoder.m_nRowHeigth >> recoder.m_nColumnWidth >> recoder.m_MergeItemList;
	return out;
}
#endif