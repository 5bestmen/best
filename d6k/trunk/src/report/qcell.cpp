
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QtCore/qmath.h>
#include "qcell.h"
#include "public_define.h"
#include "aqp/aqp.hpp"


static void getWords(const QString& str, int &startRow, int& startColumn);
//hisdbi  m_dbi;
extern int G_TABLE_TYPE;
//extern DReportApp *dc_app;

const QVariant Invalid;
static QList<QVariant> mVdataList;

QCell::QCell(const CCellData &myItemData)
{
	SetDirty();
	SetFormatData(myItemData);
}

QTableWidgetItem * QCell::clone()const
{
	return  new QCell(*this);
}
void QCell::SetData(int role, const QVariant &value)
{
	if (role == Qt::EditRole)
	{
		SetDirty();
	}
	QTableWidgetItem::setData(role, value);
}

QVariant QCell::data(int role)const
{
	//return QTableWidgetItem::data(Qt::DisplayRole);
	if (role == Qt::DisplayRole)
	{
		if (value().isValid())
		{
			if (value().toInt() == -1)
			{
				return "----";
			}
			return value().toString();
		}
		else {
			return "----";
		}
	}
	else
	{
		return QTableWidgetItem::data(role);
	}
}
void QCell::SetFormula(const QString &formula)
{
	SetData(Qt::EditRole, formula);
}
QString QCell::Formula()const
{
	return data(Qt::EditRole).toString();
}

void QCell::SetDirty()
{
	m_bCachIsDirty = true;
}

QVariant QCell::value()const
{
	if (m_bCachIsDirty)
	{
		m_bCachIsDirty = false;
		QString formulaStr = Formula();
		if (formulaStr.startsWith('@'))
		{
			m_vCachedValued = Invalid;
			QString expr = formulaStr.mid(1);
			expr.replace(" ", "");
			expr.append(QChar::Null);
			int pos = 0;
			if (expr.indexOf("dateTime") == 0)
			{
				m_vCachedValued = EvalDateTime(expr);
			}
			else
				m_vCachedValued = EvalSql(expr, pos);
		}
		else if (formulaStr.startsWith('\''))
		{
			m_vCachedValued = formulaStr.mid(1);
		}
		else if (formulaStr.startsWith('='))
		{
			m_vCachedValued = Invalid;
			QString expr = formulaStr.mid(1);

			if (expr.indexOf("MAX") == 0)
			{
				m_vCachedValued = Invalid;
				QString nexpr = expr.mid(3);
				nexpr.replace(" ", "");
				nexpr.append(QChar::Null);
				int pos = 0;
				m_vCachedValued = EvalMax(nexpr, pos);
			}
			else if (expr.indexOf("qMIN") == 0)
			{
				m_vCachedValued = Invalid;
				QString nexpr = expr.mid(3);
				nexpr.append(QChar::Null);
				int pos = 0;
				m_vCachedValued = EvalMin(nexpr, pos);
			}
			else if (expr.indexOf("SUM") == 0)
			{
				m_vCachedValued = Invalid;
				QString nexpr = expr.mid(3);
				nexpr.append(QChar::Null);
				int pos = 0;
				m_vCachedValued = EvalSum(nexpr, pos);
			}
			else if (expr.indexOf("AVG") == 0)
			{
				m_vCachedValued = Invalid;
				QString nexpr = expr.mid(3);
				expr.append(QChar::Null);
				int pos = 0;
				m_vCachedValued = EvalAverage(nexpr, pos);
			}
			else if (expr.indexOf("PLUS") == 0)
			{
				m_vCachedValued = Invalid;
				QString nexpr = expr.mid(4);
				expr.append(QChar::Null);
				int pos = 0;
				m_vCachedValued = EvalPlus(nexpr, pos);
			}
			else if (expr.indexOf("MINUS") == 0)
			{
				m_vCachedValued = Invalid;
				QString nexpr = expr.mid(5);
				expr.append(QChar::Null);
				int pos = 0;
				m_vCachedValued = EvalMinus(nexpr, pos);
			}
			else if (expr.indexOf("DIVIDE") == 0)
			{
				m_vCachedValued = Invalid;
				QString nexpr = expr.mid(6);
				expr.append(QChar::Null);
				int pos = 0;
				m_vCachedValued = EvalDivide(nexpr, pos);
			}
			else if (expr.indexOf("MULTIPY") == 0)
			{
				m_vCachedValued = Invalid;
				QString nexpr = expr.mid(7);
				expr.append(QChar::Null);
				int pos = 0;
				m_vCachedValued = EvalMultipy(nexpr, pos);
			}
			else
			{
				expr.replace(" ", "");
				expr.append(QChar::Null);
				int pos = 0;
				m_vCachedValued = EvalExdission(expr, pos);
				if (expr[pos] != QChar::Null)
					m_vCachedValued = Invalid;
			}

		}
		else
		{
			bool bOk;
			double d = formulaStr.toDouble(&bOk);
			if (bOk)
			{
				m_vCachedValued = d;
			}
			else
			{
				m_vCachedValued = formulaStr;
			}
		}
	}
	return m_vCachedValued;

}


