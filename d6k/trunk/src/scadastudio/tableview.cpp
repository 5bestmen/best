/*! @file
<PRE>
********************************************************************************
模块名       :	基础视图
文件名       :	tableview.cpp
文件实现功能 :  基础视图
作者         :  gw
版本         :  V1.00
--------------------------------------------------------------------------------
备注         : <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief  
*  @author gw
*  @version 1.0
*  @date    2015.11.17
*/

#include "tableview.h"
#include "tablemodel.h"
#include "sortfiltermodel.h"
#include "scadastudiodefine.h"
#include "readtableconfig.h"
//#include "dbapplitonapi.h"
#include "singleton.h"
#include "delegate.h"
#include "scadastudio/imainwindow.h"

#include <QFile>

/*! \fn CTableView::CTableView(QWidget *parent, QString channelName, int type, QString tableName, QString group)
********************************************************************************************************* 
** \brief CTableView::CTableView 
** \details 
** \param parent 父对象
** \param descName 描述
** \param type 类型
** \param tableName 表名
** \param group 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CTableView::CTableView(IMainWindow *parent, const QString & tagName, const QString & descName, int type, QString tableName, QString group)
	: QTableView(parent)
{
	this->setAlternatingRowColors(true);
	this->setStyleSheet( "QTableView{background-color: rgb(199, 237, 204);"   
		"alternate-background-color: rgb(141, 163, 215);}" );
	
	m_pReadTableConfig = new CReadTableConfig();

	//读取表配置文件
	m_pReadTableConfig->ReadFieldData(tableName, m_channelFieldVec);

	QStringList list;
	for(int i = 0; i < m_channelFieldVec.count(); i++)
	{
		list.append(m_channelFieldVec[i].fieldAlasName);
	}

	delete m_pReadTableConfig;

	m_pReadTableConfig = NULL;

	m_pChannelModelData = new CTableModel(this, list);

	m_pSortFilterModel = new CSortFilterModel(this);

	m_pSortFilterModel->setSourceModel(m_pChannelModelData);

	this->setModel(m_pSortFilterModel);

	connect((QObject *)verticalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(SelectRow(int)));

	//connect(this, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));

	parent->GetAtrri();

	connect(this, SIGNAL(SendToAttr(CTableView *)), (QObject *)parent->GetAtrri(), SLOT(ReciveAttri(CTableView *)));

	//CSingleton *test = CSingleton::GetInstance();
	//m_pDbi = test->GetDbi();
	 
	m_strChannelTagName = tagName;

	setSortingEnabled(true);
	sortByColumn(0, Qt::AscendingOrder);

	m_nType = type;

	m_strTableName = tableName;

	m_strGroup = group;

	m_strChannelName = descName;
}

/*! \fn CTableView::~CTableView()
********************************************************************************************************* 
** \brief CTableView::~CTableView 
** \details 
** \return  
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
CTableView::~CTableView()
{
	delete m_pReadTableConfig;

	m_pReadTableConfig = NULL;

	this->deleteLater();
}

/*! \fn void CTableView::AddRow(QVector<QString> &rowList)
********************************************************************************************************* 
** \brief CTableView::addRow 
** \details model添加一行数据
** \param rowList 
** \return void 
** \author gw
** \date 2015年10月28日 
** \note 
********************************************************************************************************/
void CTableView::AddRow(QVector<QString> &rowList)
{
	m_pChannelModelData->AddRowModel(rowList, CTableModel::NORMALROW);

	m_pChannelModelData->RefrushModel();
}

/*! \fn int CTableView::RowCount()
********************************************************************************************************* 
** \brief CTableView::rowCount 
** \details model行数
** \return int 
** \author gw
** \date 2015年10月28日 
** \note 
********************************************************************************************************/
int CTableView::RowCount()
{
	return m_pChannelModelData->rowCount(QModelIndex());
}

/*! \fn void CTableView::InitHeader(QStringList &header)
********************************************************************************************************* 
** \brief CTableView::InitHeader 
** \details 
** \param header 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CTableView::InitHeader(QStringList &header)
{
	m_pChannelModelData->SetHorizontalHeaderList(header);
}

/*! \fn void CTableView::GetFieldInfo(QString strTableName)
********************************************************************************************************* 
** \brief CTableView::getFieldInfo 
** \details 获得表字段配置信息
** \param tableName 
** \return void 
** \author gw
** \date 2015年11月2日 
** \note 
********************************************************************************************************/
void CTableView::GetFieldInfo(QString strTableName)
{
	m_strTableName = strTableName;

	//设置表头
	SetChannelViewHeader();

	//设置表数据
	Init();

	SetDelegateForChannelView(m_channelFieldVec);
}

