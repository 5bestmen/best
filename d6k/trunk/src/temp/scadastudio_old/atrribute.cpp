/*! @file
<PRE>
********************************************************************************
ģ����       :  ������
�ļ���       :  atribute.cpp
�ļ�ʵ�ֹ��� :  Item
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date    2015.11.27
*/

#include "atrribute.h"
#include "scadastudio.h"
#include "qtpropertybrowser.h"
#include "qttreepropertybrowser.h"
#include "qtpropertymanager.h"
#include "qteditorfactory.h"
#include "scadastudio.h"

/*! \fn CAtrribute::CAtrribute(QWidget *parent)
********************************************************************************************************* 
** \brief CAtribute::CAtribute 
** \details 
** \param parent ������
** \return  
** \author gw
** \date 2015��11��27�� 
** \note 
********************************************************************************************************/
CAtrribute::CAtrribute(CScadastudio *parent)
	: QMainWindow(parent)
{
	m_pUI = parent;

	m_pBrowserEditor = new QtTreePropertyBrowser(this);

	m_pBrowserEditor->setResizeMode(QtTreePropertyBrowser::Interactive);

	//Combobox
	m_pComboBoxFactory = new QtEnumEditorFactory(this);
	m_pEnumManager = new QtEnumPropertyManager(this);
	m_pBrowserEditor->setFactoryForManager(m_pEnumManager, m_pComboBoxFactory);

	//�ַ���
	m_pStringManager = new QtStringPropertyManager(this);
	m_pLineEditFactory = new QtLineEditFactory(this);
	m_pBrowserEditor->setFactoryForManager(m_pStringManager, m_pLineEditFactory);

	//������
	m_pDoubleManager = new QtDoublePropertyManager(this);
	m_pDoubleFactory = new QtDoubleSpinBoxFactory(this);
	m_pBrowserEditor->setFactoryForManager(m_pDoubleManager, m_pDoubleFactory);

	//��
	m_pGroupManager = new QtGroupPropertyManager(this);

	//Checkbox
	m_pBoolManager = new QtBoolPropertyManager(this);
	m_pCheckFactory = new QtCheckBoxFactory(this);
	m_pBrowserEditor->setFactoryForManager(m_pBoolManager, m_pCheckFactory);

	//Int
	QtIntPropertyManager *m_pIntManager = new QtIntPropertyManager(this);
	m_pSpinFactory = new QtSpinBoxFactory(this);
	m_pBrowserEditor->setFactoryForManager(m_pIntManager, m_pSpinFactory);

	//QSize
	m_pSizeManager = new QtSizePropertyManager(this);
	m_pBrowserEditor->setFactoryForManager(m_pSizeManager->subIntPropertyManager(), m_pSpinFactory);

	m_pSizePolicyManager = new QtSizePolicyPropertyManager(this);
	m_pBrowserEditor->setFactoryForManager(m_pSizePolicyManager->subIntPropertyManager(), m_pSpinFactory);
	m_pBrowserEditor->setFactoryForManager(m_pSizePolicyManager->subEnumPropertyManager(), m_pComboBoxFactory);

	m_pRectManager = new QtRectPropertyManager(this);
	m_pBrowserEditor->setFactoryForManager(m_pRectManager->subIntPropertyManager(), m_pSpinFactory);

	//QtStringPropertyManager *stringManager = new QtStringPropertyManager(this);
	//QtSizePropertyManager *sizeManager = new QtSizePropertyManager(this);
	//QtRectPropertyManager *rectManager = new QtRectPropertyManager(this);
	//QtSizePolicyPropertyManager *sizePolicyManager = new QtSizePolicyPropertyManager(this);


	CreateConnect();

	setCentralWidget(m_pBrowserEditor);


	//����
	QtProperty *item0 = m_pGroupManager->addProperty("QObject");

	m_pBrowserEditor->addProperty(item0);

	QtProperty *property = m_pDoubleManager->addProperty(tr("Position X"));
	m_pDoubleManager->setRange(property, 0, 250);
	m_pDoubleManager->setValue(property, 22);
	item0->addSubProperty(property);

	property = m_pEnumManager->addProperty(QStringLiteral("����"));
	QStringList enumNames;
	enumNames << "Boolean" << "Char" << "Byte" << "Float" << "Double" << "String";
	m_pEnumManager->setEnumNames(property, enumNames);
	item0->addSubProperty(property);
	
	//��������
	property = m_pStringManager->addProperty(QStringLiteral("��ʼֵ"));

	QString pattern = QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,5}$");
	QRegExp regExp;
	regExp.setPattern(pattern);

	m_pStringManager->setRegExp(property, QRegExp(QString("^[0-9a-zA-Z#\u4e00-\u9fa5]{1,5}$")));
	item0->addSubProperty(property);

	property = m_pBoolManager->addProperty(QStringLiteral("ȡ��"));
	item0->addSubProperty(property);

	property = m_pIntManager->addProperty("value");
	m_pIntManager->setRange(property, -100, 100);
	item0->addSubProperty(property);

	property = m_pSizeManager->addProperty("sizeIncrement");
	item0->addSubProperty(property);

	property = m_pSizePolicyManager->addProperty("sizePolicy");
	item0->addSubProperty(property);

	property = m_pRectManager->addProperty("sizeIncrement");
	item0->addSubProperty(property);
}