QVariant QCell::EvalDateTime(const  QString& date) const
{
	QString str("");
	//if (date==QString("dateTime"))
	//{
	str = QString("制表日期：") + m_Date.toString("yyyy-MM-dd");

	QString ss = m_Date.toString("yyyy-MM-dd");

	int year = m_Date.year();

	int mont = m_Date.month();
	//}
	return QVariant(str);
}
QVariant QCell::EvalExdission(const QString &str, int &pos)const
{
	QVariant result = EvalTerm(str, pos);
	while (str[pos] != QChar::Null) 
	{
		QChar op = str[pos];
		if (op != '+' && op != '-')
			return result;
		++pos;
		QVariant term = EvalTerm(str, pos);
		if (result.type() == QVariant::Double
			&& term.type() == QVariant::Double)
		{
			if (op == '+')
			{
				result = result.toDouble() + term.toDouble();
			}
			else 
			{
				result = result.toDouble() - term.toDouble();
			}
		}
		else
		{
			result = Invalid;
		}
	}
	return result;

}
QVariant QCell::EvalTerm(const QString &str, int &pos)const
{
	QVariant result = EvalFactor(str, pos);
	while (str[pos] != QChar::Null) 
	{
		QChar op = str[pos];
		if (op != '*' && op != '/')
			return result;
		++pos;
		QVariant factor = EvalFactor(str, pos);
		if (result.type() == QVariant::Double
			&& factor.type() == QVariant::Double) 
		{
			if (op == '*') 
			{
				result = result.toDouble() * factor.toDouble();
			}
			else 
			{
				if (factor.toDouble() == 0.0)
				{
					result = Invalid;
				}
				else 
				{
					result = result.toDouble() / factor.toDouble();
				}
			}
		}
		else 
		{
			result = Invalid;
		}
	}
	return result;

}
QVariant QCell::EvalFactor(const QString &str, int &pos)const
{
	QVariant result;
	bool negative = false;
	if (str[pos] == '-')
	{
		negative = true;
		++pos;
	}
	if (str[pos] == '(')
	{
		++pos;
		result = EvalExdission(str, pos);
		if (str[pos] != ')')
			result = Invalid;
		++pos;
	}
	else 
	{
		QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
		QString token;
		while (str[pos].isLetterOrNumber() || str[pos] == '.')
		{
			token += str[pos];
			++pos;
		}
		if (regExp.exactMatch(token))
		{
			int column = 0;//token[0].toUpper().unicode() - 'A';
			int row = 0;//token.mid(1).toInt() - 1;
			getWords(token, column, row);
			QCell *c = static_cast<QCell *>(
				tableWidget()->item(row, column));
			if (c) 
			{
				result = c->value();
			}
			else
			{
				result = 0.0;
			}
		}
		else 
		{
			bool ok;
			result = token.toDouble(&ok);
			if (!ok)
				result = Invalid;
		}
	}
	if (negative)
	{
		if (result.type() == QVariant::Double)
		{
			result = -result.toDouble();
		}
		else
		{
			result = Invalid;
		}
	}
	return result;

}

