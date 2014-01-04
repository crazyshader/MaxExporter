#include "Xml.h"

CXmlSubNode::CXmlSubNode(void)
{
	m_pNode = NULL;	
}

CXmlSubNode::CXmlSubNode( MSXML2::IXMLDOMNodePtr pNode)
{
	m_pNode = pNode;
}

CXmlSubNode::CXmlSubNode( CXmlSubNode & refNode)
{
	m_pNode = refNode.m_pNode;
}

CXmlSubNode::~CXmlSubNode(void)
{
	m_pNode = NULL;
}

CXmlSubNode* CXmlSubNode::operator = ( CXmlSubNode* pNode)
{
	assert(pNode != NULL);
	m_pNode = pNode->m_pNode;

	return pNode;
}

CXmlSubNode& CXmlSubNode::operator = ( CXmlSubNode& refNode)
{
	m_pNode = refNode.m_pNode;

	return (*this);
}

bool CXmlSubNode::IsNull(void)
{
	return (m_pNode == NULL);
}

CXmlSubNode* CXmlSubNode::AddChild( const std::string& strName )
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return NULL;
	}

	MSXML2::IXMLDOMNodePtr pChildNode = NULL;
	MSXML2::IXMLDOMDocumentPtr pDoc = NULL;
	HRESULT hr = m_pNode->get_ownerDocument(&pDoc);
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return NULL;
	}

	pChildNode = pDoc->createElement( _bstr_t(strName.c_str()) );
	if (NULL == pChildNode)
	{
		assert( NULL != pChildNode);
		return NULL;
	}

	m_pNode->appendChild(pChildNode);
	SAFE_RELEASE(pDoc);

	CXmlSubNode* pChild( new CXmlSubNode(pChildNode) );
	SAFE_RELEASE(pChildNode);
	if (NULL == pChild)
	{
		assert(NULL != pChild);
		return NULL;
	}

	return pChild;
}

CXmlSubNode* CXmlSubNode::GetChild(const std::string& strName)
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return NULL;
	}

	MSXML2::IXMLDOMNodePtr pChildNode = NULL;
	pChildNode = m_pNode->selectSingleNode(_bstr_t(strName.c_str()));
	if( pChildNode == NULL )
	{
		assert( pChildNode != NULL );
		return NULL;
	}

	CXmlSubNode* pChild( new CXmlSubNode(pChildNode) );
	SAFE_RELEASE(pChildNode);
	if (NULL == pChild)
	{
		assert(NULL != pChild);
		return NULL;
	}

	return pChild;
}

bool CXmlSubNode::GetValue(std::string& strValue)
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return false;
	}

	if( HasChildren() )
	{
		assert( false );
		return false;
	}

	BSTR bstr = NULL;
	HRESULT hr = m_pNode->get_text( &bstr );
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return false;
	}

	strValue = _bstr_t( bstr, true);
	if( bstr != NULL )
	{
		SysFreeString(bstr);
		bstr = NULL;
	}

	return true;
}

bool CXmlSubNode:: SetValue(const std::string& strValue)
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return false;
	}

	if( HasChildren() )
	{
		assert( false );
		return false;
	}

	HRESULT hr = S_OK;
	hr = m_pNode->put_text( _bstr_t(strValue.c_str()) );
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return NULL;
	}

	return true;
}

std::string CXmlSubNode::GetName(void)
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return "";
	}

	BSTR bstr = NULL;
	HRESULT hr = m_pNode->get_nodeName(&bstr);
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return "";
	}

	std::string strRet = _bstr_t( bstr, true);
	if( bstr != NULL )
	{
		SysFreeString(bstr);
		bstr = NULL;
	}

	return strRet;
}

std::string CXmlSubNode::GetAttribute( const std::string& strName
							   , const std::string& strDefault/* = ""*/)
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return "";
	}

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNamedNodeMapPtr pIXMLDOMNamedNodeMap = NULL;
	hr = m_pNode->get_attributes(&pIXMLDOMNamedNodeMap);
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return "";
	}

	MSXML2::IXMLDOMNodePtr pIXMLDOMNode = NULL;
	pIXMLDOMNode = pIXMLDOMNamedNodeMap->getNamedItem( _bstr_t(strName.c_str()) );

	std::string strValue;
	if( pIXMLDOMNode == NULL )
	{
		strValue = strDefault;
		SetAttribute( strName, strValue);		
	}
	else
	{
		VARIANT varValue;
		hr = pIXMLDOMNode->get_nodeValue(&varValue);
		if (!SUCCEEDED(hr))
		{
			assert( SUCCEEDED(hr) );
			return "";
		}

		strValue = (_bstr_t)varValue;
	}

	pIXMLDOMNode = NULL;
	pIXMLDOMNamedNodeMap = NULL;

	return strValue;
}

bool CXmlSubNode::SetAttribute( const std::string& strName
							, const std::string& strValue)
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return false;
	}

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMElementPtr pElement = NULL;
	pElement = static_cast<MSXML2::IXMLDOMElementPtr> (m_pNode);
	hr = pElement->setAttribute( _bstr_t(strName.c_str()), _variant_t((LPCSTR)strValue.c_str()) );
	SAFE_RELEASE(pElement);
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return false;
	}

	return true;
}

