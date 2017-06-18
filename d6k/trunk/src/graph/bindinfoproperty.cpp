#include "bindinfoproperty.h"


#include "binddatawgt.h"
#include "graphview.h"
#include "thresholdchoosewgt.h"

#include <QVariant>

CBindInfoFactory::~CBindInfoFactory()
{
	QList<CBindDataWgt *> editors = theEditorToProperty.keys();
	QListIterator<CBindDataWgt *> it(editors);
	while (it.hasNext())
		delete it.next();
}

void CBindInfoFactory::connectPropertyManager(QtVariantPropertyManager *manager)
{
	connect(manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
		this, SLOT(slotPropertyChanged(QtProperty *, const QVariant &)));
	connect(manager, SIGNAL(attributeChanged(QtProperty *, const QString &, const QVariant &)),
		this, SLOT(slotPropertyAttributeChanged(QtProperty *, const QString &, const QVariant &)));
	QtVariantEditorFactory::connectPropertyManager(manager);
}

QWidget *CBindInfoFactory::createEditor(QtVariantPropertyManager *manager,QtProperty *property, QWidget *parent)
{
	if (manager->propertyType(property) == ITEM_COLOR_RANGE)
	{
		CBindDataWgt *editor = new CBindDataWgt(parent);
		editor->SetTypeFalg(CBindDataWgt::ITEM_COLOR_RANGE);
		editor->SetWriteFlag(m_bWriteFlag);
		editor->SetArrDataTypes(m_arrDataType);
		editor->SetBindData(QString::number(manager->value(property).toLongLong()));
		editor->SetDynamicBrush(manager->value(property).toLongLong());
		//TODO
		theCreatedEditors[property].append(editor);
		theEditorToProperty[editor] = property;

		connect(editor, SIGNAL(Signal_ValueChanged(const QString &)), this, SLOT(slotSetValue(const QString &)));

		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;

	}
	else if (manager->propertyType(property) == ITEM_BIND_VAR )
	{
		CBindDataWgt *editor = new CBindDataWgt(parent);
		editor->SetTypeFalg(CBindDataWgt::ITEM_VAR);
		editor->SetWriteFlag(m_bWriteFlag);
		editor->SetArrDataTypes(m_arrDataType);

		editor->SetBindData(manager->value(property).toString());

		//TODO
		theCreatedEditors[property].append(editor);
		theEditorToProperty[editor] = property;

		connect(editor, SIGNAL(Signal_ValueChanged(const QString &)), this, SLOT(slotSetValue(const QString &)));

		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;

	}
	else if (manager->propertyType(property) == ITEM_TEXT_RANGE)
	{
		CBindDataWgt *editor = new CBindDataWgt(parent);
		editor->SetTypeFalg(CBindDataWgt::ITEM_TEXT_RANGE);
		editor->SetWriteFlag(m_bWriteFlag);
		editor->SetArrDataTypes(m_arrDataType);
		editor->SetBindData(QString::number(manager->value(property).toLongLong()));
		editor->SetDynamicBrush(manager->value(property).toLongLong());
		//TODO
		theCreatedEditors[property].append(editor);
		theEditorToProperty[editor] = property;

		connect(editor, SIGNAL(Signal_ValueChanged(const QString &)), this, SLOT(slotSetValue(const QString &)));

		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;

	}
	else if (manager->propertyType(property) == ITEM_MAP_RANGE)
	{
		CBindDataWgt *editor = new CBindDataWgt(parent);
		editor->SetTypeFalg(CBindDataWgt::ITEM_MAP_RANGE);
		editor->SetWriteFlag(m_bWriteFlag);
		editor->SetArrDataTypes(m_arrDataType);
		editor->SetBindData(QString::number(manager->value(property).toLongLong()));
		editor->SetDynamicBrush(manager->value(property).toLongLong());
		//TODO
		theCreatedEditors[property].append(editor);
		theEditorToProperty[editor] = property;

		connect(editor, SIGNAL(Signal_ValueChanged(const QString &)), this, SLOT(slotSetValue(const QString &)));

		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;

	}
	else if (manager->propertyType(property) == ITEM_BACKGROUND_IMAGE_CODE)
	{
		CBindDataWgt *editor = new CBindDataWgt(parent);
		editor->SetTypeFalg(CBindDataWgt::ITEM_BACKGROUND_CHOOSE);
		editor->SetWriteFlag(m_bWriteFlag);
		editor->SetArrDataTypes(m_arrDataType);

		editor->SetBindData(manager->value(property).toString());

		//TODO
		theCreatedEditors[property].append(editor);
		theEditorToProperty[editor] = property;

		connect(editor, SIGNAL(Signal_ValueChanged(const QString &)), this, SLOT(slotSetValue(const QString &)));

		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;

	}
	else if (manager->propertyType(property) == ITEM_PUISHBUTTON_ORDER_CODE)
	{
		CBindDataWgt *editor = new CBindDataWgt(parent);
		editor->SetTypeFalg(CBindDataWgt::ITEM_PUSHBUTTON_ORDER);
		editor->SetWriteFlag(m_bWriteFlag);
		editor->SetArrDataTypes(m_arrDataType);

		editor->SetBindData(manager->value(property).toString());

		//TODO
		theCreatedEditors[property].append(editor);
		theEditorToProperty[editor] = property;

		connect(editor, SIGNAL(Signal_ValueChanged(const QString &)), this, SLOT(slotSetValue(const QString &)));

		connect(editor, SIGNAL(destroyed(QObject *)), this, SLOT(slotEditorDestroyed(QObject *)));
		return editor;

	}

	return QtVariantEditorFactory::createEditor(manager, property, parent);
}