//解析历史数据，待历史表确定方能读数！！
QVariant QCell::EvalSql(const QString &str, int &pos)const
{
	//m_dbi.exportConnectDB();

	QVariant result;
	//获取数据点类型,flag
	QString dataType = str.section(',', 0, 0);
	int dataInfo = ParaseDefines::GetDefineInt(dataType);

	QString deviceInfo = str.section(',', 1, 1);
	//解析出厂站表，设备表和AI/DI点表，确认报表唯一点
	int stationID = deviceInfo.section('#', 0, 0).toInt();

	int deviceID = deviceInfo.section('#', 1, 1).toInt();

	int potID = deviceInfo.section('#', 2, 2).toInt();
	//当前绝对时间
	int year = m_Date.year();
	int month = m_Date.month();
	int m_ndays = m_Date.daysInMonth();

	int d_days = qAbs(m_Date.daysTo(QDate(1970, 1, 1)));

	QString timeInfo = str.section(',', 2, 2);

	QString type = timeInfo.section('#', 0, 0);

	int t_time = QString(timeInfo.section('#', 1, 1)).toLatin1().toInt();
	//! [1]
	int tableType = G_TABLE_TYPE;

	QVariant evalty;

	switch (tableType)
	{
	case 0://日报表
	{
		if (type.toInt() == 1)//AI
		{
			evalty = DealDReport(dataInfo, stationID, deviceID, potID, d_days, t_time);
		}
		if (type.toInt() == 3)//CAL
		{
			evalty = DealDCalReport(dataInfo, stationID, deviceID, potID, d_days, t_time);
		}
		if (type.toInt() == 4)//KWH
		{
			evalty = DealDKwhReport(dataInfo, stationID, deviceID, potID, d_days, t_time);
		}
		break;
	}
	case 1://月报表
	{
		// 添加日期非法判断
		if (t_time > m_ndays)
		{
			return -1;
		}
		if (type.toInt() == 1)//AI
		{
			int m_days = QDate(1970, 1, 1).daysTo(QDate(m_Date.year(), m_Date.month(), 1));
			evalty = DealMReport(dataInfo, stationID, deviceID, potID, m_days + t_time - 1, 0);
		}
		if (type.toInt() == 3)//CAL
		{
			int m_days = QDate(1970, 1, 1).daysTo(QDate(m_Date.year(), m_Date.month(), 1));
			evalty = DealMCalReport(dataInfo, stationID, deviceID, potID, m_days + t_time - 1, 0);
		}
		if (type.toInt() == 4)//KWH
		{
			int m_days = QDate(1970, 1, 1).daysTo(QDate(m_Date.year(), m_Date.month(), 1));
			evalty = DealDDReport(dataInfo, stationID, deviceID, potID, m_days + t_time - 1, 0);
		}

		break;
	}
	case 2://年报表
	{
		// 添加日期非法判断
		if (t_time > 12 || t_time < 0)
		{
			return -1;
		}
		int nDays = year * 100 + t_time;
		int nType = type.toInt();
		evalty = DealYSReport(dataInfo, stationID, deviceID, potID, nType, nDays);
	}
	break;
	case 3://特殊报表
		break;
	default://NoneReport
		return QVariant(VAL_IS_NOT_VALID);
		break;

	}
	return QVariant(evalty);

}
QVariant QCell::DealDCalReport(int dataInfo, int station_id, int device_id, int ai_id, int date, int time)const
{
	switch (dataInfo)
	{
	case HISDATA_REAL://计算量数值存储只存在日报表中
	default:
	{
		QString datat;// = (m_dbi.getCalDValue(station_id, device_id, ai_id, date, time / 60)).toString();

		if (datat.isEmpty())
		{
			return QVariant(VAL_IS_NOT_VALID);
		}
		qDebug() << datat;

		QString eal = EvalData(datat, time);
		return QVariant(eal);
	}
	break;
	}

}
QVariant QCell::DealMCalReport(int dataInfo, int station_id, int device_id, int ai_id, int date, int time)const
{
	QVariant datat;// = (m_dbi.getMCalValue(station_id, device_id, ai_id, 54, date - 1, 0, date + 1, 0)).toString();

	QString t_temp = datat.toString();

	if (t_temp.isNull() || t_temp.isEmpty())
	{
		return QVariant(VAL_IS_NOT_VALID);
	}
	double tty = t_temp.toDouble();
	QString strr = QString("%1").arg(tty, 0, 'd', 2);
	return QVariant(strr);
}
QVariant QCell::DealDKwhReport(int dataInfo, int station_id, int device_id, int ai_id, int date, int time)const
{
	switch (dataInfo)
	{
	case HISDATA_REAL://KWH数值日发电量
	default:
	{
		QString datat;// = (m_dbi.getKwhDValue(station_id, device_id, ai_id, date, time / 60)).toString();

		if (datat.isEmpty())
		{
			return QVariant(VAL_IS_NOT_VALID);
		}
		qDebug() << datat;

		QString eal = EvalData(datat, time);
		return QVariant(eal);
	}
	break;
	}
}
QVariant QCell::DealDDReport(int dataInfo, int station_id, int device_id, int ai_id, int date, int time)const
{
	QVariant datat;// = (m_dbi.getDDDSValue(station_id, device_id, ai_id, KWH_DATA, date - 1, 0, date + 1, 0)).toString();

	QString t_temp = datat.toString();

	if (t_temp.isNull() || t_temp.isEmpty())
	{
		return QVariant(VAL_IS_NOT_VALID);
	}
	double tty = t_temp.toDouble();
	QString strr = QString("%1").arg(tty, 0, 'd', 2);

	return QVariant(strr);
}
QVariant QCell::DealCalReport(int dataInfo, int station_id, int device_id, int ai_id, int date, int time)const
{
	return QVariant(0);
}

