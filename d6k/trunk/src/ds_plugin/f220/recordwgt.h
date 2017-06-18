#ifndef RECORDWGT_H
#define RECORDWGT_H

#include <QWidget>
#include <QXmlStreamReader>
#include "ui_recordwgt.h"

struct RECORDITEM
{
    int nFileIndex;         //文件编号
    QString strFileName;    //文件名称
    QString strDestriber;   //文件描述
};

struct RECORINFO
{
    int nIndex;                      //目录编号
    QString strName;                 //目录名称
    QList<RECORDITEM> lstItems;      //目录下文件信息
};

class CCommThread;
class IMainModule;
class QProcess;

class CRecordWgt : public QWidget
{
    Q_OBJECT

public:
    CRecordWgt(CCommThread *pCommunicate, IMainModule *pCore, const QString &deviceName,QWidget *parent = 0);
    ~CRecordWgt();
    //解析xml文件
    bool AnalyseRecordFile(const QString &strFileName);
    bool AnalyseXmlData(QIODevice *pDevice);
    //读取item
    void AnalyseItemData(QXmlStreamReader &xmlReader, int nIndex, const QString &strPath);
    //树中插入数据
    void InserDataToTree(int nIndex,const QString &strCatalog,int nTreeIndex);
    //设置录波配置文件名
    void SetRecoredConifgName(const QString &strFileName);
    //设置现在目录
    void SetRecordPath(const QString &strPath);
    //更新文件列表
    void UpdateRecordFileList();
public slots:
    //点击树
    void Slot_ClickTreeItem(QTreeWidgetItem *item, int nColumn);
    //table 右击菜单
    void Slot_ContextMenuRequest(const QPoint &point);
    //tree 右击菜单
    void Slot_treeContextMenuRequest(const QPoint &point);
    //table 右击菜单
    void Slot_TableContexeMenuRequest(const QPoint &point);
    //down action
    void Slot_DownFileAct();
    //更新配置文件
    void Slot_UpdateFileAct();
    //更新录波文件
    void Slot_UpdateRecordFileAct();
    //文件双击
    void Slot_DoubleClickFileList(QTableWidgetItem *pItem);
private:
    Ui::CRecordWgt ui;
    //文件index   详细信息
    QMap<int, RECORINFO> m_mapIndexFileInfo;
    //通信插件
    CCommThread *m_pCommunicate;
    //模块加载工具
    IMainModule *m_pMainModule;
    //模块名称
    QString m_strDeviceName;
    //录波配置文件名称
    QString m_strRecordConfigFile;
    //录波文件目录
    QString m_strRecordFilePath;
    //
    QProcess *m_pRecordProcess;
};

#endif // RECORDWGT_H
