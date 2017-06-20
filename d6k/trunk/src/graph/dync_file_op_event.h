/*! @file dync_file_op_event.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dync_file_op_event.h
�ļ�ʵ�ֹ��� :  �ļ���������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����������¼�����
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/
#ifndef DYNC_FILE_OP_EVENT_H
#define DYNC_FILE_OP_EVENT_H


#include "dync_event.h"
#include "base_dync_event_item.h"

#include <string>
#include <memory>
#include <vector>

#include <QString>


class QXmlStreamWriter;
class QXmlStreamReader;

/*! \class CDyncFileOpEventItem DynamicDef.h  "DynamicDef.h"
*  \brief �ļ���������
*  \details  �ļ��Ĳ���ģʽ�У�MDI��ʽ�´��ļ���ģʽ�����ļ����ر��ļ�������һ������һ��
*/
class  CDyncFileOpEventItem : public CBaseDyncEventItem
{
public:
	CDyncFileOpEventItem();
	CDyncFileOpEventItem(const CDyncFileOpEventItem *);
	virtual ~CDyncFileOpEventItem();

	CDyncFileOpEventItem& operator=(const CDyncFileOpEventItem& src);
	bool operator==(const CDyncFileOpEventItem& src) const;
public:
	enum FILE_OP_TYPE
	{
		FILE_OPEN_NORMAL = 0,          //������
		FILE_MODAL_OPEN_FILE,           //ģʽ��
		OPEN_FRAME,           //�߿��
		OPEN_IN_OTHER_PROCESS,   //��ȫģʽ��
		FILE_PRINT,                //��ӡ
		FILE_CLOSE_FILE,    //�ر�
		EXECUTE_SYNAPSES,          //ִ�й���ͼ
		FILE_OPEN_NEXT_FILE,                  //����һ��
		FILE_OPEN_PREV_FILE,                 //����һ��
		CAPTURE_AND_PRINT,         //��׽����ӡ
		CAPTURE_AND_SAVE,          //��׽������
		MAX_FILE_OP_NUM,
	};
public:
	virtual std::string  GetEventDescript()const;
// 	virtual void SaveXml(JWXml::CXmlNodePtr  pDyncPtr) const;
// 	virtual bool LoadXml(JWXml::CXmlNodePtr  pDyncPtr);
// 
	//! �����ļ����������ı�
	static std::array<QString, MAX_FILE_OP_NUM> m_arrFileOpTypeTxt;

	const std::string  & GetGraphFileName() const;
	// �ļ���������
	void SetFileOpType(const QString  & szTxt);
	QString GetFileOpTypeTxt() const;
	// ���û���λ��
	void SetGraphPos(const std::string  & szX, const std::string  & szY);
	void SetGraphXPos(const std::string  & szX);
	void SetGraphYPos(const std::string  & szY);

	void SetFileName(const std::string  & szName);

	unsigned int GetGraphXPos() const
	{
		return m_nXPos;
	}
	unsigned int GetGraphYPos() const
	{
		return m_nYPos;
	}
	// �����¼�
	virtual void DoAction(CEventIntent &intent);

	// 
	void GetFileOpEvents(std::vector <CDyncFileOpEventItem*> &data) const;

	std::vector<CBaseDyncEventItem*> & GetFileOpEvents();

	// �����ļ������¼����˲�����ɾ��ԭ�е������ļ������
	void SetFileOpEvents(const std::vector <CDyncFileOpEventItem*> &data);

	//
	void GetFileOpEvents(std::vector<CBaseDyncEventItem*> &arrEvents);
	// ����һ���µĲ���
	CDyncFileOpEventItem* CreateFileOpEvent();
	bool AddFileOpEvent(CDyncFileOpEventItem * pFileOp);
	// ����
	bool SaveFileOp(CDyncFileOpEventItem * pFileOp);
private:
	//! �ļ���������
	FILE_OP_TYPE  m_FileOpType;
	//! �ļ�������ʽ�� Ŀ¼\�ļ� ��������׺
	std::string  m_szGraphFileName;
	//������
	int m_nMontor;
	//�����ļ�
	QString m_szParaFile;
	//ˮƽλ��
	QString m_strHorPos;
	//��ֱλ��
	QString m_strVorPos;
	//���
	int m_nWidth;
	//�߶�
	int m_nHeight;
	//

	unsigned int m_nXPos;
	unsigned int m_nYPos;
};
#endif // BASE_DYNC_EVENT_H


/** @}*/
