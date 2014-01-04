//////////////////////////////////////////////////////////////////////////
// 功能：XML文档
//////////////////////////////////////////////////////////////////////////

#ifndef _XML_DOC_H_
#define _XML_DOC_H_
#include <string>

class CXmlSubNode;
class CXmlNode;

class XML_API CXmlDoc
{
public:
	// 构造函数
	CXmlDoc(void);
	~CXmlDoc(void);

public:
	// 创建XML文件
	bool Create(const std::string& strXmlFilePath);
	// 打开XML文件
	bool Open(const std::string& strXmlFilePath);
	// 保存XML文件
	bool Save(const std::string& strXmlFilePath = "");
	// 关闭XML文件
	bool Close(void);

	// 获取最后一个错误
	std::string		GetLastError(void);
	// XML文件是否存在
	static bool		IsFileExist(const std::string& strFilePath);

	// 创建根节点
	CXmlSubNode*	AddRoot(const std::string& strName);
	// 获取根节点
	CXmlSubNode*	GetRoot(const std::string& strName);

	// 获取子节点
	CXmlSubNode*	GetSubNode(const std::string& strPath);
	// 获取节点集合
	CXmlNode*			GetNode(const std::string& strPath);

protected:
	// XML格式化
	MSXML2::IXMLDOMDocument2Ptr Normalize( MSXML2::IXMLDOMDocument2Ptr pXMLDocOld);

protected:
	// XML文档
	MSXML2::IXMLDOMDocument2Ptr m_pDoc;
	// XML文件路径
	std::string					m_strFilePath;
};

#endif