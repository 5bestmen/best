#include <QFile>
#include <QFileInfo> 
#include <QMessageBox>
#include <QApplication>
#include "configinfo.h"
#include "qxmlfunction.h"

/*! \fn bool CRptFileMgr::CheckName(const QString & szName)
********************************************************************************************************* 
** \brief CRptFileMgr::CheckName 
** \details 文件命名合法性
** \param szName 
** \return bool 
** \author xingzhibing
** \date 2016年8月13日 
** \note 
********************************************************************************************************/
bool CRptFileMgr::CheckName(const QString & szName)
{
	return true;
}

/*! \fn CTableInfo* CRptFileMgr::CreateRptFile(const QString& szName)
********************************************************************************************************* 
** \brief CRptFileMgr::CreateRptFile 
** \details 创建报表文件
** \param szName 
** \return CTableInfo* 
** \author xingzhibing
** \date 2016年8月13日 
** \note 
********************************************************************************************************/
CRptFile* CRptFileMgr::CreateRptFile(CTableInfo*  pTableInfo)
{
	Q_ASSERT(pTableInfo);

	if (pTableInfo->m_szTableName.isEmpty())
	{
		return Q_NULLPTR;
	}

	//判断是否有文件存在，全路径名称判断，名称不重复
	for (auto it:m_arrFiles)
	{
		if (pTableInfo->m_szTableName.compare(it->GetRptFileName()) == 0)
		{
			Q_ASSERT(false);
			return Q_NULLPTR;
		}
	}

	//判断是否有文件存在，全路径判断，路径不重复
	for (auto it : m_arrFiles)
	{
		if (pTableInfo->m_szTablePath.compare(it->GetRptFileName()) == 0)
		{
			Q_ASSERT(false);
			return Q_NULLPTR;
		}
	}

	auto pRptFile = new CRptFile();

	pRptFile->SetRptBaseInfo(pTableInfo);

	m_arrFiles.push_back(pRptFile);

	return pRptFile;
}

/*! \fn bool CRptFileMgr::AddRptFile(CTableInfo* nRptInfo)
********************************************************************************************************* 
** \brief CRptFileMgr::AddRptFile 
** \details 新增报表文件
** \param nRptInfo 
** \return bool 
** \author xingzhibing
** \date 2016年8月13日 
** \note 
********************************************************************************************************/
bool CRptFileMgr::AddRptFile(CRptFile* pRptFile)
{
	Q_ASSERT(pRptFile);
	if (pRptFile==Q_NULLPTR)
	{
		return false;
	}
	for (auto i:m_arrFiles)
	{
		if (i)
		{
			if (i==pRptFile)
			{
				return false;
			}
		}
	}
	m_arrFiles.push_back(pRptFile);
	return true;
}


/*! \fn bool CRptFileMgr::DelRptFile(CRptFile* pRptFile)
********************************************************************************************************* 
** \brief CRptFileMgr::DelRptFile 
** \details 删除指定报表文件
** \param pRptFile 
** \return bool 
** \author xingzhibing
** \date 2016年8月15日 
** \note 
********************************************************************************************************/
bool CRptFileMgr::DelRptFile(CRptFile* pRptFile)
{
	Q_ASSERT(pRptFile);
	if (pRptFile==Q_NULLPTR)
	{
		return false;
	}
	for (auto i:m_arrFiles)
	{
		if (i)
		{
			if (i==pRptFile)
			{
				m_arrFiles.removeOne(i);
				delete pRptFile;
				pRptFile = Q_NULLPTR;
				return true;
			}
		}
	}
	return false;
}

/*! \fn CRptFileGrp * CRptFileMgr::GetRptGrp(const QString& szName)
********************************************************************************************************* 
** \brief CRptFileMgr::GetRptGrp 
** \details 获取指定报表组
** \param szName 
** \return CRptFileGrp * 
** \author xingzhibing
** \date 2016年8月13日 
** \note 
********************************************************************************************************/
CRptFileGrp * CRptFileMgr::GetRptGrp(const QString& szName)
{
	if (szName.isEmpty())
	{
		return Q_NULLPTR;
	}
	if (m_arrGrps.size()==0)
	{
		return Q_NULLPTR;
	}
	for (auto i : m_arrGrps)
	{
		if (i)
		{
			if (i->GetGrpName()==szName)
			{
				return i;
			}
		}
	}
	return Q_NULLPTR;
}