void CBindInfoFactory::disconnectPropertyManager(QtVariantPropertyManager *manager)
{
	disconnect(manager, SIGNAL(valueChanged(QtProperty *, const QVariant &)),
		this, SLOT(slotPropertyChanged(QtProperty *, const QVariant &)));
	disconnect(manager, SIGNAL(attributeChanged(QtProperty *, const QString &, const QVariant &)),
		this, SLOT(slotPropertyAttributeChanged(QtProperty *, const QString &, const QVariant &)));
	QtVariantEditorFactory::disconnectPropertyManager(manager);
}

void CBindInfoFactory::slotPropertyChanged(QtProperty *property,
	const QVariant &value)
{
	if (!theCreatedEditors.contains(property))
		return;

	QList<CBindDataWgt *> editors = theCreatedEditors[property];
	QListIterator<CBindDataWgt *> itEditor(editors);
	while (itEditor.hasNext())
	{
		itEditor.next()->SetBindData(value.toString());
	}
	//ToDO
	
}

void CBindInfoFactory::slotPropertyAttributeChanged(QtProperty *property,
	const QString &attribute, const QVariant &value)
{
	if (!theCreatedEditors.contains(property))
		return;

	if (attribute != QLatin1String("filter"))
		return;

	QList<CBindDataWgt *> editors = theCreatedEditors[property];
	QListIterator<CBindDataWgt *> itEditor(editors);
	//TODO
 	while (itEditor.hasNext())
 		itEditor.next()->SetBindData(value.toString());
}

void CBindInfoFactory::slotSetValue(const QString &value)
{
	QObject *object = sender();
	QMap<CBindDataWgt *, QtProperty *>::ConstIterator itEditor =
		theEditorToProperty.constBegin();
	while (itEditor != theEditorToProperty.constEnd()) 
	{
		if (itEditor.key() == object) 
		{
			QtProperty *property = itEditor.value();
			QtVariantPropertyManager *manager = propertyManager(property);
			if (!manager)
				return;
			manager->setValue(property, value);
			return;
		}

		itEditor++;
	}
}

void CBindInfoFactory::slotEditorDestroyed(QObject *object)
{
	QMap<CBindDataWgt *, QtProperty *>::ConstIterator itEditor =
		theEditorToProperty.constBegin();
	while (itEditor != theEditorToProperty.constEnd()) {
		if (itEditor.key() == object) {
			CBindDataWgt *editor = itEditor.key();
			QtProperty *property = itEditor.value();
			theEditorToProperty.remove(editor);
			theCreatedEditors[property].removeAll(editor);
			if (theCreatedEditors[property].isEmpty())
				theCreatedEditors.remove(property);
			return;
		}
		itEditor++;
	}
}
















class FilePathPropertyType
{
};

Q_DECLARE_METATYPE(FilePathPropertyType)

int CBindInfoManager::filePathTypeId()
{
	return qMetaTypeId<FilePathPropertyType>();
}

bool CBindInfoManager::isPropertyTypeSupported(int propertyType) const
{
	if (propertyType == ITEM_COLOR_RANGE || propertyType == ITEM_BIND_VAR 
		|| propertyType == ITEM_TEXT_RANGE || propertyType == ITEM_MAP_RANGE || propertyType ==  ITEM_BACKGROUND_IMAGE_CODE || propertyType == ITEM_PUISHBUTTON_ORDER_CODE)
		return true;
	return QtVariantPropertyManager::isPropertyTypeSupported(propertyType);
}

