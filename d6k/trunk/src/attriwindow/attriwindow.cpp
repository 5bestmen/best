#include "attriwindow/attriwindow.h"
#include "qtpropertybrowser/qtpropertybrowser.h"
#include "qtpropertybrowser/qttreepropertybrowser.h"
#include "qtpropertybrowser/qtpropertymanager.h"
#include "qtpropertybrowser/qteditorfactory.h"

/*! \fn CAttriwindow::CAttriwindow(QWidget *widget)
********************************************************************************************************* 
** \brief CAttriwindow::CAttriwindow 
** \details 
** \param widget 
** \return  
** \author gw
** \date 2016年1月26日 
** \note 
********************************************************************************************************/
CAttriwindow::CAttriwindow(QWidget *widget)
	:QMainWindow(widget), m_pBrowserEditor(NULL), m_pEnumManager(NULL), m_pStringManager(NULL), m_pDoubleManager(NULL),
	m_pGroupManager(NULL), m_pBoolManager(NULL), m_pIntManager(NULL), m_pSizeManager(NULL), m_pSizePolicyManager(NULL), m_pRectManager(NULL),
	m_pColorManager(nullptr)
{
	m_pBrowserEditor = new QtTreePropertyBrowser(this);

	m_pBrowserEditor->setResizeMode(QtTreePropertyBrowser::Interactive);

	//Combobox
	m_pComboBoxFactory = new QtEnumEditorFactory(this);
	m_pEnumManager = new QtEnumPropertyManager(this);
	m_pBrowserEditor->setFactoryForManager(m_pEnumManager, m_pComboBoxFactory);

	//字符串
	m_pStringManager = new QtStringPropertyManager(this);
	m_pLineEditFactory = new QtLineEditFactory(this);
	m_pBrowserEditor->setFactoryForManager(m_pStringManager, m_pLineEditFactory);

	//浮点数
	m_pDoubleManager = new QtDoublePropertyManager(this);
	m_pDoubleFactory = new QtDoubleSpinBoxFactory(this);
	m_pBrowserEditor->setFactoryForManager(m_pDoubleManager, m_pDoubleFactory);

	//组
	m_pGroupManager = new QtGroupPropertyManager(this);

	//Checkbox
	m_pBoolManager = new QtBoolPropertyManager(this);
	m_pCheckFactory = new QtCheckBoxFactory(this);
	m_pBrowserEditor->setFactoryForManager(m_pBoolManager, m_pCheckFactory);

	//Int
	m_pIntManager = new QtIntPropertyManager(this);
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

	m_pColorManager = new QtColorPropertyManager(this);
	m_pColorEditFactory = new QtColorEditorFactory(this);
	m_pBrowserEditor->setFactoryForManager(m_pColorManager, m_pColorEditFactory);

	setCentralWidget(m_pBrowserEditor);

	m_pBrowserEditor->show();
}

/*! \fn CAttriwindow::~CAttriwindow()
********************************************************************************************************* 
** \brief CAttriwindow::~CAttriwindow 
** \details 
** \return  
** \author gw
** \date 2016年1月26日 
** \note 
********************************************************************************************************/
CAttriwindow::~CAttriwindow()
{

}

/*! \fn void CAttriwindow::AddGroupProperty(QString strGroupName)
********************************************************************************************************* 
** \brief CAttriwindow::AddGroupProperty 
** \details 
** \param strGroupName 
** \return void 
** \author gw
** \date 2016年1月25日 
** \note 
********************************************************************************************************/
void CAttriwindow::AddGroupProperty(QString strGroupName)
{
	QtProperty *property = m_pGroupManager->addProperty(strGroupName);

	m_pBrowserEditor->addProperty(property);
}

/*! \fn QtTreePropertyBrowser *CAttriwindow::GetTreePropertyBrowser()
********************************************************************************************************* 
** \brief CAttriwindow::GetTreePropertyBrowser 
** \details Property Browser
** \return QtTreePropertyBrowser * 
** \author gw
** \date 2016年1月26日 
** \note 
********************************************************************************************************/
QtTreePropertyBrowser *CAttriwindow::GetTreePropertyBrowser()
{
	Q_ASSERT(m_pBrowserEditor);

	return m_pBrowserEditor;
}

/*! \fn QtGroupPropertyManager *CAttriwindow::GetGroupProperty()
********************************************************************************************************* 
** \brief CAttriwindow::GetGroupProperty 
** \details 组管理
** \return QtGroupPropertyManager * 
** \author gw
** \date 2016年1月26日 
** \note 
********************************************************************************************************/
QtGroupPropertyManager *CAttriwindow::GetGroupProperty()
{
	Q_ASSERT(m_pGroupManager);

	return m_pGroupManager;
}

/*! \fn QtStringPropertyManager *CAttriwindow::GetStringProperty()
********************************************************************************************************* 
** \brief CAttriwindow::GetStringProperty 
** \details 字符串管理
** \return QtStringPropertyManager * 
** \author gw
** \date 2016年1月26日 
** \note 
********************************************************************************************************/
QtStringPropertyManager *CAttriwindow::GetStringProperty()
{
	Q_ASSERT(m_pStringManager);

	return m_pStringManager;
}

/*! \fn QtIntPropertyManager *CAttriwindow::GetIntProperty()
********************************************************************************************************* 
** \brief CAttriwindow::GetIntProperty 
** \detail Int管理
** \return QtIntPropertyManager * 
** \author gw
** \date 2016年1月26日 
** \note 
********************************************************************************************************/
QtIntPropertyManager *CAttriwindow::GetIntProperty()
{
	Q_ASSERT(m_pIntManager);

	return m_pIntManager;
}

/*! \fn QtEnumPropertyManager *CAttriwindow::GetEnumProperty()
********************************************************************************************************* 
** \brief CAttriwindow::GetEnumProperty 
** \details Enum管理
** \return QtEnumPropertyManager * 
** \author gw
** \date 2016年1月26日 
** \note 
********************************************************************************************************/
QtEnumPropertyManager *CAttriwindow::GetEnumProperty()
{
	Q_ASSERT(m_pEnumManager);

	return m_pEnumManager;
}

QtDoublePropertyManager *CAttriwindow::GetDoubleProperty()
{
	Q_ASSERT(m_pDoubleManager);

	return m_pDoubleManager;
}

QtBoolPropertyManager *CAttriwindow::GetBoolProperty()
{
	Q_ASSERT(m_pBoolManager);

	return m_pBoolManager;
}

QtSizePropertyManager *CAttriwindow::GetSizeProperty()
{
	Q_ASSERT(m_pSizeManager);

	return m_pSizeManager;
}

QtSizePolicyPropertyManager *CAttriwindow::GetSizePolicyProperty()
{
	Q_ASSERT(m_pSizePolicyManager);

	return m_pSizePolicyManager;
}

QtColorPropertyManager *CAttriwindow::GetColorProperty()
{
	Q_ASSERT(m_pColorManager);

	return m_pColorManager;
}