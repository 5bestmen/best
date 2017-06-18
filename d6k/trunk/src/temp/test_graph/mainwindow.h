#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

namespace Ui {
class MainWindow;
}

class CGraphView;
class CGraphScene;
class QToolBox;
class QListWidget;
class QTableWidget;
class QTableWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
	void NewEmptyFile();

	void CreateMenus();
	void CreateToolBox();
protected:
	void CreateMenu();
	void CreateEditMenu();
	void CreateViewMenu();
	void CreateOptionMenu();
	void CreateWindowMenu();
	void CreateHelpMenu();

	void CreateLayerToolBar();

	void GetScreenInfo();

	void UpdateUI();

	void SetOtherItemsUnSelected(int nCurRow);
	void SetLayerVisible(unsigned int nLayIdx, bool bDisp);

	void SetLayerRowSelState(int nRow, bool bSelected);

public slots:
	void SelectDrawTool();

	void Simulation();

	void PrintGraph();

	void ExportImage();

	// ±à¼­
	void OnCopy();
	void OnPaste();
	void OnDelete();
	void OnCut();
	void OnWidgetsGroup();
	void OnWidgetsBreak();

	void OnAutoAlign();
	void OnLeftAlign();
	void OnRightAlign();
	void OnCenterHorzAlign();
	void OnTopAlign();
	void OnCenterVertAlign();
	void OnButtomAlign();

	void slotItemClicked(QTableWidgetItem*);
	void slotCellClicked(int ,int);

private:
	//unsigned int m_nDrawTool;

	QActionGroup *m_pDrawToolActGrp;

	QAction *m_pDrawToolSel;
	QAction *m_pDrawToolLine;
	QAction *m_pDrawToolRect;
	QAction *m_pDrawToolRound;
	QAction *m_pDrawToolRoundRect;


	CGraphView *m_pGraphView;
	CGraphScene *m_pGraphScene;


	QMenu *m_pFileMenu;
	QMenu *m_pEditMenu;
	QMenu *m_pElementMenu;
	QMenu *m_pDebugMenu;
	QMenu *m_pArrangeMenu;


	QMenu *m_pLayerMenu;

	QToolBar *m_pLayerToolBar;

// 	QMenu *m_editMenu;
// 	QMenu *m_viewMenu;
// 	QMenu *m_optionMenu;
// 	QMenu *m_windowMenu;
// 	QMenu *helpMenu;

	QToolBox *m_pToolBox;

	QTableWidget *m_pLayerList;

	QIcon m_iconEyeOn;
	QIcon m_iconEyeOff;
	QIcon m_iconLeft;
	QIcon m_iconNull;
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
