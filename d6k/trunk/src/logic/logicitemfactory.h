#ifndef CLOGICITEMFACTORY_H
#define CLOGICITEMFACTORY_H

#include "logicabstractfactory.h"

class CLogicItemFactory : public CAbstractFactory
{
public:
    CLogicItemFactory();
    virtual ~CLogicItemFactory();
public:
    virtual CFdbOrderBaseItem* CreateLogicItemProduct(int ChangeType, int nIndex);


private:
    
};

#endif // CLOGICITEMFACTORY_H
