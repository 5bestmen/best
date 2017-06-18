#ifndef CVIRTUALTABLEWGT_H
#define CVIRTUALTABLEWGT_H
#include <QTableWidgetItem>
#include "remoteponittableanalyse.h"

class CVirtualTableWgt : public QTableWidget
{
    Q_OBJECT
public:
    CVirtualTableWgt(QWidget *parent = 0);
    //设置类型
    void SetPointType(const QString &strPonitType);
    //选中区域测试
    void SelectAllRows(const QPoint &beginPoint, const QPoint &endPoint);
	//设置 sorce 或 des  
	void SetSourceTableWgt();
	void SetDescTableWgt();
	//获取当前table里面的值
	QMap<unsigned int, RPT> GetCurrentTableData();
	//清空数据
	void ClearAllData();
    //
    void SetSaveFileName(const QString &strFilaPath);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
public slots:
	//设置已提取状态槽函数
    void Slot_SetSelectedState(QList<unsigned int> lstIDs);
	//设置复原状态的槽函数
	void Slot_setRecoverState(QList<unsigned int> lstIDs);
    //右击保存
    void Slot_ContextMenuRequest(const QPoint &cPoint);
    //
    void Slot_SavaAction();
signals:
	//当有数据更新时 需要发出信号
	void Signal_UpdateTableInfo();
	//当有数据更新时  发送已存在的列表id 
	void Signal_ExistListID(QList<unsigned int>);
private:
    bool performDrag();
    QString selectionText() const;

    QString toHtml(const QString &plainText) const;
    QString toCsv(const QString &plainText) const;
    void fromCsv(const QString &csvText, QDropEvent *event);

    QPoint m_pStartPos;
    //类型名称  遥信  遥测  遥脉  等
    QString m_strPointType;
    //二次选中标识
    bool m_bSecondFlag;
	//已经存在的ID list 
	QList<unsigned int> m_lstObserveIDs;
	//需要删除的ID list
	QList<unsigned int> m_lstRecoverIDs;
    //需要保存的文件名称
    QString m_strFilename;
};

#endif // CVIRTUALTABLEWGT_H