/*! \fn CRptFile* CRptFileMgr::GetRptFile(const QString& szName)
********************************************************************************************************* 
** \brief CRptFileMgr::GetRptFile 
** \details 获取指定报表文件
** \param szName 
** \return CRptFile* 
** \author xingzhibing
** \date 2016年8月13日 
** \note 
********************************************************************************************************/
CRptFile* CRptFileMgr::GetRptFile(const QString& szName)
{
	if (szName.isEmpty())
	{
		return Q_NULLPTR;
	}
	if (m_arrGrps.size() == 0)
	{
		return Q_NULLPTR;
	}
	for (auto i : m_arrFiles)
	{
		if (i)
		{
			if (i->GetRptFileName() == szName)
			{
				return i;
			}
		}
	}
	return Q_NULLPTR;
}

/*! \fn bool CRptFileMgr::SaveFiles(const QString& szRootPath) const
********************************************************************************************************* 
** \brief CRptFileMgr::SaveFiles 
** \details 保存所有的报表文件
** \param szRootPath 
** \return bool 
** \author xingzhibing
** \date 2016年8月13日 
** \note 
********************************************************************************************************/
bool CRptFileMgr::SaveFiles() 
{
	for (auto i:m_arrFiles)
	{
		if (i)
		{
			if (i->GetModifyFlag())
			{
				i->WriteXmlFile();
			}			
		}
	}
	return true;
}

/*! \fn CRptFileMgr::~CRptFileMgr()
********************************************************************************************************* 
** \brief CRptFileMgr::~CRptFileMgr 
** \details 资源清理
** \return  
** \author xingzhibing
** \date 2016年8月13日 
** \note 
********************************************************************************************************/
CRptFileMgr::~CRptFileMgr()
{
	qDeleteAll(m_arrFiles);
	m_arrFiles.clear();

	qDeleteAll(m_arrGrps);
	m_arrGrps.clear();
}

/*! \fn QString CRptFileMgr::GetRecommandRptGrpName()
********************************************************************************************************* 
** \brief CRptFileMgr::GetRecommandRptGrpName 
** \details 获取文件container推荐名称
** \return QString 
** \author xingzhibing
** \date 2016年8月15日 
** \note 
********************************************************************************************************/
QString CRptFileMgr::GetRecommandRptGrpName()
{
	//当前推荐按照数目增加
	QString szName = QStringLiteral("报表%1组").arg(GetRptGrpNum());
	if (IsRptGrpNameValid(szName))
	{
		return szName;
	}
	Q_ASSERT(false);
	return "";
}

/*! \fn bool CRptFileMgr::IsRptGrpNameValid(QString& szName)
********************************************************************************************************* 
** \brief CRptFileMgr::IsRptGrpNameValid 
** \details 判断文件夹名称是否合法
** \param szName 
** \return bool 
** \author xingzhibing
** \date 2016年8月15日 
** \note 
********************************************************************************************************/
bool CRptFileMgr::IsRptGrpNameValid(QString& szName)
{
	if (szName.isEmpty() || szName.length() >= MAX_NAME_LEN)
	{
		return false;
	}
	//TODO  判断文件夹名称是否重复
	for (auto i : m_arrGrps)
	{
		if (i)
		{
			if (i->GetGrpName() == szName)
			{
				return false;
			}
		}
	}
	return true;
}

