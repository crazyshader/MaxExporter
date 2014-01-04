//////////////////////////////////////////////////////////////////////////
// ���ܣ�XML�ĵ�
//////////////////////////////////////////////////////////////////////////

#ifndef _XML_DOC_H_
#define _XML_DOC_H_
#include <string>

class CXmlSubNode;
class CXmlNode;

class XML_API CXmlDoc
{
public:
	// ���캯��
	CXmlDoc(void);
	~CXmlDoc(void);

public:
	// ����XML�ļ�
	bool Create(const std::string& strXmlFilePath);
	// ��XML�ļ�
	bool Open(const std::string& strXmlFilePath);
	// ����XML�ļ�
	bool Save(const std::string& strXmlFilePath = "");
	// �ر�XML�ļ�
	bool Close(void);

	// ��ȡ���һ������
	std::string		GetLastError(void);
	// XML�ļ��Ƿ����
	static bool		IsFileExist(const std::string& strFilePath);

	// �������ڵ�
	CXmlSubNode*	AddRoot(const std::string& strName);
	// ��ȡ���ڵ�
	CXmlSubNode*	GetRoot(const std::string& strName);

	// ��ȡ�ӽڵ�
	CXmlSubNode*	GetSubNode(const std::string& strPath);
	// ��ȡ�ڵ㼯��
	CXmlNode*			GetNode(const std::string& strPath);

protected:
	// XML��ʽ��
	MSXML2::IXMLDOMDocument2Ptr Normalize( MSXML2::IXMLDOMDocument2Ptr pXMLDocOld);

protected:
	// XML�ĵ�
	MSXML2::IXMLDOMDocument2Ptr m_pDoc;
	// XML�ļ�·��
	std::string					m_strFilePath;
};

#endif