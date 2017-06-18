#ifndef DGETSELECTION_H
#define DGETSELECTION_H

#include <QDialog>
#include "ui_dgetselection.h"

typedef struct                 //最值
{
	quint32 maxVal;       //最大值
	quint32 maxValTime;   //最大值发生时间
	quint32 minVal;       //最小值
	quint32 minValTime;   //最小值发生时间

}MVal, *pMVal;

typedef struct              //越限值
{
	quint32 upLimitCount;  //越上限次数
	quint32 upLimitTime;   //越下限时间
	quint32 downLimitCount;//越下限次数
	quint32 downLimitTIme; //越下限时间
	quint32 uuLimitCount;  //越上上限次数
	quint32 uuLimitTime;   //越上上限时间
	quint32 ddLimitCount;  //越下下限次数
	quint32 ddLimitTime;   //越下下限时间
}OverVal, *pOverVal;

//时间选取
typedef struct
{
	QString m_StartTime;
	QString m_EndTime;
}A1;
//时间序列   
typedef struct
{
	QString timeIncrease;
	qint32 m_nIndex;
}B1;

class  CDataUint
{
public:
	CDataUint()
	{
		m_mVal = new MVal;
		memset(m_mVal, 0, sizeof(MVal));
		m_oVal = new OverVal;
		memset(m_oVal, 0, sizeof(OverVal));
		m_val = 9999;
		m_avgVal = 9999;
		m_sumVal = 9999;

	}
	~CDataUint()
	{
		delete m_mVal;
		m_mVal = NULL;
		delete m_oVal;
		m_oVal = NULL;
	}
public:
	quint32 m_val;               //值
	pMVal   m_mVal;              //最值 
	pOverVal m_oVal;             //越限值
	quint32 m_avgVal;            //平均值
	quint32 m_sumVal;            //统计值
	//时间序列
	QString m_szTimeIncrease;
	quint32 index;
} ;

class DGetSelection : public QDialog, public Ui::DGetSelection
{
	Q_OBJECT

public:
	explicit DGetSelection(QWidget *parent = 0);
	~DGetSelection();

public:
	CDataUint* GetResult() const
	{
		Q_ASSERT(m_pVal);
		return m_pVal;
	}
	void InitDay();
	void InitMonth();
	void InitYear();
	protected slots:
	void slot_on_ok();
	void slot_on_cancel();

	void slot_on_mostValChange(bool);
	void slot_on_overValChange(bool);
protected:
	void InitDialog();
	void SetMostValue(const bool& m_val);
	void SetOverValue(const bool& m_val);

public:
	CDataUint *m_pVal;
};

#endif // DGETSELECTION_H