/*! \fn CRptFileGrp* CRptFileMgr::CreateRptGrp(const QString& szName)
********************************************************************************************************* 
** \brief CRptFileMgr::CreateRptGrp 
** \details 创建报表组管理
** \param szName 
** \return CRptFileGrp* 
** \author xingzhibing
** \date 2016年8月15日 
** \note 
********************************************************************************************************/
CRptFileGrp* CRptFileMgr::CreateRptGrp(const QString& szName)
{
	Q_ASSERT(szName.isEmpty()==false);

	if (szName.isEmpty())
	{
		return Q_NULLPTR;
	}

	//TODO 判断当前目录下是否有重名

	for (auto i:m_arrGrps)
	{
		if (i)
		{
			if (i->GetGrpName()==szName)
			{
				return Q_NULLPTR;
			}
		}
	}
	
	auto pRptGrp = new CRptFileGrp();
	pRptGrp->SetGrpName(szName);
	m_arrGrps.push_back(pRptGrp);

	return pRptGrp;

}

/*! \fn void CRptFileMgr::Close()
********************************************************************************************************* 
** \brief CRptFileMgr::Close 
** \details 文件资源清理
** \return void 
** \author xingzhibing
** \date 2016年8月17日 
** \note 
********************************************************************************************************/
void CRptFileMgr::Close()
{
	qDeleteAll(m_arrFiles);
	m_arrFiles.clear();

	qDeleteAll(m_arrGrps);
	m_arrGrps.clear();
}

/*! \fn bool CRptFileMgr::DelCtrlFile(CRptFile* pRptFile)
********************************************************************************************************* 
** \brief CRptFileMgr::DelCtrlFile 
** \details 删除特定报表显示
** \param pRptFile 
** \return bool 
** \author xingzhibing
** \date 2016年8月24日 
** \note 
********************************************************************************************************/
bool CRptFileMgr::DelCtrlFile(CRptFile* pRptFile,CCtrlData* pData)
{
	Q_ASSERT(pRptFile);
	if (pRptFile==Q_NULLPTR ||pData == Q_NULLPTR)
	{
		return false;
	}
	return true;
}

/*! \fn bool CRptFile::CheckName(const QString & szName)
********************************************************************************************************* 
** \brief CRptFile::CheckName 
** \details 检测文件名称合法性
** \param szName 
** \return bool 
** \author xingzhibing
** \date 2016年8月13日 
** \note 
********************************************************************************************************/
bool CRptFile::CheckName(const QString & szName)
{
	return true;
}



/*! \fn bool CRptFile::SetRptBaseInfo(CTableInfo nInfo)
********************************************************************************************************* 
** \brief CRptFile::SetRptBaseInfo 
** \details 
** \param nInfo 
** \return bool 
** \author xingzhibing
** \date 2016年8月13日 
** \note 
********************************************************************************************************/
bool CRptFile::SetRptBaseInfo(CTableInfo* Info)
{
	m_pTableInfo = Info;
	return true;
}


/*! \fn bool CRptFile::WriteXmlFile()
*********************************************************************************************************
** \brief CRptFile::WriteXmlFile
** \details 将内存文件写入硬盘文件中
** \return bool
** \author xingzhibing
** \date 2016年8月19日
** \note
********************************************************************************************************/
bool CRptFile::WriteXmlFile()
{
	Q_ASSERT(m_pTableInfo);
	if (m_pTableInfo == Q_NULLPTR)
		return false;
	
	Q_ASSERT(m_pTableInfo->m_szTablePath.isEmpty() == false);
	if (m_pTableInfo->m_szTablePath.isEmpty() == true)
		return false;
	
	QString szFileName;

	if (m_pTableInfo->m_szTablePath.contains(m_pTableInfo->m_szTableName))
	{
		szFileName = m_pTableInfo->m_szTablePath;
	}
	else
	{
		 szFileName = m_pTableInfo->m_szTablePath + '/' + m_pTableInfo->m_szTableName;
	}

	if (QFileInfo(szFileName).suffix()!="xml")
	{
		szFileName += ".xml";
	}

	QFile file(szFileName);

	if (szFileName.isEmpty())
	{
		return false;
	}
	if (!file.open(QIODevice::WriteOnly))
	{
		//TODO 文件失败
		return false;
	}

	QApplication::setOverrideCursor(Qt::WaitCursor);

	QXmlWriter *nFunc = new QXmlWriter;
	nFunc->SetReportInfo(m_pTableInfo);
	nFunc->SetReportCell(m_lstCells);
	nFunc->SetRptCtrls(m_lstCtrls);
	nFunc->OnWriteXmlFile(szFileName);

	QApplication::restoreOverrideCursor();
	return true;
	
}