/*! \fn void CTableView::SetChannelViewHeader()
********************************************************************************************************* 
** \brief CTableView::SetChannelViewHeader 
** \details 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CTableView::SetChannelViewHeader()
{
	QStringList list;

	for(int i = 0; i < m_channelFieldVec.count(); i++)
	{
		list.append(m_channelFieldVec[i].fieldAlasName);

		m_pChannelModelData->setHeaderData(i, Qt::Horizontal, m_channelFieldVec[i].fieldAlasName, Qt::EditRole);

		m_pSortFilterModel->setHeaderData(i, Qt::Horizontal, m_channelFieldVec[i].fieldAlasName, Qt::EditRole);

		m_pChannelModelData->setHeaderData(i, Qt::Vertical, m_channelFieldVec[i].fieldAlasName, Qt::EditRole);

		m_pSortFilterModel->setHeaderData(i, Qt::Vertical, m_channelFieldVec[i].fieldAlasName, Qt::EditRole);
	}	

	InitHeader(list);
}

/*! \fn void CTableView::Init()
********************************************************************************************************* 
** \brief CTableView::init 
** \details 初始化表数据
** \return void 
** \author gw
** \date 2015年11月2日 
** \note 
********************************************************************************************************/
void CTableView::Init()
{	
	QString strSql = "select ";

	QString strTmp;
	for(int i = 0; i < m_channelFieldVec.count(); i++)
	{
		strTmp += m_channelFieldVec[i].fieldName;

		if(i != m_channelFieldVec.count() - 1)
		{
			strTmp += ",";
		}

	}

	strSql = strSql + strTmp + " from " + m_strTableName + " where TagName='" + m_strChannelTagName + "'";

	bool bEmpty = true; 

	//if(m_pDbi->Exec(strSql))
	//{
	//	while(m_pDbi->Next())
	//	{
	//		QVector<QString> vec;

	//		for(int i = 0; i < m_pDbi->GetFieldCounts(); i++)
	//		{
	//			QString strTmp = m_pDbi->GetValue(i).toString();

	//			vec.append(strTmp);
	//		}

	//		AddRow(vec);

	//		bEmpty = false;
	//	}
	//}
	//else
	//{

	//}

	if(bEmpty)
	{
		if(m_nType == FES_CHANNEL_CHILD_ITEM)
		{	
			QVector<QString> vec;
			
			QStringList list = ((CTableModel *)m_pSortFilterModel->sourceModel())->m_lstHorizontalHeader;

			for(int j = 0; j < list.count(); j++)
			{
				for(int i = 0; i < m_channelFieldVec.count(); i++)
				{
					if(list[j].compare(m_channelFieldVec[i].fieldAlasName, Qt::CaseInsensitive) == COMPARE_TRUE)
					{
						/*if(m_channelFieldVec[i].fieldName.compare("TagGroup", Qt::CaseInsensitive) == COMPARE_TRUE)
						{
							vec.append(m_strGroup);
						}*/
						if(m_channelFieldVec[i].fieldName.compare("Description", Qt::CaseInsensitive) == COMPARE_TRUE)
						{
							vec.append(m_strChannelName);
						}
						else
						{
							vec.append(m_channelFieldVec[i].defaultValue);
						}

					}
				}
			}

			AddRow(vec);
		}
	}
}

