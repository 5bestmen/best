/*! @file
<PRE>
********************************************************************************
ģ����       : ������ģ���ں��ṩ�ӿ� ������ӿڡ������ģ�����
�ļ���       :
�ļ�ʵ�ֹ��� :
����         :
�汾         :
--------------------------------------------------------------------------------
��ע         :
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
*  
********************************************************************************
</PRE>
*  @brief �������ṩ�ӿڡ�����ӿڡ�.
*  @author gw
*  @version 1.0
*  @date    2016.08.16
*/

#ifndef ICORE_H
#define ICORE_H

#include <QString>
#include <vector>

class IMainWindow;
class IModuleMgr;
class IProjMgr;
class IModule;
class CExtendServiceModuleMgr;

class IMainModuleInterface
{
public:
	//���main uiָ��
	virtual IMainWindow *GetUIMgr() = 0;

	//���ģ�����ָ��

	//virtual CModuleMgr *GetModuleMgr() = 0;

	//��ù��̹���ָ��
	virtual IProjMgr *GetProjMgr() = 0;

	//�����չ����ģ�����ָ��
	virtual CExtendServiceModuleMgr *GetExtendServiceModuleMgr() = 0;

	//virtual bool LoadModule(int nModuleID) = 0;
	// ��������ʵ�֣�������Ե��øýӿ�
	virtual bool LogMsg(const char *pszMoudleName,const char *szLogTxt, int nLevel) = 0;
	// ��������ʵ�֣�������Ե��øýӿ�
	virtual bool LogCheckMsg(const char *pszMoudleName, const char *szDescript, int nLevel,int nErrNo) = 0;

	virtual bool GetModulesName(std::vector<QString> & arrNames) = 0;
	// ��ȡ��ĳ��ģ���ָ��
	virtual IModule *GetModule(const char *szTxt) = 0;

	/*! \fn virtual bool SelectTag(const std::vector<int>& arrAllowDataTypes,bool bFilterEnWrite, std::string & szNodeTagName,
	std::string & szTagName, std::string &szFiledName);
	*********************************************************************************************************
	** \brief CCore::SelectTag
	** \details
	** \param arrAllowDataTypes ���������������������ͣ����磺����ѡ��Float,Double
	** \param bFilterEnWrite   ��������� ����ò���Ϊtrue��˵��ѡ���д������
	** \param szNodeTagName ����������ڵ����ƣ������ѡ���������Ҫ����ڵ�TAGNAME�������ѡ�������ò������践��
							Ӧ�ó���ͼ�Ρ��߼�������ȵȣ��Բ�㡢����Ķ�д���ʲ�������ģʽ��
							�ڵ���.������.����

							�磺  F1.AIN004.VALUE  ǰ�ýڵ�F1��ģ����AIN004�Ĳ�ֵ
							F2.DIN008.QUA    ǰ�ýڵ�F2�Ŀ�����DIN008��Ʒ��
							F4.$CPU          ǰ�ýڵ�F4��CPU���أ�ϵͳ����$CPU

	** \param szTagName     �����������������
	** \param szFiledName   ����������������ƣ������ֵ��Value��Ʒ����Quality...�����Ϊ�գ���Ĭ����Ϊ��Value
	** \return bool         ��������������false���򷵻ز���������
	** \author LiJin
	** \date 2016��10��13��
	** \note
	********************************************************************************************************/
	virtual bool SelectTag(const std::vector<int>& arrAllowDataTypes, bool bFilterEnWrite, std::string & szNodeTagName, std::string & szTagName, std::string &szFiledName) = 0;


	/*virtual bool SelectPoint(std::string & szTagName) = 0;*/
};

/** @}*/
#endif // ICORE_H