/*! \fn CAtrribute::~CAtrribute()
********************************************************************************************************* 
** \brief CAtribute::~CAtribute 
** \details 
** \return  
** \author gw
** \date 2015��11��27�� 
** \note 
********************************************************************************************************/
CAtrribute::~CAtrribute()
{

}

/*! \fn void CAtrribute::createConnect()
********************************************************************************************************* 
** \brief CAtrribute::createConnect 
** \details �źŲ�
** \return void 
** \author gw
** \date 2015��11��29�� 
** \note 
********************************************************************************************************/
void CAtrribute::CreateConnect()
{
	//connect(m_filePathManager, SIGNAL(valueChanged(QtProperty *, const QString &)),
	//	this, SLOT(valueChanged(QtProperty *, const QString &)));

	connect(m_pEnumManager, SIGNAL(valueChanged(QtProperty *, int)),
		this, SLOT(valueChanged(QtProperty *, int)));

	connect(m_pDoubleManager, SIGNAL(valueChanged(QtProperty *, double )),
		this, SLOT(valueChanged(QtProperty *, double)));

	connect(m_pStringManager, SIGNAL(valueChanged(QtProperty *, const QString &)),
		this, SLOT(valueChanged(QtProperty *, const QString &)));

	connect(m_pBoolManager, SIGNAL(valueChanged(QtProperty *, bool)),
		this, SLOT(valueChanged(QtProperty *, bool)));

	connect(m_pSizeManager, SIGNAL(valueChanged(QtProperty *, const QSize &)),
		this, SLOT(valueChanged(QtProperty *, const QSize &)));

	connect(m_pSizePolicyManager, SIGNAL(valueChanged(QtProperty *, const QSizePolicy &)),
		this, SLOT(valueChanged(QtProperty *, const QSizePolicy &)));
}

/*! \fn void CAtrribute::valueChanged(QtProperty *property, double val)
********************************************************************************************************* 
** \brief CAtrribute::valueChanged 
** \details double
** \param property 
** \param val 
** \return void 
** \author gw
** \date 2015��11��29�� 
** \note 
********************************************************************************************************/
void CAtrribute::valueChanged(QtProperty *property, double val)
{
	if (!m_propertyToId.contains(property))
	{
		return;
	}

	//����
	QString strTmp = m_propertyToId[property];
}

/*! \fn void CAtrribute::valueChanged(QtProperty *property, int val)
********************************************************************************************************* 
** \brief CAtrribute::valueChanged
** \details int
** \param property 
** \param val 
** \return void 
** \author gw
** \date 2015��11��29�� 
** \note 
********************************************************************************************************/
void CAtrribute::valueChanged(QtProperty *property, int val)
{
	if (!m_propertyToId.contains(property))
	{
		return;
	}

	//����
	QString strTmp = m_propertyToId[property];

	m_pEnumManager->setValue(property, val);
}

/*! \fn void CAtrribute::valueChanged(QtProperty *property, const QString &val)
********************************************************************************************************* 
** \brief CAtrribute::valueChanged 
** \details QString
** \param property 
** \param val 
** \return void 
** \author gw
** \date 2015��11��29�� 
** \note 
********************************************************************************************************/
void CAtrribute::valueChanged(QtProperty *property, const QString &val)
{
	if (!m_propertyToId.contains(property))
	{
		return;
	}

	//����
	QString strTmp = m_propertyToId[property];
}

/*! \fn void CAtrribute::GetAttriInfo()
********************************************************************************************************* 
** \brief CAtrribute::GetAttriInfo 
** \details ��view���ݵ�������Ϣ
** \return void 
** \author gw
** \date 2015��11��29�� 
** \note 
********************************************************************************************************/
void CAtrribute::GetAttriInfo()
{
	//�ṹ����
}

/*! \fn 
********************************************************************************************************* 
** \brief CAtrribute::valueChanged 
** \details 
** \param property 
** \param val 
** \return void 
** \author gw
** \date 2015��12��1�� 
** \note 
********************************************************************************************************/
void CAtrribute::valueChanged(QtProperty *property, bool val)
{
	if (!m_propertyToId.contains(property))
	{
		return;
	}

	//����
	QString strTmp = m_propertyToId[property];
}

/*! \fn 
********************************************************************************************************* 
** \brief CAtrribute::valueChanged 
** \details 
** \param property 
** \param val 
** \return void 
** \author gw
** \date 2015��12��1�� 
** \note 
********************************************************************************************************/
void CAtrribute::valueChanged(QtProperty *property, const QSize &val)
{
	if (!m_propertyToId.contains(property))
	{
		return;
	}

	//����
	QString strTmp = m_propertyToId[property];
}

/*! \fn 
********************************************************************************************************* 
** \brief CAtrribute::valueChanged 
** \details 
** \param property 
** \param val 
** \return void 
** \author gw
** \date 2015��12��1�� 
** \note 
********************************************************************************************************/
void CAtrribute::valueChanged(QtProperty *property, const QSizePolicy &val)
{
	if (!m_propertyToId.contains(property))
	{
		return;
	}

	//����
	QString strTmp = m_propertyToId[property];
}

/*! \fn 
********************************************************************************************************* 
** \brief CAtrribute::CreateAttriWindow 
** \details ����������
** \param nType 
** \param pData 
** \return void 
** \author gw
** \date 2015��12��1�� 
** \note 
********************************************************************************************************/
void CAtrribute::CreateAttriWindow(int32s nType, void *pData)
{

}