/*! \fn void CTableView::SetDelegateForChannelView(QVector<CHANNEL> &channelStr)
********************************************************************************************************* 
** \brief CTableView::SetDelegateForChannelView 
** \details 
** \param channelStr 
** \return void 
** \author gw
** \date 2015年11月17日 
** \note 
********************************************************************************************************/
void CTableView::SetDelegateForChannelView(QVector<CHANNELSCADASTUDIO> &channelStr)
{
	for(int i = 0; i < channelStr.count(); i++)
	{
		CHANNELSCADASTUDIO channel = channelStr[i];

		if(channel.fieldFlag == LINEEDIT)
		{
			//CLineEditDelegate *lineDelegate = new CLineEditDelegate(this, channel.fieldType, channel.fieldWidth);
			//setItemDelegateForColumn(i, lineDelegate);
		}
		else if(channel.fieldFlag == COMBOBOX)
		{
			QVector<IDDESC> vec;

			QString strSql;

			strSql = "select " + channel.valueField + "," + channel.showDesc + " from " + channel.searchTable;

			//if(channel.fieldName.compare("PROTOCOL_NAME", Qt::CaseInsensitive) == COMPARE_TRUE)
			//{
			//	 strSql = "select PROTOCOL_ID,DESCRIPTION FROM FES_PROTOCOL";
			//}
			//else if(channel.fieldName.compare("DRIVE_PARA") == COMPARE_TRUE)
			//{
			//	strSql = "select PARAM_ID,DESCRIPTION FROM DRIVE_PARA_104";
			//}

			//bool bReturn = m_pDbi->Exec(strSql);

			//QString strError = m_pDbi->LastError();

			/*if(bReturn)
			{
			while(m_pDbi->Next())
			{
			IDDESC info;

			info.ID = m_pDbi->GetValue(0).toString();

			info.desc = m_pDbi->GetValue(1).toString();

			vec.append(info);
			}
			}*/

			//m_comboDataMap[channel.fieldName] = vec;

			CComboBoxDelegate *comboDelegate = new CComboBoxDelegate(this, vec);
			setItemDelegateForColumn(i, comboDelegate);
		}
		else if(channel.fieldFlag == CHECKBOX)
		{
			CCheckBoxDelegate *checkDelegate = new CCheckBoxDelegate(this);
			setItemDelegateForColumn(i, checkDelegate);
		}
		else if(channel.fieldFlag == COMBOBOXTXTINDEX)
		{
			char filename[ 256 ];
			sprintf( filename , "ini\\");

			QString strFileName = QString(filename) + channelStr[i].searchTable;
			QFile file(strFileName);
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
				return;

			//QMap<QString, QString> valueVec;
			QVector<IDDESC> vec;

			int index = 0;

			while (!file.atEnd()) 
			{
				IDDESC info;

				QByteArray line = file.readLine();
				QString str(line);

				info.ID = QString("%1").arg(index);
				info.desc = str.trimmed();

				index++;

				vec.push_back(info);
			}

			//m_comboDataMap[channel.fieldName] = vec;

			CComboBoxDelegate *comboDelegate = new CComboBoxDelegate(this, vec);
			setItemDelegateForColumn(i, comboDelegate);
		}
		else if(channel.fieldFlag == COMBOBOXTXTSTRING)
		{
			char filename[ 256 ];
			sprintf( filename , "ini\\");

			QString strFileName = QString(filename) + channelStr[i].searchTable;
			QFile file(strFileName);
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
				return;

			QVector<IDDESC> vec;

			int index = 0;

			while (!file.atEnd()) 
			{
				IDDESC info;

				QByteArray line = file.readLine();
				QString str(line);

				info.ID = str.trimmed();
				info.desc = str.trimmed();

				index++;

				vec.push_back(info);
			}

			//m_comboDataMap[channel.fieldName] = vec;

			CComboBoxDelegate *comboDelegate = new CComboBoxDelegate(this, vec);
			setItemDelegateForColumn(i, comboDelegate);
		}
	}
}

/*! \fn void CTableView::Save()
********************************************************************************************************* 
** \brief CTableView::Save 
** \details 保存数据
** \return void 
** \author gw
** \date 2015年12月11日 
** \note 
********************************************************************************************************/
void CTableView::Save()
{
	DeleteData();

	Update();

	////插入
	////QString strTmp;
	//for (int i = 0; i < m_channelFieldVec.count(); i++)
	//{
	//	strTmp += m_channelFieldVec[i].fieldName;

	//	if (i != m_channelFieldVec.count() - 1)
	//	{
	//		strTmp += ",";
	//	}
	//}

	//m_pChannelModelData->arr_row_list;

	//int nCount = m_pChannelModelData->arr_row_list.count();

	//for (int i = 0; i < m_pChannelModelData->arr_row_list.count(); i++)
	//{
	//	QString strSql = "Insert into " + m_strTableName + " Values(";
	//	
	//	for (int j = 0; j < m_pChannelModelData->columnCount(); j++)
	//	{
	//		strSql += "'" + m_pChannelModelData->arr_row_list[i][j] + "',";
	//	}

	//	int nIndex = strSql.lastIndexOf(",");

	//	strSql = strSql.replace(nIndex, 1, ")");

	//	if (m_pDbi->Exec(strSql))
	//	{
	//		m_pDbi->Commit();
	//	}
	//	else
	//	{

	//	}

	//	strSql.clear();
	//}
}

/*! \fn void CTableView::Delete()
********************************************************************************************************* 
** \brief CTableView::Delete 
** \details 删除
** \return void 
** \author gw
** \date 2015年12月14日 
** \note 
********************************************************************************************************/
void CTableView::DeleteData()
{
	//删除
	int nCount = m_pChannelModelData->m_arrRow.size();

	for (int i = nCount - 1; i >= 0; i--)
	{
		if (m_pChannelModelData->m_lstRowStatus[i] == CTableModel::DELEROW)
		{
			QString strSql;

			strSql = "Delete From " + m_strTableName + " Where ";

			QString strKey;

			for (int j = 0; j < m_channelFieldVec.count(); j++)
			{
				if (m_channelFieldVec[j].keyFlag)
				{
					//主键
					strKey += m_channelFieldVec[j].fieldName + "='" + m_pChannelModelData->m_arrRow[0][j] + "',";
				}
			}

			int nIndex = strKey.lastIndexOf(",");
			strKey = strKey.replace(nIndex, 1, "");

			//strSql += strKey;

			//if (m_pDbi->Exec(strSql))
			//{
			//	m_pDbi->Commit();

			//	//删除掉内存
			//	m_pChannelModelData->RemoveRows(i, 1);
			//	m_pChannelModelData->m_rowStatus.removeAt(i);
			//}
			//else
			//{

			//}
		}
	}
}

