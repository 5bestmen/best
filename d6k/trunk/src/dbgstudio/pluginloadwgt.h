#ifndef PLUGINLOADWGT_H
#define PLUGINLOADWGT_H

#pragma execution_character_set("utf-8")

#define PLUGIN_DLL_FILE  "/ini/pluginconfig.xml"
#include <QWidget>

class QTableWidget;
class QIODevice;

class CPluginLoadWgt : public QWidget
{
	Q_OBJECT

public:
	CPluginLoadWgt(QWidget *parent=0);
	~CPluginLoadWgt();
public:
	enum 
	{
		COL_NUMBER = 0, //!< 序号
		COL_NAME = 1,
		COL_VER = 2,
		COL_COMMENT = 3,
		COL_DLL = 4,

		COL_MAX 
	};
	//打开文件
	void OpenPluginFile(const QString &strFilename);
	//解析文件
	void AnalyseXmlData(QIODevice *pDevice);
	//往表格中插入数据
	void InsertDataToTable(const QString &strNum, const QString &strName, const QString &strCommon, const QString &strVersion, const QString &strDll);
signals:
	void Signal_NeedLoadInfo(const QString &strDllName,const QString &strPluginName);
	
private slots:
    //确定
    void Slot_ClickConform();


private:
	QTableWidget *m_pTableWgt;
	
};

#endif // PLUGINLOADWGT_H
