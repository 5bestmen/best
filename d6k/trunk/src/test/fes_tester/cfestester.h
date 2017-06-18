#pragma once
#include "datatypes.h"
#include <QtWidgets/QMainWindow>
#include <QLibrary>
#include <functional>
#include <iostream>
#include <memory>
#include <deque>

#include "ui_cfestester.h"

struct  FileInfo
{
	QString m_szName;
	QString m_szPath;
};

enum  DataType
{
	AO_TYPE = 1,
	DO_TYPE = 2,
	MSGC_TYPE=3,
};

class CFesTester : public QMainWindow
{
	Q_OBJECT

public:
	CFesTester(QWidget *parent = Q_NULLPTR);

private:
	Ui::CFesTesterClass ui;

public:
	void InitProjView();
	bool IsFileHasAdded(const QString& szName);
public Q_SLOTS:
	void slot_OnPathSelection();
	void slot_OnLoad();
	void slot_OnUnLoad();
	void slot_OnRadioChanged();
	void slot_OnPathComBoxChanged(int nIndex);
	void slot_OnAOComBoxChannelChanged(int );
	void slot_OnAOInfoComBoxChanged(int );
	void slot_OnDOComBoxChannelChanged(int );
	void slot_OnDOInfoComBoxChanged(int );
	void slot_OnMsgComBoxChannelChanded(int );

	void slot_OnOk();
	void slot_OnCancel();
private:
	QString m_szFilePath;
	QLibrary m_LibFes;
	QLineEdit* m_pLineEdit;
	int m_nMaxRecentFileNum;
	int m_nRadioIndex;
	int32u m_nMyNode;
	std::deque< std::shared_ptr<FileInfo> > m_FileInfoQue;

private:
	std::function < int(const char*, unsigned int) > m_fnScdsvcMain;
	std::function < int() > m_fnScdsvcStop;
	std::function < int() > Functional;

private:
	void SetAoFuncEnable(bool);
	void SetDoFuncEnable(bool);
	void SetMsgFuncEnable(bool);
	void InitAOChannelFunc();
	void InitDOChannelFunc();
	void InitMsgChannelFunc();

	void InitDoInfo(int32u nChannelID);
	void InitAoInfo(int32u nChannelID);
	void InitMsgInfo(int32u nChannelID);
};