/*! \fn bool  CRptFile::ReadXmlFile()
********************************************************************************************************* 
** \brief CRptFile::ReadXmlFile 
** \details 从硬盘上打开真正的报表文件
** \return bool 
** \author LiJin 
** \date 2016年8月18日 
** \note 
********************************************************************************************************/
bool  CRptFile::ReadXmlFile()
{
	Q_ASSERT(m_pTableInfo->m_szTablePath.isEmpty() ==false);
	if (m_pTableInfo->m_szTablePath.isEmpty() == true)
		return false;

	// 先回收内存
	qDeleteAll(m_lstCells);
	m_lstCells.clear();

	QString szName = m_pTableInfo->m_szTablePath + '/' + m_pTableInfo->m_szTableName;

	if (QFileInfo(szName).suffix()!="xml")
	{
		szName += ".xml";
	}

	QFile file(szName);

	if (file.exists() == false)
	{
		// todo -> log
		return false;
	}
 
	QFileInfo info(file);
	if (info.suffix() == "xml")
	{
	 	//按照XML格式解析
		QXmlReader *pReader = new QXmlReader;
		if (!pReader->OnReadXmlFile(szName))
		{		
			delete pReader;
				pReader = Q_NULLPTR;
			return false;
		}

		this->m_pTableInfo = pReader->GetReportInfo();
	
		QList<CCellData*>& lstTemp = pReader->GetCellList();

		QList<CCtrlData*>& lstCtrlTemp = pReader->GetCtrlList();

		m_lstCells = lstTemp;

		m_lstCtrls = lstCtrlTemp;

		delete pReader;
		pReader = Q_NULLPTR;

		return true;
		// todo -> log 打开 报表成功
	}
	else
	{
		return false;
	}

	return true;
}

/*! \fn QString CRptFile::GetRecommendCtrlName()
********************************************************************************************************* 
** \brief CRptFile::GetRecommendCtrlName 
** \details 获取控件名称
** \return QT_NAMESPACE::QString 
** \author xingzhibing
** \date 2016年8月25日 
** \note 
********************************************************************************************************/
QString CRptFile::GetRecommendCtrlName()
{
	int nCount = m_lstCtrls.size();

	QString szName = QString("CtrlNameX#%1").arg(nCount);

	return szName;
}

/*! \fn CCtrlData* CRptFile::CreateCtrlData(const QString& tagName)
********************************************************************************************************* 
** \brief CRptFile::CreateCtrlData 
** \details 由对应报表创建所属控件
** \param tagName tagname作为控件唯一表示名称
** \return CCtrlData* 
** \author xingzhibing
** \date 2016年8月25日 
** \note 
********************************************************************************************************/
CCtrlData* CRptFile::CreateCtrlData(const QString& tagName)
{
	Q_ASSERT(tagName.isEmpty()==false);
	if (tagName.isEmpty())
	{
		return Q_NULLPTR;
	}
	//若找到已存在，则创建失败
	for (auto i:m_lstCtrls)
	{
		if (i)
		{
			if (i->GetCtrlName()==tagName)
			{
				return Q_NULLPTR;
			}
		}
	}
	auto pCtrlData = new CCtrlData;
	//每个控件有唯一的TagName，由程序自动分配
	pCtrlData->SetCtrlTagName(tagName);

	m_lstCtrls.append(pCtrlData);

	return pCtrlData;
}