//定位到某一时刻数值
QString QCell::EvalData(const QString& str, const int& time)const
{
	QString temp = str;

	int num = (time % 60) / 5;

	QString result = temp.section(';', num, num);

	double t_data = result.section(',', 0, 0).toLatin1().toDouble();

	QString strr = QString("%1").arg(t_data, 0, 'd', 4);

	return strr;
}

QVariant QCell::DealDReport(int dataInfo, int station_id, int device_id, int ai_id, int date, int time)const
{

	switch (dataInfo)
	{
	case HISDATA_REAL:
	{
		QString datat;// = (m_dbi.getAiDValue(station_id, device_id, ai_id, date, time / 60)).toString();

		if (datat.isEmpty())
		{
			return QVariant(VAL_IS_NOT_VALID);
		}
		qDebug() << datat;

		QString eal = EvalData(datat, time);

		/* if (eal.toDouble()<-100)
		 {
		 eal=QString::number(eal.toDouble()+6553.6);
		 }*/

		return QVariant(eal);
	}
	break;
	case HISDATA_AVERAGE://平均值
	case HISDATA_REALMAX://最大值
	case HISDATA_MAXTIME:
	case HISDATA_REALMIN://最小值
	case HISDATA_MINTIME://最小值时间
	case HISDATA_OVHCOUNT://越上限次数
	case HISDATA_OVHTIME:
	case HISDATA_OVHHCOUNT://越上上限次数
	case HISDATA_OVHHTIME://越上上限时间
	case HISDATA_DOWNLCOUNT://越下限次数
	case HISDATA_DOWNLTIME:
	case HISDATA_DOWNLLCOUNT:
	case HISDATA_DOWNLLTIME:
	case HISDATA_REALHGL:
		//统计值
	case HISDATA_DAYS_MAX:
	case HISDATA_DAYS_MIN:
	case HISDATA_DAYS_AVERAGE:
	{
		QVariant datat;// = (m_dbi.getAiDSValue(station_id, device_id, ai_id, dataInfo, date - 1, 0, date + 1, 1440)).toString();

		if (datat.toString().isEmpty() || datat.toInt() == -1)
		{
			return QVariant(VAL_IS_NOT_VALID);
		}
		qDebug() << datat;

		/* if (datat.toDouble()<-100)
		 {
		 datat.setValue(datat.toDouble()+6553.6);
		 }*/

		return datat;
	}
	break;
	case HISDATA_DAYS_MAXTIME://最大值时间
	{
		QVariant datat;// = (m_dbi.getAiMaxTimeSValue(station_id, device_id, ai_id, 150, date - 1, 0, date + 1, 1440)).toString();
		if (datat.toString().isEmpty() || datat.toInt() == -1)
		{
			return QVariant(VAL_IS_NOT_VALID);
		}
		qDebug() << datat;
		return datat;
		break;
	}
	case HISDATA_DAYS_MINTIME:
	{
		QVariant datat;// = (m_dbi.getAiMinTimeSValue(station_id, device_id, ai_id, 151, date - 1, 0, date + 1, 1440)).toString();
		if (datat.toString().isEmpty() || datat.toInt() == -1)
		{
			return QVariant(VAL_IS_NOT_VALID);
		}
		qDebug() << datat;
		return datat;
		break;
	}
	default:
		return QVariant(VAL_IS_NOT_VALID);
		break;
	}
}