int CBindInfoManager::valueType(int propertyType) const
{
	if (propertyType == ITEM_COLOR_RANGE || propertyType == ITEM_BIND_VAR 
		|| propertyType == ITEM_TEXT_RANGE || propertyType == ITEM_MAP_RANGE || propertyType == ITEM_BACKGROUND_IMAGE_CODE || propertyType == ITEM_PUISHBUTTON_ORDER_CODE)
		return QVariant::String;
	return QtVariantPropertyManager::valueType(propertyType);
}

QVariant CBindInfoManager::value(const QtProperty *property) const
{
	if (theValues.contains(property))
		return theValues[property].value;
	return QtVariantPropertyManager::value(property);
}

QStringList CBindInfoManager::attributes(int propertyType) const
{
	if (propertyType == ITEM_COLOR_RANGE || propertyType == ITEM_BIND_VAR 
		|| propertyType == ITEM_TEXT_RANGE || propertyType == ITEM_MAP_RANGE || propertyType == ITEM_BACKGROUND_IMAGE_CODE || propertyType == ITEM_PUISHBUTTON_ORDER_CODE)
	{
		QStringList attr;
		attr << QLatin1String("filter");
		return attr;
	}
	return QtVariantPropertyManager::attributes(propertyType);
}

int CBindInfoManager::attributeType(int propertyType, const QString &attribute) const
{
	if (propertyType == ITEM_COLOR_RANGE || propertyType == ITEM_BIND_VAR
		|| propertyType == ITEM_TEXT_RANGE || propertyType == ITEM_MAP_RANGE || propertyType == ITEM_BACKGROUND_IMAGE_CODE || propertyType == ITEM_PUISHBUTTON_ORDER_CODE)
	{
		if (attribute == QLatin1String("filter"))
			return QVariant::String;
		return 0;
	}
	return QtVariantPropertyManager::attributeType(propertyType, attribute);
}

QVariant CBindInfoManager::attributeValue(const QtProperty *property, const QString &attribute)
{
	if (theValues.contains(property)) {
		if (attribute == QLatin1String("filter"))
			return theValues[property].filter;
		return QVariant();
	}
	return QtVariantPropertyManager::attributeValue(property, attribute);
}

QString CBindInfoManager::valueText(const QtProperty *property) const
{
	if (theValues.contains(property))
		return theValues[property].value;
	return QtVariantPropertyManager::valueText(property);
}

void CBindInfoManager::setValue(QtProperty *property, const QVariant &val)
{
	if (theValues.contains(property)) {
		if (val.type() != QVariant::String && !val.canConvert(QVariant::String))
			return;
		QString str = val.value<QString>();
		Data d = theValues[property];
		if (d.value == str)
			return;
		d.value = str;
		theValues[property] = d;
		emit propertyChanged(property);
		emit valueChanged(property, str);
		return;
	}
	QtVariantPropertyManager::setValue(property, val);
}

void CBindInfoManager::setAttribute(QtProperty *property,
	const QString &attribute, const QVariant &val)
{
	if (theValues.contains(property)) {
		if (attribute == QLatin1String("filter")) {
			if (val.type() != QVariant::String && !val.canConvert(QVariant::String))
				return;
			QString str = val.value<QString>();
			Data d = theValues[property];
			if (d.filter == str)
				return;
			d.filter = str;
			theValues[property] = d;
			emit attributeChanged(property, attribute, str);
		}
		return;
	}
	QtVariantPropertyManager::setAttribute(property, attribute, val);
}

void CBindInfoManager::initializeProperty(QtProperty *property)
{
	if (propertyType(property) == ITEM_COLOR_RANGE || propertyType(property) == ITEM_BIND_VAR 
		|| propertyType(property) == ITEM_TEXT_RANGE || propertyType(property) == ITEM_MAP_RANGE || propertyType(property) == ITEM_BACKGROUND_IMAGE_CODE || propertyType(property) == ITEM_PUISHBUTTON_ORDER_CODE)
		theValues[property] = Data();
	QtVariantPropertyManager::initializeProperty(property);
}

void CBindInfoManager::uninitializeProperty(QtProperty *property)
{
	theValues.remove(property);
	QtVariantPropertyManager::uninitializeProperty(property);
}

