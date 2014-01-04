//////////////////////////////////////////////////////////////////////////
// ���ܣ�XML�ڵ㼯��
//////////////////////////////////////////////////////////////////////////

#ifndef _XML_NODE_H_
#define _XML_NODE_H_
#include <string>

class CXmlDoc;
class CXmlSubNode;

class XML_API CXmlNode
{
public:
	CXmlNode(void);
	~CXmlNode(void);

public:
	// ���캯��
	CXmlNode( CXmlNode& refNodes );
	CXmlNode(MSXML2::IXMLDOMNodeListPtr pNodeList);

	// ��ֵ����
	CXmlNode* operator = (CXmlNode* pNodes);
	CXmlNode& operator = (CXmlNode& refNodes);

	// �±�����
	CXmlSubNode* operator[] ( unsigned int unIndex );
	CXmlSubNode* operator[] ( const std::string& strName );

	// ��ȡ�ӽڵ�����
	unsigned int GetCount(void);

	// ��ȡ�ӽڵ�
	CXmlSubNode* GetItem( unsigned int nIndex );
	CXmlSubNode* GetItem( const std::string& strName );

protected:
	// �ڵ㼯��
 	MSXML2::IXMLDOMNodeListPtr m_pNodeList;
};

#endif