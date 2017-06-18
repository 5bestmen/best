//
//#ifndef  MENU_MACRO_H
//#define  MENU_MACRO_H
//
//QT_BEGIN_NAMESPACE
//      class QAction;
//      class QMenu;
//QT_END_NAMESPACE
//    //消息映射表
//#define BEGIN_MY_MSGMAP(theClass) \
//    void theClass::__Create_My_Qt_Msg_Map() \
//{\
//    statusBar()->showMessage(QObject::tr("Ready")); \
//    QMenuBar* pMenubar = this->menuBar();
//
//    //菜单
//#define BEGIN_MY_MENU(theMenuName)\
//{ \
//    QMenu* pMenuGroup = pMenubar->addMenu(QObject::tr(theMenuName)); \
//    QAction* pAction = NULL;
//
//#define END_MY_MENU \
//}
//
//#define ADD_MENU(theItemName,theHandlerClass,theHandlerName) \
//    pAction = new QAction(QObject::tr(theItemName), this);\
//    pMenuGroup->addAction(pAction);\
//    m_actions[theItemName] = pAction;\
//    this->connect(pAction, SIGNAL(triggered()), theHandlerClass, SLOT(theHandlerName()));
//
//#define ADD_TIP_MSG(theTipName)\
//    pAction->setStatusTip(QObject::tr(theTipName));              
//
//#define ADD_CHECK(theCheckState)\
//    pAction->setCheckable(true);\
//    pAction->setChecked(theCheckState);   
//
//#define CHECK_ITEM(theItemName,theCheckState) \
//    QAction * pAction = m_actions[theItemName];\
//    if(pAction != NULL)\
//{\
//    pAction->setChecked(theCheckState);\
//}
//
//
//#define ADD_MENU_EX(theIconName,theItemName,theHandlerClass,theHandlerName) \
//    pAction = new QAction(QIcon(theIconName), QObject::tr(theItemName), this); \
//    m_actions[theItemName] = pAction;\
//    pMenuGroup->addAction(pAction);\
//    pAction->setCheckable(true);\
//    this->connect(pAction, SIGNAL(triggered()), theHandlerClass, SLOT(theHandlerName()));
//
//#define ADD_MENU_SEPARATOR \
//    pMenubar->addSeparator();
//
//
//    //工具栏
//#define BEGIN_MY_TOOLBAR(theToolbarName)\
//{ \
//    QToolBar* pToolbar = addToolBar(QObject::tr(theToolbarName)); \
//    QAction* pAction = NULL;
//
//#define END_MY_TOOLBAR \
//}
//
//#define ADD_TOOLBAR(theIconName,theItemName,theHandlerName) \
//    pAction = new QAction(QIcon(theIconName), QObject::tr(theItemName), this); \
//    m_actions[theItemName] = pAction;\
//    pToolbar->addAction(pAction);\
//    pAction->setCheckable(true);\
//    this->connect(pAction, SIGNAL(triggered()), this, SLOT(theHandlerName()));
//
//#define ADD_TOOLBAR_SEPARATOR \
//    pToolbar->addSeparator();
//
//#define END_MY_MSGMAP \
//}
//
//
//#define DECLARE_MY_MSGMAP \
//    void __Create_My_Qt_Msg_Map();\
//    map<string,QAction*> m_actions;\
//    QAction* GetAction(string strName);
//
//#define CREATE_MY_MSGMAP \
//    __Create_My_Qt_Msg_Map();
//
//
//#define BEGIN_MY_CONNECTION(theClass) \
//    void theClass::__Create_My_Qt_Connection() {
//
//#define ADD_CONNECTION(theTarget,theSignalName,theSlotName) \
//{\
//    connect(theTarget,SIGNAL(theSignalName),this,SLOT(theSlotName)); \
//}
//
//#define END_MY_CONNECTION \
//}
//
//#define DECLARE_MY_CONNECTION \
//    void __Create_My_Qt_Connection();
//
//#define CREATE_MY_CONNECTION \
//    __Create_My_Qt_Connection();
//
//
//#endif //MENU_MACRO_H