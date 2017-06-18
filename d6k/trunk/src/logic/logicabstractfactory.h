#ifndef LOGICABSTRACTFACTORY
#define LOGICABSTRACTFACTORY

class CFdbOrderBaseItem;

class CAbstractFactory{

public:
    CAbstractFactory(){};
    virtual ~CAbstractFactory(){};

public:
    virtual CFdbOrderBaseItem* CreateLogicItemProduct(int ChangeType, int nIndex) = 0;

};


#endif