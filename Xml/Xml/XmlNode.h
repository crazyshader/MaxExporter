//////////////////////////////////////////////////////////////////////////
// 功能：XML节点集合
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
	// 构造函数
	CXmlNode( CXmlNode& refNodes );
	CXmlNode(MSXML2::IXMLDOMNodeListPtr pNodeList);

	// 赋值重载
	CXmlNode* operator = (CXmlNode* pNodes);
	CXmlNode& operator = (CXmlNode& refNodes);

	// 下标重载
	CXmlSubNode* operator[] ( unsigned int unIndex );
	CXmlSubNode* operator[] ( const std::string& strName );

	// 获取子节点数量
	unsigned int GetCount(void);

	// 获取子节点
	CXmlSubNode* GetItem( unsigned int nIndex );
	CXmlSubNode* GetItem( const std::string& strName );

protected:
	// 节点集合
 	MSXML2::IXMLDOMNodeListPtr m_pNodeList;
};

#endif