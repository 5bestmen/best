#ifndef TREE_ITEM_DEFINE_H
#define TREE_ITEM_DEFINE_H

//������ṹ 
enum DS_TREEDEF
{
    TREE_FTU_ITEM = 30,
    //ѡ����
    TREE_CHOOSE_OBSERVE_POINTS = 300,
    //ң��
    TREE_REMOTE_SIGNALITEM = 301,
    //ң��
    TREE_REMOTE_MEASUREITEM = 302,

    //ң��
    TREE_REMOTE_CONTROLITEM = 303,

    //SOE
    TREE_REMOTE_SOE_ITEM = 304,
    //Kwh
    TREE_REMOTE_KWH_ITEM = 305,
    //fix
    TREE_FIX_DEV_CONFIG = 306,
    //record ¼��
    TREE_RECORD_ITEM = 307,
    //ά�����Թ���
    TREE_DEBUG_ITEM = 308,
    //��ʷ���ݲ鿴
    TREE_HISTORY_ITEM = 309,
    //������ʷ����
    TREE_CMD_HISTORY_ITEM = 400,
    //ģ��Һ��
    TREE_LED_ITEM         = 500,
    //�����¼�
    TREE_MAL_ITEM         = 501,
	//web
	TREE_WEB_ITEM        = 601,
	//file
	TREE_FILE_ITEM       = 701,
};


// �ܹ���32 λ��ʾ
// ��� 2λ ��ʾ ������

enum DS_TREEITEM
{
	TREE_LEVEL_DEF    = Qt::UserRole+100,
	TREE_PLUGIN_ROOT     = 100 ,  //<! ����ĸ��˵�

	TREE_PLUGIN_1  = 0x01000000,
	TREE_PLUGIN_1_LEAF1_M1 = 0x01010000,  //!< ���1��1���Ӳ˵�1


//	TREE_PLUGIN_CHILD_2  = 300,
//	TREE_PLUGIN_CHILD_3  = 400,


};

 



#endif
