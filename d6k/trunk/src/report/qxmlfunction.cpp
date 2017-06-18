#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include "qxmlfunction.h"
#include "public/dataconvert.h"

QXmlWriter::QXmlWriter()
{
	m_pReportInfo=NULL;
	m_CellList.clear();
}

QXmlWriter::~QXmlWriter()
{
	if (m_pReportInfo!=Q_NULLPTR)
	{
		delete m_pReportInfo;
		m_pReportInfo = Q_NULLPTR;
	}
	m_CellList.clear();
}

/*! \fn void QXmlFunction::OnWriteXmlFile(const QString& nFileName)
********************************************************************************************************* 
** \brief QXmlFunction::OnWriteXmlFile 
** \details 写XML文件
** \param nFileName 
** \return void 
** \author xingzhibing
** \date 2015年12月10日 
** \note 
********************************************************************************************************/

void QXmlWriter::OnWriteXmlFile(const QString& nFileName)
{
	if(!m_pReportInfo)
	{
		return;
	}
	QFile file(nFileName);
	if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
	{
		return;
	}

	QDomProcessingInstruction instruction;  
	instruction = m_doc.createProcessingInstruction("xml",QString("version=\"1.0\" encoding=\"UTF-8\"")); 
	m_doc.appendChild(instruction);

	QDomElement root=m_doc.createElement("config");
	m_doc.appendChild(root);

	QDomElement nReportInfo=m_doc.createElement("baseinfo");
	QDomElement nReportSpan=m_doc.createElement("spaninfo");
	QDomElement nReportW=m_doc.createElement("width");
	QDomElement nReportH = m_doc.createElement("heigth");
	QDomElement nReportCell=m_doc.createElement("cell");
	QDomElement nCtrlInfo = m_doc.createElement("ctrl");

	AddReportInfo(nReportInfo);
	AddReportW(nReportW);
	AddReportH(nReportH);
	AddReportSpan(nReportSpan);
	AddReportCell(nReportCell);
	AddRptCtrl(nCtrlInfo);

	root.appendChild(nReportInfo);
	root.appendChild(nReportW);
	root.appendChild(nReportH);
	root.appendChild(nCtrlInfo);
	root.appendChild(nReportSpan);
	root.appendChild(nReportCell);

	QFile f(nFileName);
	if (!f.open(QFile::WriteOnly | QFile::Text))
		return ;

	QTextStream out(&f);
	m_doc.save(out, 4);
	f.close();
}
/*! \fn void QXmlFunction::AddReportInfo(QDomElement nElement)
********************************************************************************************************* 
** \brief QXmlFunction::AddReportInfo 
** \details 报表信息
** \param nElement 
** \return void 
** \author xingzhibing
** \date 2015年12月10日 
** \note 
********************************************************************************************************/

void QXmlWriter::AddReportInfo(QDomElement nElement)
{
	if (!m_pReportInfo||nElement.isNull())
	{
		return ;
	}
	QDomAttr nAddrName=m_doc.createAttribute("reportname");
	QDomAttr nAddrType=m_doc.createAttribute("reporttype");
	QDomAttr nAddrRowCount=m_doc.createAttribute("reportrowcount");
	QDomAttr nAddrColCount=m_doc.createAttribute("reportcolcount");

	nElement.setAttributeNode(nAddrName);
	nElement.setAttributeNode(nAddrType);
	nElement.setAttributeNode(nAddrRowCount);
	nElement.setAttributeNode(nAddrColCount);

	nElement.setAttribute("reportname",m_pReportInfo->m_szTableName);
	nElement.setAttribute("reporttype", m_pReportInfo->m_nTableType);
	nElement.setAttribute("reportrowcount", m_pReportInfo->m_RowCount);
	nElement.setAttribute("reportcolcount", m_pReportInfo->m_ColCount);

}

/*! \fn void QXmlFunction::AddReportSpan(QDomElement nElement)
********************************************************************************************************* 
** \brief QXmlFunction::AddReportSpan 
** \details 合并单元信息
** \param nElement 
** \return void 
** \author xingzhibing
** \date 2015年12月10日 
** \note 
********************************************************************************************************/

void QXmlWriter::AddReportSpan(QDomElement nElement)
{
	if (!m_pReportInfo||nElement.isNull())
	{
		return ;
	}
	int nCount = m_pReportInfo->m_MergeItemList.size();

	for (int i = 0; i < nCount;++i)
	{
		CMergeInfo nData = m_pReportInfo->m_MergeItemList.at(i);
		QDomElement  nEEE=m_doc.createElement("spaninfo");
		nEEE.setAttribute("startrow", nData.m_nRow);
		nEEE.setAttribute("startcol", nData.m_nColumn);
		nEEE.setAttribute("rowcount", nData.m_nRowCount);
		nEEE.setAttribute("colcount", nData.m_nColCount);
		nElement.appendChild(nEEE);
	}
}