QVariant QCell::DealMReport(int dataInfo, int station_id, int device_id, int ai_id, int date, int time)const/*最后为天数*/
{
	switch (dataInfo)
	{
	case HISDATA_REAL://此处月报表统计每天最大值
	{
		QVariant datat;// = (m_dbi.getAiDSValue(station_id, device_id, ai_id, HISDATA_DAYS_MAX, date - 1, 0, date + 1, 0)).toString();

		QString t_temp = datat.toString();

		if (t_temp.isNull() || t_temp.isEmpty())
		{
			return QVariant(VAL_IS_NOT_VALID);
		}
		double tty = t_temp.toDouble();
		/*  if (tty<-100)
		{
		tty+=6553.6;
		}*/
		QString strr = QString("%1").arg(tty, 0, 'd', 4);
		return QVariant(strr);
	}
	case HISDATA_AVERAGE://平均值
	case HISDATA_REALMAX://最大值
	case HISDATA_MAXTIME://最大值时间
	case HISDATA_REALMIN://最小值
	case HISDATA_MINTIME://最小值时间
	case HISDATA_OVHCOUNT://越上限次数
	case HISDATA_OVHTIME:
	case HISDATA_OVHHCOUNT://越上上限次数
	case HISDATA_OVHHTIME://越上上限时间
	case HISDATA_DOWNLCOUNT://越下限次数
	case HISDATA_DOWNLTIME:
	case HISDATA_DOWNLLCOUNT:
	case HISDATA_DOWNLLTIME:
	case HISDATA_REALHGL:
		//统计值
	case HISDATA_DAYS_MAX:
	case HISDATA_DAYS_MIN:
	case HISDATA_DAYS_AVERAGE:
	case HISDATA_DAYS_MAXTIME:
	case HISDATA_DAYS_MINTIME:
	{
		int m_days = QDate(1970, 1, 1).daysTo(QDate(m_Date.year(), m_Date.month(), 1));
		int t_daysInMonths = m_Date.daysInMonth();
		//   evalty=dealMReport(dataInfo,stationID,deviceID,potID,m_days+t_time-1,0);
		QVariant datat;// = (m_dbi.getAiDSValue(station_id, device_id, ai_id, dataInfo, m_days + t_daysInMonths - 2, 0, m_days + t_daysInMonths, 0)).toString();

		QString t_temp = datat.toString();

		if (t_temp.isNull() || t_temp.isEmpty() || datat.toInt() == -1)
		{
			return QVariant(VAL_IS_NOT_VALID);
		}

		double tty = t_temp.toDouble();

		/* if (tty<-100)
		{
		tty+=6553.6;
		}*/
		QString strr = QString("%1").arg(tty, 0, 'd', 2);
		return QVariant(strr);
		break;
	}
	default:
		return QVariant(VAL_IS_NOT_VALID);
		break;
	}
	return QVariant(0);
}
void QCell::SetMyTextAlign(int data)
{
	switch (data)
	{
	case 0x0081:
		this->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		break;
	case 0x0082:
		this->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
		break;
	case  0x0084:
		this->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		break;
	default:
		break;
	}
}

