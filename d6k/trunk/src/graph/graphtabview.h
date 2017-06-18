#ifndef GRAPH_TAB_VIEW_H
#define GRAPH_TAB_VIEW_H

#include <QMainWindow>
#include <QPushButton>
#include "scadastudio/base.h"

class ICoreui;
class CGraphView;
class CGraphScene;
class CGraphFile;
class QTableWidget; 
class QTableWidgetItem;

class CGraphTabView : public QMainWindow, public IBase
{
	Q_OBJECT
public:
	CGraphTabView(ICoreui *pUi, QWidget *pParent);
	CGraphTabView(QWidget *pParent,CGraphFile *pFile);

	~CGraphTabView();

	void closeEvent(QCloseEvent * event);

	virtual void Save()
	{

	}
	virtual void Refresh()
	{

	}
	virtual void SetModifyFlag(bool bFlag)
	{
		m_bFlag = bFlag;
	}
	virtual bool GetModifyFlag()
	{
		return m_bFlag;
	}
	CGraphFile *GetGraphFile()
	{
		return m_pGraphFile;
	}
	void UpdateGraphLayerList(QTableWidget*pGraphLayerList);
	void UpdateUI(CGraphFile *pFile, QTableWidget*pGraphLayer);

	CGraphView *GetGraphView()
	{
		return m_pGraphView;
	}

	void SetOtherItemsUnSelected(int nCurRow);
	void SetLayerVisible(unsigned int nLayIdx, bool bDisp);

	void SetLayerRowSelState(int nRow, bool bSelected);

	void slotCellClicked(int, int);
protected:
    void resizeEvent(QResizeEvent *event);

public slots:
	void OnOk();

	void OnDelete();
	void slotItemClicked(QTableWidgetItem*);

private:
	CGraphView  *m_pGraphView;
	CGraphScene *m_pGraphScene;
	CGraphFile  *m_pGraphFile;
	bool   m_bFlag;
	ICoreui *m_pUi;
};

#endif // GRAPH_TAB_VIEW_H
