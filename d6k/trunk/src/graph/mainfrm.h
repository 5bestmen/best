#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H
#include <QMainWindow> 
#include <memory>
#include <vector>
class CGraphFile;
class QTreeWidget;
class CRealTimeTask;
class CGraphScene;
class CRealTimeView;
class QTreeWidgetItem;
class QDockWidget;
#define USER_ROLE_ROOT Qt::UserRole + 1
#define USER_GRP_ROLE  Qt::UserRole + 2
#define USER_FILE_ROLE Qt::UserRole + 3

typedef struct
{
	std::string szFileName;
	std::string szFilePath;

}GraphFileDataUnit;

using SIM_GRAPH_DEF = std::shared_ptr < GraphFileDataUnit >;


class CMainFrame : public QMainWindow 
{
	Q_OBJECT
public:
	CMainFrame(QWidget *pParent=nullptr);
	virtual ~CMainFrame();

	void closeEvent(QCloseEvent * event);

public:
	bool Initalize(std::vector < SIM_GRAPH_DEF >& arrFiles);
	void Run();
	void ShutDown();


public:
	void AddView(std::shared_ptr<CGraphFile>pGraphFile);
	void SetProjPath(const QString& szProjPath)
	{
		m_szProjPath = szProjPath;
	}
	QString& GetProjPath()  
	{
		return  m_szProjPath;
	}
protected:
	//加载文件由谁来实现?!
	void InitTreeWidget();
	void InitMainWindow();
protected:
    void resizeEvent(QResizeEvent *event);
	void SetCurrentFile(const QString& szName);
public slots: 
	void OnDelete();
	void slot_OnTreeDbClicked(QTreeWidgetItem *item, int column);
private:
	QTabWidget  *m_pTabWidget;
	QTreeWidget* m_pTreeWidget;
	QDockWidget * m_pDockWidget;
private:
	QString m_szProjPath;
	//当前页面
	std::shared_ptr< CGraphFile > m_pGraphFile;
	std::vector < SIM_GRAPH_DEF > m_arrFiles;

	std::vector<CRealTimeTask*> m_arrTasks;
};

#endif // GRAPH_TAB_VIEW_H
