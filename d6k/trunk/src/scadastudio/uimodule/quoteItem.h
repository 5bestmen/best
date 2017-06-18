#ifndef CQUOTEITEM_H
#define CQUOTEITEM_H
#include <QStandardItem>

class CQuoteItem : public QStandardItem 
{
public:
	CQuoteItem()
	{

	}  

	CQuoteItem(const QIcon &icon, const QString &strText):QStandardItem(icon, strText)
	{
		
	}

	~CQuoteItem();

	CQuoteItem(const QString &strText)  
		:QStandardItem(strText)  
	{

	}

	CQuoteItem(const CQuoteItem &other)  
		: QStandardItem(other)  
	{  
	}  

	CQuoteItem &operator=(const CQuoteItem &other)  
	{  
		QStandardItem::operator=(other);  
		
		return *this;  
	}  

	virtual bool operator<(const QStandardItem &other) const  
	{  
		const QVariant l = data(Qt::DisplayRole), r = other.data(Qt::DisplayRole);  
		
		if(l.toString().compare("channel0") == 0)
		{
			int a = 5;

			a = 6;

			a = 666;
		}

		int nTypeL = data(Qt::UserRole).toInt();
		int nTypeR = other.data(Qt::UserRole).toInt();
		
		if(nTypeR == 111)
		{

		}

		bool ok, ok1;
		double fltL = l.toString().toDouble(&ok);

		double fltR = r.toString().toDouble(&ok1);
		//fltL = 1234567.563;
		if(ok & ok1)
		{
			return fltL < fltR;
		}
		else
		{	
			if(nTypeL != nTypeR)
			{
				return nTypeL < nTypeR;
			}
			else
			{
				return l.toString().localeAwareCompare(r.toString()) < 0;
			}
			
			
		}

		return QStandardItem::operator<(other);  
	}

private:
	
};

#endif // CQUOTEITEM_H