/*! \fn void QXmlFunction::AddReportCell(QDomElement nElement)
********************************************************************************************************* 
** \brief QXmlFunction::AddReportCell 
** \details 添加单元格信息
** \param nElement 
** \return void 
** \author xingzhibing
** \date 2015年12月10日 
** \note 
********************************************************************************************************/

void QXmlWriter::AddReportCell(QDomElement nElement)
{
	if (!m_CellList.size()||nElement.isNull())
	{
		return;
	}
	CCellData *pCellData = Q_NULLPTR;
	for (int i = 0; i < m_CellList.size();++i)
	{
		pCellData = m_CellList.at(i);
		if (!pCellData)
		{
			continue;
		}
		QDomElement nCell = m_doc.createElement("c");
		nCell.setAttribute("rowindex", (pCellData->m_nRowIndex));
		nCell.setAttribute("colindex", (pCellData->m_nColIndex));

		nCell.setAttribute("bgcolor",QVariant(pCellData->m_CellBgColor).toString());
		nCell.setAttribute("textalign", pCellData->m_TextAlign);
		nCell.setAttribute("textcolor", QVariant(pCellData->m_TextColor).toString());
		//字体格式保存
		nCell.setAttribute("font", QVariant(pCellData->m_TextFont).toString());

		/*nCell.setAttribute("fontfamily", pCellData->m_TextFont.family());
		nCell.setAttribute("fontpointsize", pCellData->m_TextFont.pointSize());
		nCell.setAttribute("fontweight", pCellData->m_TextFont.weight());
		nCell.setAttribute("fontitalic", pCellData->m_TextFont.italic());*/
		//字体内容
		nCell.setAttribute("content",pCellData->m_szDataContent);
		nElement.appendChild(nCell);
	}
}

/*! \fn void QXmlFunction::AddReportH(QDomElement nElement)
********************************************************************************************************* 
** \brief QXmlFunction::AddReportH 
** \details 添加行高信息
** \param nElement 
** \return void 
** \author xingzhibing
** \date 2015年12月10日 
** \note 
********************************************************************************************************/

void QXmlWriter::AddReportH(QDomElement nElement)
{
	if (nElement.isNull())
	{
		return;
	}
	int nRowCount = m_pReportInfo->m_RowHeight.size();
	QVector<float>m_data = m_pReportInfo->m_RowHeight.values().toVector();
	for (int i = 0; i < nRowCount;++i)
	{
		QDomElement nRowElement = m_doc.createElement("r");
		nRowElement.setAttribute("row",i);
		nRowElement.setAttribute("width",m_data[i] );
		nElement.appendChild(nRowElement);
	}	
} 
/*! \fn void QXmlFunction::AddReportW(QDomElement nElement)
********************************************************************************************************* 
** \brief QXmlFunction::AddReportW 
** \details 添加列宽信息
** \param nElement 
** \return void 
** \author xingzhibing
** \date 2015年12月10日 
** \note 
********************************************************************************************************/

void QXmlWriter::AddReportW(QDomElement nElement)
{
	if (nElement.isNull())
	{
		return;
	}
	int nColCount = m_pReportInfo->m_ColWidth.size();
	QVector<float>m_data = m_pReportInfo->m_ColWidth.values().toVector();
	for (int i = 0; i < nColCount; ++i)
	{
		QDomElement nColElement = m_doc.createElement("c");
		nColElement.setAttribute("column", i);
		nColElement.setAttribute("height", m_data[i]);
		nElement.appendChild(nColElement);
	}
}

/*! \fn void QXmlWriter::AddRptCtrl(QDomElement nElement)
********************************************************************************************************* 
** \brief QXmlWriter::AddRptCtrl 
** \details 增加报表控件节点
** \param nElement 
** \return void 
** \author xingzhibing
** \date 2016年8月30日 
** \note 
********************************************************************************************************/
void QXmlWriter::AddRptCtrl(QDomElement nElement)
{
	int nCount = m_CtrlList.size();

	for (int i = 0; i < nCount; ++i)
	{
		CCtrlData* pData = m_CtrlList.at(i);

		QDomElement  ctrlInfo = m_doc.createElement("ctrlInfo");

		ctrlInfo.setAttribute("rowIndex", pData->GetRowIndex());
		ctrlInfo.setAttribute("colIndex", pData->GetColIndex());
		ctrlInfo.setAttribute("tagName", pData->GetCtrlTagName());
		ctrlInfo.setAttribute("ctrlName",pData->GetCtrlName());
		ctrlInfo.setAttribute("ctrlType",pData->GetCtrlType());
		ctrlInfo.setAttribute("xAisDes",pData->GetXaisContent());
		ctrlInfo.setAttribute("yAisDes",pData->GetYasiContent());
		
		QVector< CCtrlGernalValue* > ctrlVal = pData->GetCtrlVecData();
		//控件关联描述点信息
		for (int i = 0; i < ctrlVal.size(); ++i)
		{		
			CCtrlGernalValue* pVal = ctrlVal.at(i);
			Q_ASSERT(pVal);
			if (pVal)
			{
				QDomElement  pointInfo = m_doc.createElement("pointInfo");
				pointInfo.setAttribute("tagName",pVal->m_TagName );
				pointInfo.setAttribute("pointName",pVal->m_szName);

				ctrlInfo.appendChild(pointInfo);
			}			
		}
		nElement.appendChild(ctrlInfo);
	}
}