void QCell::SetFormatData(const CCellData& cellFormat)
{
	if (!cellFormat.m_CellBgColor.isValid())
	{
		this->setBackgroundColor(QColor(0, 0, 0));
	}
	this->setBackgroundColor(cellFormat.m_CellBgColor);
	QColor mycolor = cellFormat.m_CellBgColor;
	qDebug("R:%d,G:%d,B:%d", mycolor.red(), mycolor.green(), mycolor.blue());
	this->SetMyTextAlign(cellFormat.m_TextAlign);
	this->setTextColor(cellFormat.m_TextColor);
	this->setFont(cellFormat.m_TextFont);
	this->SetFormula(cellFormat.m_szDataContent);
}

CCellData QCell::GetFormatData()
{
	// ItemData;
	//表格format

	QColor mycolor = backgroundColor();
	if (mycolor.isValid())
	{
		m_ItemData.m_CellBgColor = backgroundColor();
	}
	else{
		m_ItemData.m_CellBgColor = QColor(255, 255, 255);
	}
	qDebug("R:%d,G:%d,B:%d", mycolor.red(), mycolor.green(), mycolor.blue());
	//内容format
	m_ItemData.m_TextAlign = textAlignment();
	m_ItemData.m_TextColor = textColor();
	m_ItemData.m_TextFont = font();
	//TODO 返回局局部变量
	m_ItemData.m_szDataContent = Formula();
	m_ItemData.m_nRowIndex = row();
	m_ItemData.m_nColIndex = column();

	return m_ItemData;
}

QVariant QCell::EvalMax(const QString& str, int& pos)const
{
	QVariantList mylist = Eval(str);
	if (mylist.isEmpty())
	{
		return QVariant(double(0));
	}

	QVariant maxNum(mylist.at(0));
	foreach(QVariant var, mylist)
	{
		maxNum = qMax(var.toDouble(), maxNum.toDouble());
	}
	return maxNum;
}
QVariant QCell::EvalMin(const QString& str, int& pos)const
{
	QVariantList mylist = Eval(str);
	if (mylist.isEmpty())
	{
		return QVariant(double(0));
	}

	QVariant minNum(mylist.at(0));
	foreach(QVariant var, mylist)
	{
		minNum = qMin(var.toDouble(), minNum.toDouble());
	}
	return minNum;
}
QVariant QCell::EvalAverage(const QString& str, int& pos)const
{
	QVariantList mylist = Eval(str);
	if (mylist.isEmpty())
	{
		return QVariant(double(0));
	}
	double avgNum = 0.0;

	foreach(QVariant var, mylist)
	{
		avgNum += var.toDouble();
	}

	avgNum = avgNum / (mylist.count());

	return avgNum;
}

QVariant QCell::EvalSum(const QString& str, int& pos)const
{
	QVariantList mylist = Eval(str);
	if (mylist.isEmpty())
	{
		return QVariant(double(0));
	}
	double avgSum = 0.0;

	/*int nRow=0;
	int nColumn=0;*/

	foreach(QVariant var, mylist)
	{
		//getWords(var.toString(),nColumn,nRow);

		/*QVariant result;

		QCell *c=static_cast<QCell *>(tableWidget()->item(nRow,nColumn));

		if (c)
		{
		result=c->value();
		}
		else
		{
		result=0.0;
		}*/
		avgSum += var.toDouble();
	}
	return avgSum;
}

void getWords(const QString& inputstr, int &startRow, int& startColumn)
{
	if (inputstr.isEmpty())
	{
		return;
	}
	int endW = 0, endN = 0;
	for (register int i = 0; i < inputstr.length(); i++)
	{
		if (inputstr[i] >= 'A'&&inputstr[i] <= 'Z')
		{

			continue;
		}
		else
		{
			endW = i;
			break;
		}

	}

	for (register int i = 0; i < inputstr.length(); i++)
	{
		if (inputstr[i] >= '0'&&inputstr[i] <= '9')
		{
			continue;
		}
		else
		{
			endN = i;
			break;
		}
	}
	//获取所有字母
	QString outPutWords = inputstr.mid(0, endW);
	startRow = 0;
	if (outPutWords.length() > 1)
	{
		startRow = 26 * (outPutWords[0].toUpper().unicode() - 'A' + 1) + outPutWords[1].toUpper().unicode() - 'A';
	}
	else
	{
		startRow = outPutWords[0].toUpper().unicode() - 'A';
	}
	/*for (int i=outPutWords.length()-1;i>=0;--i)
	{
	startRow+=(26*(i)+int(outPutWords[i].toUpper().unicode()-'A'));
	}*/
	startRow = startRow;
	//获取所有数字
	QString outPutNums = inputstr.mid(endW);

	startColumn = 0;

	startColumn = outPutNums.toInt() - 1;
}

