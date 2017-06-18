#ifndef ATTRIWINDOW_H
#define ATTRIWINDOW_H

#include <qmainwindow.h>
#include <qmap.h>
#include "attriwindow_global.h"

class CUIMgr;
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
class QtColorPropertyManager;
class QtColorEditorFactory;
class QtVariantPropertyManager;


class ATTRIWINDOW_EXPORT CAttriwindow : public QMainWindow
{
public:
	CAttriwindow(QWidget *widget);
	~CAttriwindow();

	void AddGroupProperty(QString strGroupName);

	QtTreePropertyBrowser *GetTreePropertyBrowser();

	QtGroupPropertyManager *GetGroupProperty();

	QtStringPropertyManager *GetStringProperty();

	QtIntPropertyManager *GetIntProperty();

	QtEnumPropertyManager *GetEnumProperty();

	QtDoublePropertyManager *GetDoubleProperty();

	QtBoolPropertyManager *GetBoolProperty();

	QtSizePropertyManager *GetSizeProperty();

	QtSizePolicyPropertyManager *GetSizePolicyProperty();

	QtColorPropertyManager *GetColorProperty();
private:
	QtTreePropertyBrowser *m_pBrowserEditor;

	QtVariantPropertyManager *m_pVariantManager;

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

	QtColorPropertyManager *m_pColorManager;
	QtColorEditorFactory *m_pColorEditFactory;
};

#endif // ATTRIWINDOW_H
