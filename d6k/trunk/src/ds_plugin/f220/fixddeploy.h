#ifndef FIXDDEPLOY_H
#define FIXDDEPLOY_H

#include <QWidget>
#include <QMap>
#include "asdu_info.h"
#include "com103.h"

struct s_DevItems
{
	QString valueType;
	int defalutVlaue;
	QString ItemName;
	QString ItemDesc;
	QString rangeFt;
	//类型标识
	int TypeCode;
	//数据长度
	int DataLength;
};
struct s_DevParam 
{
	QString strDevParamName;
	int iBeginAddress;
	int iDesplay;
	QList<s_DevItems> lstItems;
};

//定值地址定义
// #define DZ_BG_NUM          0x4001                   //装置写入参数起始地址
// #define  DZ_READ_BG_NUM    0x4001                   //装置读取参数起始地址

//固有参数
#define  DZ_GYCS_ADDR 0x8001
//装置参数
#define  DZ_DEVICE_ADDR  0x8020
//保护定值参数
#define  DZ_PROTECT_ADDR 0x8220

// #define BHDZCS_BG_NUM      0x6301                  //保护定值参数起始地址
// #define YXCS_BG_NUM        0x6401                  //遥信参数起始地址
// #define XDCGLCS_BG_NUM     0x6501                  //蓄电池管理参数起始地址
// 
// #define JLJDCS_BG_NUM      0x5201                  //交流精度参数起始地址
// #define ZLCS_BG_NUM        0x5501                  //直流参数起始地址
// #define CKTXCS_BG_NUM      0x5601                  //串口通讯参数起始地址
// #define ZLJDCS_BG_NUM      0x5701                  //直流精度参数起始地址
// #define GGXYCS_BG_NUM      0x5801                  //公共协议参数起始地址
// #define TSXYCS_BG_NUM      0x5901                  //特殊协议参数起始地址


class QTabWidget;
class QTableWidget;
class QTableWidgetItem;
class CCommThread;
class IMainModule;

class CFixdDeploy : public QWidget
{
	Q_OBJECT
public:
	CFixdDeploy(CCommThread *pCommunicate, IMainModule *pCore,const QString &deviceName,QWidget *parent = 0);
	~CFixdDeploy();
	//初始化界面
	bool InitWgt(const QString &strFileName);
	void AnalyseXmlData(QIODevice *pDevice);
	//往表中写入数据
	void UpdateDataToWgt(const QList<s_DevParam> & lstFixData);
	//发送定值命令接口
	void SendOrderMsg();
	enum 
	{
		UNDISPLAY=0,
		DISPLAY
	};
	//设置table属性
	void SetTableAttr(QTableWidget* pCurrentTable);
	//加载数据
	void InitDataToTable(QTableWidget* pCurrentTable, const s_DevParam &devParamDatas);
	//更新定值参数
	void UpdateDevData(QTableWidget *pCurrentTable, int iNumber, const QString &strValue);
	//根据名称返回编号
	int GetBeginNumFromName(const QString &strName);
	//获取选中的行号
	QList<int> GetSelectRows(QTableWidget *pTableWidget);
    //排序
    void ChangeItemSort();
    //设置设备名称
    void SetDeviceName(const QString strDeviceName);
    //更新item
    void UpdateDevConfigFile(QByteArray btArray);
    //往装置参数中填充数据
    void UpdateDeviceParam(QTableWidget *pTable,tDownloadParam &DeviceParam);
    //往保护定值参数中填充数据
    void UpdateProtectParam(QTableWidget *pTable, tDownloadParam &DeviceParam);
    //往遥信参数定值中填充数据
    void UpdateBinaryParam(QTableWidget *pTable, tDownloadParam &DeviceParam);
    //往蓄电池管理中填充数据
    void UpdateElecParam(QTableWidget *pTable, tDownloadParam &DeviceParam);
    //设置工程名称
    void SetPorjectName(const QString &strPrjName);
    //读取配置文件中value
    void SetConfigBinValue(const QString &strFilename);
    //读取值写入
    void InputBinDataToTable(const tDownloadParam &tParam);
    //写入数据
    void InputDeviceToTable(QTableWidget *pTable, const tDownloadParam &DeviceParam);
    //写入保护定值参数
    void InputProtectToTable(QTableWidget *pTable, const tDownloadParam &DeviceParam);
    //写入遥信参数
    void InputBinaryToTable(QTableWidget *pTable, const tDownloadParam &DeviceParam);

private slots:
    //双击表格
    void Slot_DoubleClickTableItem(QTableWidgetItem *item);
	//右击菜单槽函数
	void Slot_ContextMenuRequest(const QPoint &pointCurrent);
	//更新数据
	void Slot_updateFixData(QMap<int, short> mapAddrData);
	//更新数据
	void Slot_ReadFixData(DEV_BASE &devRead );
	//发送需要更新的点  请求数据
	void Slot_SendUpdatePoints();
	//发送所有的点      请求数据
	void Slot_SendUpateAllPoints();
	//发送现有的数据    发送数据
	void Slot_SendCurrentPoints();
	//发送所有的数据
	void Slot_SendCurrentAllPoints();
	//定值设定返回信息 48/136    6/7   1/0  
	void Slot_WriteFeedBack(int iControlType, int iCot, int iQos);
	//超时
	void Slot_FixWaitTimeOut();
	//读取超时
	void Slot_FixReadTimeOut();
    //保存定值配置文件
    void Slot_SaveFixConfigAct();

private:
	//通信插件
	CCommThread *m_pCommunicate;
	//新增的tabwidget
	QTabWidget *m_pTabWgt;
	//tableIndex 和 对应的table
	QMap<int, QTableWidget*> m_MAPTabindexToTable;
	//对应的名称和indx
	QMap<QString, int> m_MapTableNameToIndex;
	//文件读取到的数据
	QList<s_DevParam> m_lstFixData;

	//待激活的命令
	QList<DEV_BASE> m_lstBaseInfo;
	//定值设置超时
	QTimer *m_pFixTimeOut;
	//定时读取超时
	QTimer *m_pFixReadTimeOut;
	//模块加载工具
	IMainModule *m_pMainModule;
	//模块名称
	QString m_strDeviceName;
    //当前编号
    int m_nCurrentDevAddr;
    //保护定值起始地址
    int m_nProtectDevBgAddr;
    //装置参数其实地址
    int m_nBinaryDevBgAddr;
    //蓄电池管理
    int m_nBatteryDevBgAddr;
    //定值配置中 valueType
    QString m_strConfigValueType;
    //定值配置中 version
    QString m_strConfigVersion;
    //定值文件名称
    QString m_strDevFilePath;
	//数据类型映射
	QMap<int, QString> m_mapTypeCode;
};

#endif // FIXDDEPLOY_H
