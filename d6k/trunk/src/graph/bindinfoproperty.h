#ifndef BINDINFOPROPERTY
#define BINDINFOPROPERTY

#include "qtpropertybrowser/QtVariantEditorFactory"
#include <QDateTime>
#include <QLineEdit>
#include "qtpropertybrowser/QtVariantProperty"
#include "qtpropertybrowser/QtVariantPropertyManager"


class CBindDataWgt;

class CBindInfoFactory : public QtVariantEditorFactory
{
	Q_OBJECT
public:
	CBindInfoFactory(QObject *parent = 0)
		: QtVariantEditorFactory(parent)
	{ }

	virtual ~CBindInfoFactory();

	void AddDataType(int nType)
	{
		m_arrDataType.push_back(nType);
	}

	void RemoveDataType(int nType)
	{
		for (auto it = m_arrDataType.begin(); it != m_arrDataType.end();)
		{
			if (*it == nType)
			{
				it = m_arrDataType.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void SetWriteFlag(bool bFlag)
	{
		m_bWriteFlag = bFlag;
	}
protected:
	virtual void connectPropertyManager(QtVariantPropertyManager *manager);
	virtual QWidget *createEditor(QtVariantPropertyManager *manager, QtProperty *property,
		QWidget *parent);
	virtual void disconnectPropertyManager(QtVariantPropertyManager *manager);
	private slots:
	void slotPropertyChanged(QtProperty *property, const QVariant &value);
	void slotPropertyAttributeChanged(QtProperty *property, const QString &attribute, const QVariant &value);
	void slotSetValue(const QString &value);
	void slotEditorDestroyed(QObject *object);
private:
	QMap<QtProperty *, QList<CBindDataWgt *> > theCreatedEditors;
	QMap<CBindDataWgt *, QtProperty *> theEditorToProperty;
	//
	//°óµãÊý¾Ý
	std::vector<int> m_arrDataType;
	//
	bool m_bWriteFlag;

};

class CBindInfoManager : public QtVariantPropertyManager
{
	Q_OBJECT
public:
	CBindInfoManager(QObject *parent = 0): QtVariantPropertyManager(parent)
	{ }

	virtual QVariant value(const QtProperty *property) const;
	virtual int valueType(int propertyType) const;
	virtual bool isPropertyTypeSupported(int propertyType) const;

	virtual QStringList attributes(int propertyType) const;
	virtual int attributeType(int propertyType, const QString &attribute) const;
	virtual QVariant attributeValue(const QtProperty *property, const QString &attribute);


	static int filePathTypeId();
	public slots:
	virtual void setValue(QtProperty *property, const QVariant &val);
	virtual void setAttribute(QtProperty *property,
		const QString &attribute, const QVariant &value);
protected:
	virtual QString valueText(const QtProperty *property) const;
	virtual void initializeProperty(QtProperty *property);
	virtual void uninitializeProperty(QtProperty *property);
private:
	struct Data {
		QString value;
		QString filter;
	};
	QMap<const QtProperty *, Data> theValues;
};

#endif