/*! @file configinfo.h
<PRE>
********************************************************************************
模块名       :
文件名       :  configinfo.h
文件实现功能 :  数据结构定义
作者         :  xingzhibing
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   数据结构定义
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
	ChartType = 1,    //曲线
	BarType   = 2,    //棒状图
	PieType   = 3,    //饼状图
	UNKNOWN_TYPE=4,   //未知状态
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

//基本记录数据单元(1条记录)
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
	QString m_TagName;                             //数据标志
	QString m_szName;                              //显示名称
	QVector< CPoint > m_vecValue;                  //存储数据
	float m_TotalXVal;
	float m_TotalYVal;
	QColor m_bgColor;                              //颜色控制
	QFont  m_Font;                                 //显示字体
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

//单个控件
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

	//数值设定
	typedef QVector< CCtrlGernalValue* > CVecCtrlData;

	void SetCtrlData(CVecCtrlData& vecCtrlData)
	{
		m_VecValue = vecCtrlData;
	}
	CVecCtrlData& GetCtrlVecData()
	{
		return m_VecValue;
	}
	//X 数值名称设定
	void SetXaisContent(const QString& szList)
	{
		m_szXais = szList;
	}
	QString& GetXaisContent()
	{
		return m_szXais;
	}

	//Y 数值名称设定
	void SetYaisContent(const QString& szName)
	{
		m_szYais = szName;
	}
	QString& GetYasiContent()
	{
		return m_szYais;
	}
	//报表数值信息
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
	//!行列标识符
	int m_nRowIndex;
	int m_nColIndex;
	//!控件唯一标识
	QString m_TagName;

	QString m_ChartName;
	//!控件类型
	CtrlType m_CtrlType;
	//!数据映射
	CVecCtrlData m_VecValue;
	//！X轴文本
	QString m_szXais;
	//！Y轴文本
	QString m_szYais;

	QString m_szData;
};


//报表存储基本合并信息
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

//报表存储基本单元格信息
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
	//行列索引
	int m_nRowIndex;
	int m_nColIndex;
	//表格格式
	QColor m_CellBgColor;                   //单元格色彩记录
	//字体格式
	int m_TextAlign;                        //对齐方式
	QColor m_TextColor;                     //字体颜色
	QFont  m_TextFont;                      //字体设置，（大小size，粗体，斜体，字体族）
	//表格内容
	QString m_szDataContent;                //表格内容
	CMergeInfo m_MergeRecoder;              //合并信息
};

//报表基本信息
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
	QString m_szTableName;                   //报表名称
	int m_nTableType;                        //报表类型
	QString m_szTablePath;                   //报表文件路径
	int m_RowCount;                          //报表行数
	int m_ColCount;                          //报表列数
};

//报表存储信息
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
	int m_nRowHeigth;                        //报表行高
	int m_nColumnWidth;                      //报表列宽
	QMap< int, float > m_RowHeight;
	QMap< int, float > m_ColWidth;
	QList< CMergeInfo > m_MergeItemList;     //合并单元
};

//报表文件
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

	//! 设置报表合法名称
	void SetRptFileName(const QString& szFileName)
	{
		//1<名称修改
		if (QFileInfo(szFileName).suffix()!="xml")
		{
			m_pTableInfo->m_szTableName = szFileName+".xml";
		}
		else
		{
			m_pTableInfo->m_szTableName = szFileName;
		}	
		//2<路径修改
		QString szPath = QFileInfo(m_pTableInfo->m_szTablePath).absolutePath();

		QFile::rename(QString(m_pTableInfo->m_szTablePath).toStdString().c_str(), QString(szPath + '/' + m_pTableInfo->m_szTableName).toStdString().c_str());
	
		//3<路径设置
		m_pTableInfo->m_szTablePath = QString(szPath + '/' + m_pTableInfo->m_szTableName);
	}

	//! 获取报表名称
	QString GetRptFileName()
	{
		return m_pTableInfo->m_szTableName;
	}

	//! 获取报表基本信息
	CTableInfo* GetRptInfo() 
	{
		return m_pTableInfo;
	}

	typedef QList< CCellData* >   CCellList;
	typedef QList< CCtrlData* >   CCtrlList;
	//检查命名合法性问题
	bool CheckName(const QString & szName);
	//设置报表信息
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
	CTableInfo* m_pTableInfo;       //! 基本信息
	CCellList  m_lstCells;          //! 单元格信息
	CCtrlList  m_lstCtrls;          //! 控件信息
	bool m_bDirtyFlag;              //! true 为修改过，false为没有修改过
};


//报表组管理
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

	//设置报表组名称
	void SetGrpName(const QString& szName)
	{
		m_szGroupName = szName;
	}

	//获取报表组名称
	QString &GetGrpName()
	{
		return m_szGroupName;
	}

	//获取报表文件数目
	int GetRptFileCount()
	{
		return m_arrFiles.size();
	}

	//获取报表组数目
	int GetRptGrpCount()
	{
		return m_arrGrps.size();
	}
	//检查文件名称合法性
	bool CheckName(const QString & szName);

	//获取报表文件整体集合
	void GetArryFiles( QVector<CRptFile*>& vecRptFile)
	{
		vecRptFile = m_arrFiles;
	}

	//获取报表组集合
	void GetArryGrps(QVector<CRptFileGrp*>& vecRptGrps)
	{
		vecRptGrps = m_arrGrps;
	}

	//新建报表
	CTableInfo* CreateRptFile(const QString& szName);
	//新增报表文件
	bool AddRptFile(CRptFile* nRptInfo);
	bool AddRptGrp(CRptFileGrp* pRptGrp);
	//删除指定报表文件
	bool DelRptFile(CTableInfo* nRptInfo);
	bool DelRptFile(const QString& szName);

	//保存报表文件
	bool SaveFiles(const QString& szRootPath) const ;

	/*! \fn bool GetModifyFlag()
	********************************************************************************************************* 
	** \brief CRptFileGrp::GetModifyFlag 
	** \details 只要有一个文件被修改过，即认为该模块的内容被修改过
	** \return bool 
	** \author LiJin 
	** \date 2016年8月18日 
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


//报表统筹管理
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
	//新建报表
	CRptFile* CreateRptFile(CTableInfo* tableInfo);

	//删除控件
	bool DelCtrlFile(CRptFile* pRptFile,CCtrlData* pData);

	//新增报表文件
	bool AddRptFile(CRptFile* nRptInfo);

	//删除指定报表文件
	bool DelRptFile(CRptFile* pRptFile);

	//获取指定文件或者文件夹
	CRptFileGrp *GetRptGrp(const QString& szName);
	CRptFile* GetRptFile(const QString& szName);

	//保存报表文件
	bool SaveFiles();

	//获取问价夹数目
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
	** \details 只要有一个文件被修改过，即认为该模块的内容被修改过
	** \return bool
	** \author LiJin
	** \date 2016年8月18日
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
	//报表文件集合
	QVector<CRptFile*> m_arrFiles;

	//报表组文件集合
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