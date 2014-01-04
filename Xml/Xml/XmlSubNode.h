//////////////////////////////////////////////////////////////////////////
// 功能：XML子节点
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
	// 构造函数
	CXmlSubNode( CXmlSubNode & refNode);
	CXmlSubNode( MSXML2::IXMLDOMNodePtr pNode);	

	// 赋值重载
	CXmlSubNode* operator = ( CXmlSubNode* pNode);
	CXmlSubNode& operator = ( CXmlSubNode& refNode);

	// 设置文档节点
	void			SetNode(MSXML2::IXMLDOMNodePtr pNode);

	// 获取节点名
	std::string GetName(void);
	// 当前是否为空
	bool			IsNull(void);

	// 获取属性
	std::string GetAttribute(const std::string& strName, const std::string& strDefault = "");
	// 设置属性
	bool			SetAttribute(const std::string& strName, const std::string& strValue);
	// 删除属性
	bool			DelAttribute(const std::string& strName);

	// 增加子节点
	CXmlSubNode* AddChild(const std::string& strName);
	// 获取子节点
	CXmlSubNode* GetChild(const std::string& strName);
	// 获取子节点集合
	CXmlNode*		 GetChildren();
	// 是否有子节点
	bool					 HasChildren(void);
	// 移除子节点
	bool					 RemoveChildren(void);
	// 移除当前节点
	bool				    Remove(void);

	// 获取节点字符串值
	std::string GetString(const std::string& strDefault = "");
	// 获取节点布尔值
	bool			GetBool(bool bDefault = false );
	// 获取节点整型值
	int			GetInt( int nDefault = 0 );
	// 获取节点浮点值
	double		GetFloat( double fDefault = 0.0f );

	// 设置节点字符串值
	bool SetString( const std::string& strValue );
	// 设置节点布尔值
	bool SetBool( bool bValue );
	// 设置节点整型值
	bool SetInt( int nValue );
	// 设置节点浮点值
	bool SetFloat( float fValue );

protected:
	// 获取节点值
	bool GetValue(std::string& strValue);
	// 设置节点值
	bool SetValue(const std::string& strValue);

protected:
	// XML节点
	MSXML2::IXMLDOMNodePtr   m_pNode;
};

#endif