/*! \fn void CTableView::Update()
********************************************************************************************************* 
** \brief CTableView::Update 
** \details 更新
** \return void 
** \author gw
** \date 2015年12月14日 
** \note 
********************************************************************************************************/
void CTableView::Update()
{
	//更新
	int nCount = m_pChannelModelData->m_arrRow.size();
	
	for(int i = 0; i < nCount; i++)
	{
		if (m_pChannelModelData->m_lstRowStatus[i] == CTableModel::MODIFIEDROW)
		{
			QString strSql = "Update " + m_strTableName + " Set ";

			QString strKey = " Where ";

			QString strTmp;

			for (int j = 0; j < m_channelFieldVec.count(); j++)
			{
				strTmp += m_channelFieldVec[j].fieldName;

				if (m_channelFieldVec[j].keyFlag)
				{
					//主键
					strKey += m_channelFieldVec[j].fieldName + "='" + m_pChannelModelData->m_arrRow[0][j] + "',";
				}

				if (j != m_channelFieldVec.count())
				{
					strTmp += "='" + m_pChannelModelData->m_arrRow[0][j] + "',";
				}
			}

			int nIndex = strKey.lastIndexOf(",");
			strKey = strKey.replace(nIndex, 1, "");

			nIndex = strTmp.lastIndexOf(",");
			strTmp = strTmp.replace(nIndex, 1, "");

			strSql += strTmp + strKey;

			//if (m_pDbi->Exec(strSql))
			//{
			//	m_pDbi->Commit();
			//}
			//else
			//{

			//}
		}
	}
}

/*! \fn void CTableView::Add()
********************************************************************************************************* 
** \brief CTableView::Add 
** \details 添加
** \return void 
** \author gw
** \date 2015年12月14日 
** \note 
********************************************************************************************************/
void CTableView::Add()
{
	//插入
	QString strTmp;
	for (int i = 0; i < m_channelFieldVec.count(); i++)
	{
		strTmp += m_channelFieldVec[i].fieldName;

		if (i != m_channelFieldVec.count() - 1)
		{
			strTmp += ",";
		}
	}

	int nCount = m_pChannelModelData->m_arrRow.count();

	for (int i = 0; i < nCount; i++)
	{
		QString strSql = "Insert into " + m_strTableName + " Values(";

		for (int j = 0; j < m_pChannelModelData->columnCount(); j++)
		{
			strSql += "'" + m_pChannelModelData->m_arrRow[i][j] + "',";
		}

		int nIndex = strSql.lastIndexOf(",");

		strSql = strSql.replace(nIndex, 1, ")");

		//if (m_pDbi->Exec(strSql))
		//{
		//	m_pDbi->Commit();
		//}
		//else
		//{

		//}
	}
}

/*! \fn void CTableView::SetDeleFlag()
********************************************************************************************************* 
** \brief CTableView::SetDeleFlag 
** \details 设置删除标志
** \return void 
** \author gw
** \date 2015年12月15日 
** \note 
********************************************************************************************************/
void CTableView::SetDeleFlag()
{	
	QModelIndexList indexList = this->selectionModel()->selectedIndexes();
	
	QVector<int> vec;

	int row;
	foreach(QModelIndex index, indexList) 
	{
		row = m_pSortFilterModel->mapToSource(index).row();

		if(!vec.contains(row))
		{
			vec.append(row);

			//设置删除标志
			m_pChannelModelData->m_lstRowStatus[row] = CTableModel::DELEROW;

			//刷新(row,0)项
			m_pChannelModelData->emitDataChanged(m_pChannelModelData->index(row, 0));
		}
	}

	//m_pChannelModelData->RefrushModel();
}

void CTableView::SelectRow(int nRow)
{
	Q_UNUSED(nRow);
	
	if (m_pSortFilterModel->rowCount() == 0)
	{
		return;
	}
	
	//QModelIndex index = m_pSortFilterModel->mapToSource(m_pSortFilterModel->index(nRow, 0));

	//int row = index.row();

	emit SendToAttr(this);
}

