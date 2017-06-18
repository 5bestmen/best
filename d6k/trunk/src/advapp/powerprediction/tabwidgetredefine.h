#ifndef TABWIDGET_REDEFINE_H
#define TABWIDGET_REDEFINE_H



#include <QTabWidget>
#include <functional>
#include <QMap>

struct TabKeyStruct
{
	TabKeyStruct()
	{
		TagName.clear();
		Type = -1;
	}

	QString TagName;
	int Type;
	QString BigTagName;

	bool operator<(const TabKeyStruct& a)const
	{
		if (this->Type < a.Type)
		{
			return true;
		}
		else if (this->Type == a.Type)
		{
			if (this->TagName < a.TagName)
			{
				return true;
			}
			else if (this->TagName == a.TagName)
			{
				if (this->BigTagName < a.BigTagName)
				{
					return true;
				}
			}
		}

		return false;
	}
};

class CTabWidget : public QTabWidget
{
	Q_OBJECT

public:
	CTabWidget(QWidget *parent);
	~CTabWidget();

	void AddTab(QWidget *widget, const QString &strTitle, TabKeyStruct &keyInfo/*, const int &nType, const QString &strTagName, const QString &strBigTagName*/, QIcon &icon = QIcon())
	{
		m_mapWidget.insert(keyInfo, widget);
		m_mapTabKey.insert(widget, keyInfo);

		addTab(widget, icon, strTitle);

		setCurrentWidget(widget);

		QString strType = QString("%1").arg(keyInfo.Type);

		widget->setObjectName(strType);
	}
	
	void AddTab(QWidget *widget, const QString &strTitle, const int &nType, const QString &strTagName, QIcon &icon = QIcon())
	{
		TabKeyStruct info;
		info.TagName = strTagName;
		info.Type = nType;

		m_mapWidget.insert(info, widget);
		m_mapTabKey.insert(widget, info);

		addTab(widget, icon, strTitle);

		setCurrentWidget(widget);

		QString strType = QString("%1").arg(nType);

		widget->setObjectName(strType);
	}

	bool CTabWidget::IsWidgetInTab(const QString &strTagName, const int &nType, const QString &strBigTagName = "");


	void SetTabName(const QString &strTagName, const int &nType, const QString &strLastTag);

	void SetTabName(const TabKeyStruct &newKey, const TabKeyStruct &lastKey);


	void ClearMap()
	{
		m_mapWidget.clear();
		m_mapTabKey.clear();
	}

	QMap<QWidget *, TabKeyStruct> &GetWidgetInfo()
	{
		return m_mapTabKey;
	}

	bool RefeshWidget(int nType);

	std::function< int(void* pData)> m_fnBeforeCloseTab;
	void *m_pBeforeCloseTabData;

	bool ModifyBigTagName(const QString &strBigTagName, int nType, const QString &strLastBigTagName);

signals:
	void SendCloseWidget(QWidget *pWidget);

	public slots:
	void closeTileWidget(int index);
	void tabChanged(int index);

private:
	QMap<TabKeyStruct, QWidget *> m_mapWidget;
	QMap<QWidget *, TabKeyStruct> m_mapTabKey;
};




#endif  //TABWIDGET_REDEFINE_H
