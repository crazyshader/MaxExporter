//////////////////////////////////////////////////////////////////////////
// ���ܣ�XML�ӽڵ�
//////////////////////////////////////////////////////////////////////////

#ifndef _XML_SUB_NODE_H_
#define _XML_SUB_NODE_H_
#include <string>

class CXmlDoc;
class CXmlNode;

class XML_API CXmlSubNode
{
public:
	CXmlSubNode(void);
	~CXmlSubNode(void);

public:
	// ���캯��
	CXmlSubNode( CXmlSubNode & refNode);
	CXmlSubNode( MSXML2::IXMLDOMNodePtr pNode);	

	// ��ֵ����
	CXmlSubNode* operator = ( CXmlSubNode* pNode);
	CXmlSubNode& operator = ( CXmlSubNode& refNode);

	// �����ĵ��ڵ�
	void			SetNode(MSXML2::IXMLDOMNodePtr pNode);

	// ��ȡ�ڵ���
	std::string GetName(void);
	// ��ǰ�Ƿ�Ϊ��
	bool			IsNull(void);

	// ��ȡ����
	std::string GetAttribute(const std::string& strName, const std::string& strDefault = "");
	// ��������
	bool			SetAttribute(const std::string& strName, const std::string& strValue);
	// ɾ������
	bool			DelAttribute(const std::string& strName);

	// �����ӽڵ�
	CXmlSubNode* AddChild(const std::string& strName);
	// ��ȡ�ӽڵ�
	CXmlSubNode* GetChild(const std::string& strName);
	// ��ȡ�ӽڵ㼯��
	CXmlNode*		 GetChildren();
	// �Ƿ����ӽڵ�
	bool					 HasChildren(void);
	// �Ƴ��ӽڵ�
	bool					 RemoveChildren(void);
	// �Ƴ���ǰ�ڵ�
	bool				    Remove(void);

	// ��ȡ�ڵ��ַ���ֵ
	std::string GetString(const std::string& strDefault = "");
	// ��ȡ�ڵ㲼��ֵ
	bool			GetBool(bool bDefault = false );
	// ��ȡ�ڵ�����ֵ
	int			GetInt( int nDefault = 0 );
	// ��ȡ�ڵ㸡��ֵ
	double		GetFloat( double fDefault = 0.0f );

	// ���ýڵ��ַ���ֵ
	bool SetString( const std::string& strValue );
	// ���ýڵ㲼��ֵ
	bool SetBool( bool bValue );
	// ���ýڵ�����ֵ
	bool SetInt( int nValue );
	// ���ýڵ㸡��ֵ
	bool SetFloat( float fValue );

protected:
	// ��ȡ�ڵ�ֵ
	bool GetValue(std::string& strValue);
	// ���ýڵ�ֵ
	bool SetValue(const std::string& strValue);

protected:
	// XML�ڵ�
	MSXML2::IXMLDOMNodePtr   m_pNode;
};

#endif
