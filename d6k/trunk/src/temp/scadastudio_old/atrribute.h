#ifndef ATRRIBUTE_H
#define ATRRIBUTE_H

#include <QMainWindow>
#include <QMap>
#include "datatypes.h"

class CScadastudio;
class QtTreePropertyBrowser;
class QtEnumEditorFactory;
class QtEnumPropertyManager;
class QtStringPropertyManager;
class QtLineEditFactory;
class QtDoublePropertyManager;
class QtDoubleSpinBoxFactory;
class QtProperty;
class QtGroupPropertyManager;
class QtBoolPropertyManager;
class QtCheckBoxFactory;
class QtIntPropertyManager;
class QtSpinBoxFactory;
class QtSizePropertyManager;
class QtSizePolicyPropertyManager;
class QtRectPropertyManager;

class CAtrribute : public QMainWindow
{
	Q_OBJECT

public:
	CAtrribute(CScadastudio *parent);
	~CAtrribute();

	void CreateAttriWindow(int32s nType, void *pData);

public slots:
	void valueChanged(QtProperty *property, const QString &val);

	void valueChanged(QtProperty *property, int val);

	void valueChanged(QtProperty *property, double val);

	void GetAttriInfo();

	void valueChanged(QtProperty *property, bool val);

	//QSize
	void valueChanged(QtProperty *property, const QSize &val);

	//QSizePolicy
	void valueChanged(QtProperty *property, const QSizePolicy &val);

private:
	void CreateConnect();

private:
	//´ó¿ò¼ÜUIÖ¸Õë
	CScadastudio *m_pUI;

	QtTreePropertyBrowser *m_pBrowserEditor;

	QtEnumEditorFactory *m_pComboBoxFactory;
	QtEnumPropertyManager *m_pEnumManager;

	QtStringPropertyManager *m_pStringManager;
	QtLineEditFactory *m_pLineEditFactory;

	QtDoublePropertyManager  *m_pDoubleManager;
	QtDoubleSpinBoxFactory *m_pDoubleFactory;

	QMap<QtProperty *, QString> m_propertyToId;
	QMap<QString, QtProperty *> m_idToProperty;
	QMap<QString, bool> m_idToExpanded;

	QtGroupPropertyManager *m_pGroupManager;

	QtBoolPropertyManager *m_pBoolManager;
	QtCheckBoxFactory *m_pCheckFactory;

	QtIntPropertyManager *m_pIntManager;
	QtSpinBoxFactory *m_pSpinFactory;

	QtSizePropertyManager *m_pSizeManager;

	QtSizePolicyPropertyManager *m_pSizePolicyManager;

	QtRectPropertyManager *m_pRectManager;
};

#endif // CATRRIBUTE_H
