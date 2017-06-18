#ifndef BASE_H
#define BASE_H

//tabwidget���ڹرձ���ӿ�

class IBase
{
public:
	IBase() 	
	{
		m_bModify = false;
	}

	~IBase()	
	{

	}

	virtual void Save() = 0;
	virtual void Refresh() = 0;
	virtual void Delete()
	{

	}


	//module file
	virtual void SetModifyFlag(bool bFlag) = 0;

	virtual bool GetModifyFlag() = 0;

protected:
	bool m_bModify;
};

#endif // CBASE_H