QVariantList& QCell::Eval(const QString &str)const
{

	QString expr = str;
	mVdataList.clear();
	if (expr.startsWith('(') && expr[1] != ')')
	{
		expr.replace('(', "");
		expr.replace(')', "");

		int pos = 0;
		QString token;

		while (expr[pos].isLetterOrNumber() || expr[pos] == ',' || expr[pos] == ':')
		{
			token += expr[pos];
			++pos;
		}

		if (!token.contains(':') && !(token.count(',')))
		{
			int nRow, nColumn;

			getWords(token, nColumn, nRow);

			QVariant m_ret;

			QCell *c = static_cast<QCell *>(tableWidget()->item(nRow, nColumn));

			// QCell *c = static_cast<QCell *>(tableWidget()->item(i,j));

			if (c)
			{
				m_ret = c->value();
			}
			else
			{
				m_ret = 0.0;
			}

			mVdataList.append(m_ret);
		}

		if (token.contains(':') && token.count(',') == 0)
		{
			QVariant result;
			// 连续数据
			QString startStr = token.section(':', 0, 0);
			QString endStr = token.section(':', 1, 1);

			int startColumn = 0;//getWords[0].toUpper().unicode()-'A';

			int startRow = 0;//startStr.mid(1).toInt()-1;

			int endColumn = 0;//endStr[0].toUpper().unicode()-'A';
			int endRow = 0;//endStr.mid(1).toInt()-1;

			getWords(startStr, startColumn, startRow);
			getWords(endStr, endColumn, endRow);

			for (int i = startRow; i <= endRow; i++)
			{
				for (int j = startColumn; j <= endColumn; j++)
				{
					QCell *c = static_cast<QCell *>(tableWidget()->item(i, j));
					if (c)
					{
						result = c->value();
					}
					else
					{
						result = 0.0;
					}
					mVdataList.append(result);
				}
			}
		}
		else if (token.contains(',') && token.count(':') == 0)
		{
			//离散数据
			int nCount = token.count(',');
			QStringList myList = token.split(',');

			foreach(QString temp, myList)
			{
				int tempColumn = 0;//temp[0].toUpper().unicode()-'A';
				int tempRow = 0;//temp.mid(1).toInt()-1;
				getWords(temp, tempColumn, tempRow);

				QVariant result;

				QCell *c = static_cast<QCell *>(tableWidget()->item(tempRow, tempColumn));
				if (c)
				{
					result = c->value();
				}
				else
				{
					result = 0.0;
				}
				mVdataList.append(result);
			}
		}
		else if (token.contains(':') && token.contains(','))
		{
			//离散数据和连续数据集合
			int nQCount = token.count(':');

			int nDCount = token.count(',');

			QStringList allList = token.split(',');

			foreach(QString temp, allList)
			{
				if (temp.contains(':'))
				{
					QString startStr = token.section(':', 0, 0);
					QString endStr = token.section(':', 1, 1);

					int startColumn = 0;//0startStr[0].toUpper().unicode()-'A';
					int startRow = 0;//startStr.mid(1).toInt()-1;

					int endColumn = 0;//endStr[0].toUpper().unicode()-'A';
					int endRow = 0;//endStr.mid(1).toInt()-1;

					getWords(startStr, startColumn, startRow);
					getWords(endStr, endColumn, endRow);

					QVariant result;

					for (int i = startRow; i <= endRow; i++)
					{
						for (int j = startColumn; j <= endColumn; j++)
						{
							QCell *c = static_cast<QCell *>(tableWidget()->item(i, j));
							if (c)
							{
								result = c->value();
							}
							else
							{
								result = 0.0;
							}
							mVdataList.append(result);
						}
					}
				}
				else
				{
					int tempColumn = 0;//temp[0].toUpper().unicode()-'A';
					int tempRow = 0;//temp.mid(1).toInt()-1;
					getWords(temp, tempColumn, tempRow);

					QVariant result;

					QCell *c = static_cast<QCell *>(tableWidget()->item(tempRow, tempColumn));
					if (c)
					{
						result = c->value();
					}
					else
					{
						result = 0.0;
					}
					mVdataList.append(result);
				}
			}
		}
	}
	return mVdataList;
}

