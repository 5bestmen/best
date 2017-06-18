#ifndef LOGICPROPERTYWGT_H
#define LOGICPROPERTYWGT_H

#include <QWidget>
#include <memory>
#include "ui_logicpropertywgt.h"
#include "seqdefine.h"

class QTableWidgetItem;
class CBinProperty;
class CLogicPropertyWgt : public QWidget
{
    Q_OBJECT

public:
    CLogicPropertyWgt(QWidget *parent = 0);
    ~CLogicPropertyWgt();
    //更新表格中内容
    //void UpdateItemPro(const QList<LOGICITEMPRO>& lstInput, const QList<LOGICITEMPRO>& lstOutput);
    //隐藏 input数量
    void SetInputNumHidden();
    //设置item类型编码
    void SetItemTypeCode(int nCode);
    //设置子程序列表
    void SetChildLst(const QStringList &lstChildNames);
    //
    void UpdateItemDetail(std::vector<std::shared_ptr<CBinProperty>> lstInput, std::vector<std::shared_ptr<CBinProperty>> lstOutput);
    //
    void SetEnoFlag(bool bEnoFlag);
    //
    void SetComment(const QString &strComment);
	//更新绑定信息
	void UpdateBindInfo(const QString &strBindValue);

public slots:
    void Slot_Conform();
    //取消
    void Slot_Cancel();
    //双击
    void Slot_DoubleClickTableItem(QTableWidgetItem *pItem);
signals:
    void Signal_ConformComment(const QString &strComment);
    //显示  en/eno 信号  0不显示   1显示
    void Signal_EnoDisplay(int nFlag);
	//更改的绑点信息
	void Signal_ChangeBindValue(QMap<int, QString> &mapBindValue);
private:
    Ui::CLogicPropertyWgt ui;
    //注释
    QString m_strComment;
    //item类型编码
    int m_nItemTypeCode;
    //子程序名称列表
    QStringList m_lstChildNames;
    //
    bool m_bEnoFlag;
	//
	//绑点数据
	std::vector<int> m_arrDataType;
	//
	std::vector<std::shared_ptr<CBinProperty>> m_pBinPro;
	//绑点信息
	QMap<int, QString> m_mapBindValues;
};

#endif // LOGICPROPERTYWGT_H
