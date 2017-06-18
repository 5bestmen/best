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
    //���±��������
    //void UpdateItemPro(const QList<LOGICITEMPRO>& lstInput, const QList<LOGICITEMPRO>& lstOutput);
    //���� input����
    void SetInputNumHidden();
    //����item���ͱ���
    void SetItemTypeCode(int nCode);
    //�����ӳ����б�
    void SetChildLst(const QStringList &lstChildNames);
    //
    void UpdateItemDetail(std::vector<std::shared_ptr<CBinProperty>> lstInput, std::vector<std::shared_ptr<CBinProperty>> lstOutput);
    //
    void SetEnoFlag(bool bEnoFlag);
    //
    void SetComment(const QString &strComment);
	//���°���Ϣ
	void UpdateBindInfo(const QString &strBindValue);

public slots:
    void Slot_Conform();
    //ȡ��
    void Slot_Cancel();
    //˫��
    void Slot_DoubleClickTableItem(QTableWidgetItem *pItem);
signals:
    void Signal_ConformComment(const QString &strComment);
    //��ʾ  en/eno �ź�  0����ʾ   1��ʾ
    void Signal_EnoDisplay(int nFlag);
	//���ĵİ����Ϣ
	void Signal_ChangeBindValue(QMap<int, QString> &mapBindValue);
private:
    Ui::CLogicPropertyWgt ui;
    //ע��
    QString m_strComment;
    //item���ͱ���
    int m_nItemTypeCode;
    //�ӳ��������б�
    QStringList m_lstChildNames;
    //
    bool m_bEnoFlag;
	//
	//�������
	std::vector<int> m_arrDataType;
	//
	std::vector<std::shared_ptr<CBinProperty>> m_pBinPro;
	//�����Ϣ
	QMap<int, QString> m_mapBindValues;
};

#endif // LOGICPROPERTYWGT_H