void QCell::SetSpanData(CMergeInfo &temp)
{
	m_ItemData.m_MergeRecoder = temp;
}

void QCell::SetSpanClear()
{
	m_ItemData.m_MergeRecoder.m_nRow = 0;
	m_ItemData.m_MergeRecoder.m_nColCount = 0;
	m_ItemData.m_MergeRecoder.m_nColumn = 0;
	m_ItemData.m_MergeRecoder.m_nRowCount = 0;

}

QVariant QCell::EvalPlus(const QString &str, int& pos)const
{
	QVariantList mylist = Eval(str);
	if (mylist.isEmpty())
	{
		return QVariant(double(0));
	}
	double avgSum = 0.0;

	foreach(QVariant var, mylist)
	{
		avgSum += var.toDouble();
	}
	return avgSum;
}

QVariant QCell::EvalMinus(const QString &str, int& pos)const
{
	QString startStr = str.section(',', 0, 0) + ')';

	QString endStr = '(' + str.section(',', 1, 1);

	int temp = 0;

	double leftData = EvalSum(startStr, temp).toDouble();

	double rightData = EvalSum(endStr, temp).toDouble();

	double result = leftData - rightData;

	return result;
}

QVariant QCell::EvalMultipy(const QString&str, int& pos)const
{
	QString startStr = str.section(',', 0, 0) + ')';

	QString endStr = '(' + str.section(',', 1, 1);

	int temp = 0;

	double leftData = EvalSum(startStr, temp).toDouble();

	double rightData = EvalSum(endStr, temp).toDouble();

	double result = leftData*rightData;

	return result;
}
#define  MYDEFINEDATA  0.000001

QVariant QCell::EvalDivide(const QString& str, int& pos)const
{
	QString startStr = str.section(',', 0, 0) + ')';

	QString endStr = '(' + str.section(',', 1, 1);

	int temp = 0;

	double leftData = EvalSum(startStr, temp).toDouble();

	double rightData = EvalSum(endStr, temp).toDouble();

	if (abs(rightData) <= MYDEFINEDATA)
	{
		//AQP::warning(this,"计算数据出现错误","被除数为0 ，需重新计算！！！");
		return double(-1);
	}

	double result = leftData / rightData;

	return result;
}

//************************************
// Method:    DealYSReport
// FullName:  QCell::DealYSReport
// Access:    private 
// Returns:   QT_NAMESPACE::QVariant
// Qualifier: const
// Parameter: int dataInfo    存储类型
// Parameter: int station_id  场站ID
// Parameter: int device_id   设备ID
// Parameter: int ai_id       测点ID
// Parameter: int type        数据类型
// Parameter: int date        读取日期
//************************************
QVariant QCell::DealYSReport(int dataInfo, int station_id, int device_id, int ai_id, int type, int date) const
{
	switch (dataInfo)
	{
	case HISDATA_DAYS_AVERAGE:     //统计平均值
	case HISDATA_DAYS_MAXTIME:     //统计最大值时间
	case HISDATA_DAYS_MAX:         //统计最大值
	case HISDATA_DAYS_MINTIME:     //统计最小值时间
	case HISDATA_DAYS_MIN:         //统计最小值
	case HISDATA_DAYKWHTTL:        //累计值
	{
		QVariant datat;// = (m_dbi.GetYSValue(station_id, device_id, ai_id, type, dataInfo, date)).toString();
		if (datat.toString().isEmpty() || datat.toInt() == -1)
		{
			return QVariant(VAL_IS_NOT_VALID);
		}
		qDebug() << datat;
		return datat;
		break;
	}
	default:
		break;
	}
	return QVariant();
}