bool CXmlSubNode::DelAttribute( const std::string& strName)
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return false;
	}

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNamedNodeMapPtr pIXMLDOMNamedNodeMap = NULL;
	hr = m_pNode->get_attributes(&pIXMLDOMNamedNodeMap);
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return false;
	}

	hr = pIXMLDOMNamedNodeMap->removeNamedItem( _bstr_t(strName.c_str()) );
	SAFE_RELEASE(pIXMLDOMNamedNodeMap);
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return false;
	}

	return true;
}

bool CXmlSubNode::HasChildren(void)
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return false;
	}

	bool bHasChildren = false;

	HRESULT hr = S_OK;
	IXMLDOMNodePtr pNode = NULL;
	DOMNodeType NodeType;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	hr = m_pNode->get_childNodes(&pNodeList);
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return false;
	}

	for( int i = 0; i < pNodeList->length; i++)
	{
		pNode = pNodeList->item[i];
		hr = pNode->get_nodeType(&NodeType);
		SAFE_RELEASE(pNode);
		if (!SUCCEEDED(hr))
		{
			assert( SUCCEEDED(hr) );
			return false;
		}

		if( NodeType == MSXML2::NODE_ELEMENT )
		{
			bHasChildren = true;
			break;
		}
	}

	SAFE_RELEASE(pNodeList);

	return bHasChildren;
}

bool CXmlSubNode::Remove(void)
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return false;
	}

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	hr = m_pNode->get_parentNode(&pNode);
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return false;
	}

	pNode->removeChild(m_pNode);

	return true;
}

bool CXmlSubNode::RemoveChildren(void)
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return false;
	}

	if( !HasChildren() )
	{
		assert(false);
		return false;
	}

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNodePtr pNode = NULL;
	MSXML2::DOMNodeType NodeType;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	hr = m_pNode->get_childNodes(&pNodeList);
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return false;
	}

	for( int i = 0; i < pNodeList->length; i++)
	{
		pNode = pNodeList->item[i];
		hr = pNode->get_nodeType(&NodeType);
		if (!SUCCEEDED(hr))
		{
			assert( SUCCEEDED(hr) );
			return false;
		}

		if( NodeType == MSXML2::NODE_ELEMENT )
		{
			m_pNode->removeChild(pNode);
		}

		SAFE_RELEASE(pNode);
	}

	SAFE_RELEASE(pNodeList);

	return true;
}

CXmlNode* CXmlSubNode::GetChildren()
{
	if (NULL == m_pNode)
	{
		assert(NULL != m_pNode);
		return NULL;
	}

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	hr = m_pNode->get_childNodes(&pNodeList);
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return NULL;
	}

	CXmlNode* pNodes(new CXmlNode(pNodeList));
	if (NULL == pNodes)
	{
		assert(NULL != pNodes);
		return NULL;
	}

	return pNodes;
}

std::string CXmlSubNode::GetString(const std::string& strValue/* = ""*/)
{
	std::string strTemp;
	GetValue(strTemp);

	if( strTemp == "" && strValue != "")
	{
		strTemp = strValue;
		SetValue(strTemp);
	}

	return strTemp;
}



bool CXmlSubNode::GetBool( bool bDefault /* = false */)
{
	std::string strValue;
	GetValue(strValue);

	if( strValue == "true" || strValue == "TRUE")
	{
		return true;
	}
	else if( strValue == "false" || strValue == "FALSE")
	{
		return false;
	}
	else
	{
		strValue = bDefault ? "true" : "false";
		SetValue(strValue);
		return bDefault;
	}	
}

int CXmlSubNode::GetInt( int nDefault /* = 0 */)
{
	std::string strValue;
	GetValue(strValue);

	if( strValue == "" )
	{
		char szBuf[128] = {0};
		sprintf_s(szBuf, "%d", nDefault);
		strValue = szBuf;
		SetValue(strValue);
	}

	return atoi(strValue.c_str());
}



double CXmlSubNode::GetFloat( double fDefault /* = 0.0 */)
{
	std::string strValue;
	GetValue(strValue);

	if( strValue == "" )
	{
		char szBuf[128] = {0};
		sprintf_s(szBuf, "%f", fDefault);
		strValue = szBuf;
		SetValue(strValue);
	}

	return atof(strValue.c_str());
}

bool CXmlSubNode::SetString( const std::string& strValue )
{
	return SetValue(strValue);
}

bool CXmlSubNode::SetBool( bool bValue )
{
	std::string strValue;
	strValue = bValue ? "true" : "false";

	return SetValue(strValue);
}

bool CXmlSubNode::SetInt( int nValue )
{
	std::string strValue;
	char szBuf[128] = {0};
	sprintf_s(szBuf, "%d", nValue);
	strValue = szBuf;

	return SetValue(strValue);
}

bool CXmlSubNode::SetFloat( float fValue )
{
	std::string strValue;
	char szBuf[128] = {0};
	sprintf_s(szBuf, "%f", fValue);
	strValue = szBuf;

	return SetValue(strValue);
}

void CXmlSubNode::SetNode( MSXML2::IXMLDOMNodePtr pNode)
{
	m_pNode = pNode;
}
