#include <QFile>
#include <QFileInfo> 
#include <QMessageBox>
#include <QApplication>
#include "configinfo.h"
#include "qxmlfunction.h"

/*! \fn bool CRptFileMgr::CheckName(const QString & szName)
********************************************************************************************************* 
** \brief CRptFileMgr::CheckName 
** \details �ļ������Ϸ���
** \param szName 
** \return bool 
** \author xingzhibing
** \date 2016��8��13�� 
** \note 
********************************************************************************************************/
bool CRptFileMgr::CheckName(const QString & szName)
{
	return true;
}

/*! \fn CTableInfo* CRptFileMgr::CreateRptFile(const QString& szName)
********************************************************************************************************* 
** \brief CRptFileMgr::CreateRptFile 
** \details ���������ļ�
** \param szName 
** \return CTableInfo* 
** \author xingzhibing
** \date 2016��8��13�� 
** \note 
********************************************************************************************************/
CRptFile* CRptFileMgr::CreateRptFile(CTableInfo*  pTableInfo)
{
	Q_ASSERT(pTableInfo);

	if (pTableInfo->m_szTableName.isEmpty())
	{
		return Q_NULLPTR;
	}

	//�ж��Ƿ����ļ����ڣ�ȫ·�������жϣ����Ʋ��ظ�
	for (auto it:m_arrFiles)
	{
		if (pTableInfo->m_szTableName.compare(it->GetRptFileName()) == 0)
		{
			Q_ASSERT(false);
			return Q_NULLPTR;
		}
	}

	//�ж��Ƿ����ļ����ڣ�ȫ·���жϣ�·�����ظ�
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
** \details ���������ļ�
** \param nRptInfo 
** \return bool 
** \author xingzhibing
** \date 2016��8��13�� 
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
** \details ɾ��ָ�������ļ�
** \param pRptFile 
** \return bool 
** \author xingzhibing
** \date 2016��8��15�� 
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
** \details ��ȡָ��������
** \param szName 
** \return CRptFileGrp * 
** \author xingzhibing
** \date 2016��8��13�� 
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
** \details ��ȡָ�������ļ�
** \param szName 
** \return CRptFile* 
** \author xingzhibing
** \date 2016��8��13�� 
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
** \details �������еı����ļ�
** \param szRootPath 
** \return bool 
** \author xingzhibing
** \date 2016��8��13�� 
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
** \details ��Դ����
** \return  
** \author xingzhibing
** \date 2016��8��13�� 
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
** \details ��ȡ�ļ�container�Ƽ�����
** \return QString 
** \author xingzhibing
** \date 2016��8��15�� 
** \note 
********************************************************************************************************/
QString CRptFileMgr::GetRecommandRptGrpName()
{
	//��ǰ�Ƽ�������Ŀ����
	QString szName = QStringLiteral("����%1��").arg(GetRptGrpNum());
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
** \details �ж��ļ��������Ƿ�Ϸ�
** \param szName 
** \return bool 
** \author xingzhibing
** \date 2016��8��15�� 
** \note 
********************************************************************************************************/
bool CRptFileMgr::IsRptGrpNameValid(QString& szName)
{
	if (szName.isEmpty() || szName.length() >= MAX_NAME_LEN)
	{
		return false;
	}
	//TODO  �ж��ļ��������Ƿ��ظ�
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
** \details �������������
** \param szName 
** \return CRptFileGrp* 
** \author xingzhibing
** \date 2016��8��15�� 
** \note 
********************************************************************************************************/
CRptFileGrp* CRptFileMgr::CreateRptGrp(const QString& szName)
{
	Q_ASSERT(szName.isEmpty()==false);

	if (szName.isEmpty())
	{
		return Q_NULLPTR;
	}

	//TODO �жϵ�ǰĿ¼���Ƿ�������

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
** \details �ļ���Դ����
** \return void 
** \author xingzhibing
** \date 2016��8��17�� 
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
** \details ɾ���ض�������ʾ
** \param pRptFile 
** \return bool 
** \author xingzhibing
** \date 2016��8��24�� 
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
** \details ����ļ����ƺϷ���
** \param szName 
** \return bool 
** \author xingzhibing
** \date 2016��8��13�� 
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
** \date 2016��8��13�� 
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
** \details ���ڴ��ļ�д��Ӳ���ļ���
** \return bool
** \author xingzhibing
** \date 2016��8��19��
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
		//TODO �ļ�ʧ��
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
** \details ��Ӳ���ϴ������ı����ļ�
** \return bool 
** \author LiJin 
** \date 2016��8��18�� 
** \note 
********************************************************************************************************/
bool  CRptFile::ReadXmlFile()
{
	Q_ASSERT(m_pTableInfo->m_szTablePath.isEmpty() ==false);
	if (m_pTableInfo->m_szTablePath.isEmpty() == true)
		return false;

	// �Ȼ����ڴ�
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
	 	//����XML��ʽ����
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
		// todo -> log �� ����ɹ�
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
** \details ��ȡ�ؼ�����
** \return QT_NAMESPACE::QString 
** \author xingzhibing
** \date 2016��8��25�� 
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
** \details �ɶ�Ӧ�����������ؼ�
** \param tagName tagname��Ϊ�ؼ�Ψһ��ʾ����
** \return CCtrlData* 
** \author xingzhibing
** \date 2016��8��25�� 
** \note 
********************************************************************************************************/
CCtrlData* CRptFile::CreateCtrlData(const QString& tagName)
{
	Q_ASSERT(tagName.isEmpty()==false);
	if (tagName.isEmpty())
	{
		return Q_NULLPTR;
	}
	//���ҵ��Ѵ��ڣ��򴴽�ʧ��
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
	//ÿ���ؼ���Ψһ��TagName���ɳ����Զ�����
	pCtrlData->SetCtrlTagName(tagName);

	m_lstCtrls.append(pCtrlData);

	return pCtrlData;
}

/*! \fn void CRptFile::DelCtrlData(CCtrlData* pData)
********************************************************************************************************* 
** \brief CRptFile::DelCtrlData 
** \details ɾ�����ÿؼ�
** \param pData 
** \return void 
** \author xingzhibing
** \date 2016��8��26�� 
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
** \details ���ӱ��������
** \param pRptGrp 
** \return bool 
** \author xingzhibing
** \date 2016��8��15�� 
** \note 
********************************************************************************************************/
bool CRptFileGrp::AddRptGrp(CRptFileGrp* pRptGrp)
{
	Q_ASSERT(pRptGrp);
	if ( pRptGrp == Q_NULLPTR)
	{
		return false;
	}
	//�ж��Ƿ����ظ�
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
** \details �½������Ҫ��Ϣ�ļ�
** \param nRptInfo 
** \return bool 
** \author xingzhibing
** \date 2016��8��15�� 
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
** \details �������ݳ�Ա
** \param pCtrlValue 
** \return bool 
** \author xingzhibing
** \date 2016��8��23�� 
** \note 
********************************************************************************************************/
bool CCtrlData::AddCtrlValue(CCtrlGernalValue* pCtrlValue)
{
	Q_ASSERT(pCtrlValue);
	if (pCtrlValue==Q_NULLPTR)
	{
		return false;
	}
	//�ж��Ƿ��������ظ�
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
	//���ݲ��ظ�����ӽ��б�
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
** \date 2016��8��23�� 
** \note 
********************************************************************************************************/
bool CCtrlData::RemoveCtrlVaule(CCtrlGernalValue* pCtrlValue)
{
	Q_ASSERT(pCtrlValue);
	if (pCtrlValue == Q_NULLPTR)
	{
		return false;
	}
	//�ж��Ƿ��������ظ�
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
