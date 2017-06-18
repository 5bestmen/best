#ifndef FTU_MODULE_H
#define FTU_MODULE_H
#include <QMap>
#include "f220_global.h"

#include "devicestudio/plugin_module.h"
 
class IMainWindow;
class CRealTimeBinary;
class CRealTimeAnalog;
class CRealTimeSOE;
class CRealTimeKwh;
class CCommThread;
class CChoosePointsWgt;
class CRemotePonitTableAnalyse;
class CInfoConfigWgt;
class CPointInfo;
class CRemoteControlWgt;
class CFixdDeploy;
class QStandardItem;
class CRecordWgt;
class CMaintecanceWgt;
class CHistoryWgt;
class CCmdHistoryWgt;
class QProcess;
class CAnalyse104Msg;
class QDockWidget;
class CSycsConfigDlg;
class CLcdoperatorWgt;
class CFaultEventWgt;
class CWebDeviceWgt;
class CFileTransWgt;

class F220SHARED_EXPORT CFtuModule : public IPluginModule
{
	Q_OBJECT
public:
    CFtuModule();
	~CFtuModule();
	//往树结构中插入数据
	void CreateTreeItem();
	bool CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc);
    IMainModule *GetMainModule();
    QString GetDeviceName();
    //获取Device addr
    int GetDeviceAddr();
    int GetBinaryStartAddr();
    int GetAnalogStartAddr();
    int GetKwhStartAddr();
    int GetControlStartAddr();
    //组态工具包调用 
    void InitCallZt();
    //处理对时报文    nDFlag 1:发送   2接收
    void AnalyseSycsTime(const QByteArray &btData,int nLength,int nDFlag);
	//设置定值区号
	void SetFixCode(int nCode)
	{
		m_nFixAreaCode = nCode;
	}

	int GetFixCode()
	{
		return m_nFixAreaCode;
	}
public:
	virtual void Init(IMainModule *pCore) ;
	virtual void UnInit() ;
	//设置加载的插件的名称
	virtual void SetLoadDeviceName(const QString &strDeviceName);
public slots:
	void Slot_ClickLeftTreeItem(const QModelIndex &mIndex);
	void Slot_CloseOneTabWidget(int iIndex);
	//网络socke错误
	void Slot_SocketError(QString strError);
	//socket连接成功
	void Slot_SocketSuccess();
	//收到总召响应
	void Slot_TotalCallResponseSuccess();
	//收到的信息
    void Slot_Recv16Data(QByteArray buff, int nLenght);
	//发送的信息
	void Slot_Send16Data(QByteArray buff);
	//遥控反馈
	void Slot_ControlFeedback(int iControlType, int iPointNum, int iOperateType, QString strStatus);
	//树右击菜单
	void Slot_ContextMenuRequest(const QPoint &cPoint);
	//配置action
	void Slot_SetConfigAct();
    //调用commconfig软件
    void Slot_CallCommConfig();
    //双击tab
    void Slot_DoubleClickTabBar(int nIndex);
    //调用总召
    void Slot_GeneralCall();
    //调用对时
    void Slot_SycsTimeAct();
    //调用电度
    void Slot_KwhCallTimeAct();
    //保存测点
    void Slot_SavePoints();
	//切换定值区号
	void Slot_SwitchFixArea();
	//读取定值区号
	void Slot_ReadFixArea();
private:
	//模块加载工具
	IMainModule *m_pMainModule;
	//主界面加载控件对象
	IMainWindow *m_pMainWindow;
	//树结构 编号  对应的tab页
	QMap<int, QWidget*> m_MIndexTab;
	//树结构 tab页  编号
	QMap<QWidget*, int> m_MTabIndex;
	//通信模块
	//ICommPlugin *m_pCommunicatePlugin;
	CCommThread *m_pCommThread;
	//实时遥信widget
	CRealTimeBinary *m_pRealTimeBinaryWgt;
	//实时遥测widget
	CRealTimeAnalog *m_pRealTimeAnalog;
	//SOE
	CRealTimeSOE *m_pRealTimeSoe;
	//kwh
	CRealTimeKwh *m_pRealTimeKwh;
	//选择测点widget
	CChoosePointsWgt *m_pChoosePointWgt;
	//点表
	CRemotePonitTableAnalyse *m_pPointTableAnalyse;
	//配置界面
	CInfoConfigWgt *m_pConfigWgt;
	//当前插件其实标号
	int m_iCurrentPluginIndex;
	//点表数据
	CPointInfo *m_pPointInfo;
	//插件对应的设备名称
	QString m_strDeviceName;
	//遥控
	CRemoteControlWgt *m_pReomteControl;
	//定值
	CFixdDeploy *m_pFixDeploy;
	//当前新增的节点
	QStandardItem *m_pPluginTopItem;
    //录波
    CRecordWgt *m_pRecord;
    //维护调试功能
    CMaintecanceWgt *m_pMaintencanceWgt;
    //历史数据
    CHistoryWgt *m_pHistory;
    //操作历史文件
    CCmdHistoryWgt *m_pCmdHistory;
    //调用进程
    QProcess *m_pCommProcess;
    //解析文件
    CAnalyse104Msg *m_pAnalyse104;
    //监控界面
    QDockWidget *m_pMonitorDockWgt;
    //时间同步配置界面
    CSycsConfigDlg *m_pSycsConfigWgt;
    //模拟液晶
    CLcdoperatorWgt *m_pLcdWgt;
    //故障事件
    CFaultEventWgt *m_FaultEventWgt;
	//web
	CWebDeviceWgt *m_pWebDevice;
	//定值区号
	int m_nFixAreaCode;
	//文件传输
	CFileTransWgt *m_pFileTransWgt;
};

#endif // FTU_MODULE_H
