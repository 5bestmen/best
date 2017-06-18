#ifndef WIDGET_FACTORY_H
#define WIDGET_FACTORY_H   

class CBaseDyncData;

class CDynamicFactory
{
public:
	CDynamicFactory();
	~CDynamicFactory();
public:
	static CBaseDyncData * CreateDynamicAttr(unsigned int nAttrType);

};

#endif