QXmlReader::QXmlReader()
{
	m_pReportInfo = NULL;
	m_szFileName  = QString::null;
	m_pReportInfo = new CTableInfo;
}

QXmlReader::~QXmlReader()
{

}
bool QXmlReader::OnReadXmlFile(const QString& nFileName)
{
	SetFileName(nFileName);
	if(nFileName.isEmpty())
	{
		return false;
	}
	QFile file(nFileName);
	if(!file.open(QIODevice::ReadOnly))
	{
		return false;
	}
	QDomDocument document;
	if (!document.setContent(&file))
	{
		file.close();
		return false;
	}
	QDomElement root = document.documentElement();
	if (root.tagName() != "config")
	{
		return false;
	}
	QDomNode node = root.firstChild();
	for (; !node.isNull(); node = node.nextSibling())
	{
		QString nNodeName = node.nodeName();
		if(nNodeName=="baseinfo")
		{
			m_BaseInfoElement = node.toElement();
			if(m_BaseInfoElement.isNull())
			{
				return false;
			}
		}
		if(nNodeName=="spaninfo")
		{
			m_SpanElement = node.toElement();
			if(m_SpanElement.isNull())
			{
				return false;
			}
		}
		if (nNodeName == "width")
		{
			m_WidthElement = node.toElement();
			if(m_WidthElement.isNull())
			{
				return false;
			}
		}
		if(nNodeName=="heigth")
		{
			m_HeightElement = node.toElement();
			if(m_HeightElement.isNull())
			{
				return false;
			}
		}
		if(nNodeName=="cell")
		{
			m_CellElement = node.toElement();
			if(m_CellElement.isNull())
			{
				return false;
			}
		}
		if (nNodeName=="ctrl")
		{
			m_CtrlInfoElement = node.toElement();
			if (m_CtrlInfoElement.isNull())
			{
				return false;
			}
		}
	}
	if(!OnParaseBaseInfo(m_BaseInfoElement))
	{
		return false;
	}
	if(!OnParaseHeigthInfo(m_HeightElement))
	{
		return false;
	}
	if(!OnParaseWidthInfo(m_WidthElement))
	{
		return false;
	}
	if(!OnParaseSpanInfo(m_SpanElement))
	{
		return false;
	}
	if(!OnParaseCellInfo(m_CellElement))
	{
		return false;
	}
	if (!OnParaseCtrlInfo(m_CtrlInfoElement))
	{
		return false;
	}
	file.close();
	return true;
}
bool QXmlReader::OnParaseBaseInfo(QDomElement nElememnt)
{
	if(nElememnt.isNull())
	{
		return false;
	}
	QDomElement pData = nElememnt.toElement();
	   //实际文件信息,此按照传入值判断
		m_pReportInfo->m_szTableName = QFileInfo(GetFIleName()).fileName();
		m_pReportInfo->m_szTablePath = QFileInfo(GetFIleName()).absolutePath();
		//文件类型按照文件读取产生
		m_pReportInfo->m_nTableType = pData.attribute("reporttype").toInt();

		m_pReportInfo->m_RowCount = pData.attribute("reportrowcount").toInt();
		m_pReportInfo->m_ColCount = pData.attribute("reportcolcount").toInt();
	return true;
}
bool QXmlReader::OnParaseSpanInfo(QDomElement nElememnt)
{
	if (nElememnt.isNull())
	{
		return false;
	}
	QDomNode pChild;
	QList<CMergeInfo>nSpanList;	
	for (pChild = nElememnt.firstChild(); !pChild.isNull(); pChild = pChild.nextSibling())
	{
		CMergeInfo nSpanData;
		QDomElement pData = pChild.toElement();
		nSpanData.m_nRow = pData.attribute("startrow").toInt();
		nSpanData.m_nColumn = pData.attribute("startcol").toInt();
		nSpanData.m_nRowCount = pData.attribute("rowcount").toInt();
		nSpanData.m_nColCount = pData.attribute("colcount").toInt();
		nSpanList.append(nSpanData);
	}
	m_pReportInfo->m_MergeItemList = nSpanList;

	return true;
}
bool QXmlReader::OnParaseWidthInfo(QDomElement nElememnt)
{
	if (nElememnt.isNull())
	{
		return false;
	}
	QDomNode pChild;
	QMap<int,float>vecWidth;
	for (pChild = nElememnt.firstChild(); !pChild.isNull(); pChild = pChild.nextSibling())
	{
		QDomElement pData = pChild.toElement();
		float nColWidth = pData.attribute("height").toInt();
		int nIndex=pData.attribute("column").toInt();
		vecWidth[nIndex]=nColWidth;
	}
	m_pReportInfo->m_ColWidth = vecWidth;
	return true;
}
bool QXmlReader::OnParaseHeigthInfo(QDomElement nElememnt)
{
	if (nElememnt.isNull())
	{
		return false;
	}
	QDomNode pChild;
	QMap<int,float>vecHeight;
	for (pChild = nElememnt.firstChild(); !pChild.isNull(); pChild = pChild.nextSibling())
	{
		QDomElement pData = pChild.toElement();
		float nColheight = pData.attribute("width").toInt();
		int nIndex=pData.attribute("row").toInt();
		vecHeight[nIndex]=nColheight;		
	}
	m_pReportInfo->m_RowHeight = vecHeight;
	return true;
}
bool QXmlReader::OnParaseCellInfo(QDomElement nElememnt)
{
	if (nElememnt.isNull())
	{
		return false;
	}
	QDomNode pChild;
	QDomElement elsData;
	CCellData* pCellData = Q_NULLPTR;
	for (pChild = nElememnt.firstChild(); !pChild.isNull(); pChild = pChild.nextSibling())
	{
		pCellData=new CCellData;
		elsData = pChild.toElement();
		pCellData->m_nRowIndex = elsData.attribute("rowindex").toInt();
		pCellData->m_nColIndex = elsData.attribute("colindex").toInt();
		pCellData->m_CellBgColor = QVariant(elsData.attribute("bgcolor")).value<QColor>();
		pCellData->m_TextAlign = elsData.attribute("textalign").toInt();
		pCellData->m_TextFont = QVariant(elsData.attribute("font")).value<QFont>();
		pCellData->m_TextColor = QVariant(elsData.attribute("textcolor")).value<QColor>();
		pCellData->m_szDataContent = elsData.attribute("content");
		m_CellList.append(pCellData);
	}
	return true;
}

