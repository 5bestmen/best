#ifndef TREE_ITEM_DEFINE_H
#define TREE_ITEM_DEFINE_H

//左边树结构 
enum DS_TREEDEF
{
    TREE_FTU_ITEM = 30,
    //选择测点
    TREE_CHOOSE_OBSERVE_POINTS = 300,
    //遥信
    TREE_REMOTE_SIGNALITEM = 301,
    //遥测
    TREE_REMOTE_MEASUREITEM = 302,

    //遥控
    TREE_REMOTE_CONTROLITEM = 303,

    //SOE
    TREE_REMOTE_SOE_ITEM = 304,
    //Kwh
    TREE_REMOTE_KWH_ITEM = 305,
    //fix
    TREE_FIX_DEV_CONFIG = 306,
    //record 录波
    TREE_RECORD_ITEM = 307,
    //维护调试功能
    TREE_DEBUG_ITEM = 308,
    //历史数据查看
    TREE_HISTORY_ITEM = 309,
    //操作历史数据
    TREE_CMD_HISTORY_ITEM = 400,
    //模拟液晶
    TREE_LED_ITEM         = 500,
    //故障事件
    TREE_MAL_ITEM         = 501,
	//web
	TREE_WEB_ITEM        = 601,
	//file
	TREE_FILE_ITEM       = 701,
};


// 总共用32 位表示
// 最高 2位 表示 插件序号

enum DS_TREEITEM
{
	TREE_LEVEL_DEF    = Qt::UserRole+100,
	TREE_PLUGIN_ROOT     = 100 ,  //<! 插件的根菜单

	TREE_PLUGIN_1  = 0x01000000,
	TREE_PLUGIN_1_LEAF1_M1 = 0x01010000,  //!< 插件1的1级子菜单1


//	TREE_PLUGIN_CHILD_2  = 300,
//	TREE_PLUGIN_CHILD_3  = 400,


};

 



#endif