/*! \fn void CRptFile::DelCtrlData(CCtrlData* pData)
********************************************************************************************************* 
** \brief CRptFile::DelCtrlData 
** \details 删除无用控件
** \param pData 
** \return void 
** \author xingzhibing
** \date 2016年8月26日 
** \note 
********************************************************************************************************/
void CRptFile::DelCtrlData(CCtrlData* pData)
{
	Q_ASSERT(pData);
	if (pData==Q_NULLPTR)
	{
		return;
	}

	for (auto i:m_lstCtrls)
	{
		if (i)
		{
			if (i==pData)
			{
				m_lstCtrls.removeOne(i);
				if (pData)
				{
					delete pData;
					pData = Q_NULLPTR;
				}
			}
		}
	}
}




/*! \fn bool CRptFileGrp::AddRptGrp(CRptFileGrp* pRptGrp)
********************************************************************************************************* 
** \brief CRptFileGrp::AddRptGrp 
** \details 增加报表组管理
** \param pRptGrp 
** \return bool 
** \author xingzhibing
** \date 2016年8月15日 
** \note 
********************************************************************************************************/
bool CRptFileGrp::AddRptGrp(CRptFileGrp* pRptGrp)
{
	Q_ASSERT(pRptGrp);
	if ( pRptGrp == Q_NULLPTR)
	{
		return false;
	}
	//判断是否有重复
	for (auto i:m_arrGrps)
	{
		if (i)
		{
			if (i->GetGrpName() == pRptGrp->GetGrpName())
			{
				return false;
			}
		}		
	}
	m_arrGrps.push_back(pRptGrp);
	return true;
}

/*! \fn bool CRptFileGrp::AddRptFile(CRptFile* nRptInfo)
********************************************************************************************************* 
** \brief CRptFileGrp::AddRptFile 
** \details 新建报表概要信息文件
** \param nRptInfo 
** \return bool 
** \author xingzhibing
** \date 2016年8月15日 
** \note 
********************************************************************************************************/
bool CRptFileGrp::AddRptFile(CRptFile* pRptInfo)
{
	Q_ASSERT(pRptInfo);
	if (pRptInfo==NULL)
	{
		return false;
	}
	for (auto i:m_arrFiles)
	{
		if (i)
		{
			if (i==pRptInfo)
			{
				return false;
			}
		}
	}
	m_arrFiles.push_back(pRptInfo);

	return true;
}




/*! \fn bool CCtrlData::AddCtrlValue(CCtrlValue* pCtrlValue)
********************************************************************************************************* 
** \brief CCtrlData::AddCtrlValue 
** \details 增加数据成员
** \param pCtrlValue 
** \return bool 
** \author xingzhibing
** \date 2016年8月23日 
** \note 
********************************************************************************************************/
bool CCtrlData::AddCtrlValue(CCtrlGernalValue* pCtrlValue)
{
	Q_ASSERT(pCtrlValue);
	if (pCtrlValue==Q_NULLPTR)
	{
		return false;
	}
	//判断是否有数据重复
	for (auto i:m_VecValue)
	{
		if (i)
		{
			if (i==pCtrlValue)
			{
				return false;
			}
		}
	}
	//数据不重复则添加进列表
	m_VecValue.append(pCtrlValue);

	return true;
}

/*! \fn bool CCtrlData::RemoveCtrlVaule(CCtrlValue* pCtrlValue)
********************************************************************************************************* 
** \brief CCtrlData::RemoveCtrlVaule 
** \details 
** \param pCtrlValue 
** \return bool 
** \author xingzhibing
** \date 2016年8月23日 
** \note 
********************************************************************************************************/
bool CCtrlData::RemoveCtrlVaule(CCtrlGernalValue* pCtrlValue)
{
	Q_ASSERT(pCtrlValue);
	if (pCtrlValue == Q_NULLPTR)
	{
		return false;
	}
	//判断是否有数据重复
	for (auto i : m_VecValue)
	{
		if (i)
		{
			if (i == pCtrlValue)
			{
				m_VecValue.removeOne(i);
				delete pCtrlValue;
				pCtrlValue = Q_NULLPTR;
				return true;
			}
		}
	}

	return true;
}