/*! \fn bool QXmlReader::OnParaseCtrlInfo(QDomElement eleInfo)
********************************************************************************************************* 
** \brief QXmlReader::OnParaseCtrlInfo 
** \details 解析读取控件信息
** \param eleInfo 
** \return bool 
** \author xingzhibing
** \date 2016年8月31日 
** \note 
********************************************************************************************************/
bool QXmlReader::OnParaseCtrlInfo(QDomElement eleInfo)
{
	if (eleInfo.isNull())
	{
		return false;
	}
	QDomNode childNode;
	QDomElement eleCtrlInfo;
	CCtrlData* pCtrlData = Q_NULLPTR;
	for (childNode = eleInfo.firstChild();!childNode.isNull();childNode=childNode.nextSibling())
	{
		eleCtrlInfo = childNode.toElement();

		pCtrlData = new CCtrlData;
		//基本属性
		pCtrlData->SetCtrlName(eleCtrlInfo.attribute("ctrlName"));
		pCtrlData->SetCtrlTagName(eleCtrlInfo.attribute("tagName"));
		pCtrlData->SetCtrlPosition(eleCtrlInfo.attribute("rowIndex").toInt(),eleCtrlInfo.attribute("colIndex").toInt());
		pCtrlData->SetCtrlType((CtrlType)eleCtrlInfo.attribute("ctrlType").toInt());
		pCtrlData->SetXaisContent(eleCtrlInfo.attribute("xAisDes"));
		pCtrlData->SetYaisContent(eleCtrlInfo.attribute("yAisDes"));
		
		QDomNode grandChildNode = eleCtrlInfo.firstChild();

		CCtrlGernalValue* pVal = Q_NULLPTR;
		QVector<CCtrlGernalValue*> ctrlGerVal;
		for (;!grandChildNode.isNull();grandChildNode=grandChildNode.nextSibling())
		{
			QDomElement elePointInfo = grandChildNode.toElement();
			pVal = new CCtrlGernalValue;
			pVal->m_szName = elePointInfo.attribute("pointName");
			pVal->m_TagName = elePointInfo.attribute("tagName");
			ctrlGerVal.append(pVal);
		}
		pCtrlData->SetCtrlData(ctrlGerVal);
		m_CtrlList.append(pCtrlData);
	}
	return true;
}
