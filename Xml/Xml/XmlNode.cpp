#include "Xml.h"

CXmlNode::CXmlNode(void)
: m_pNodeList(NULL)
{
}

CXmlNode::CXmlNode( CXmlNode& refNodes )
{
	m_pNodeList = refNodes.m_pNodeList;
}

CXmlNode::CXmlNode( MSXML2::IXMLDOMNodeListPtr pNodeList )
{
	m_pNodeList = pNodeList;
}

CXmlNode::~CXmlNode(void)
{
	SAFE_RELEASE(m_pNodeList);
}

CXmlNode* CXmlNode::operator=( CXmlNode* pNodes )
{
	if (NULL == pNodes)
	{
		assert(NULL != pNodes);
		return NULL;
	}

	SAFE_RELEASE(m_pNodeList);
	m_pNodeList = pNodes->m_pNodeList;

	return pNodes;
}

CXmlNode& CXmlNode::operator=( CXmlNode& refNodes )
{
	SAFE_RELEASE(m_pNodeList);

	m_pNodeList = refNodes.m_pNodeList;

	return (*this);
}

CXmlSubNode* CXmlNode::operator[]( unsigned int unIndex )
{
	return GetItem(unIndex);
}

CXmlSubNode* CXmlNode::operator[]( const std::string& strName )
{
	return GetItem(strName);
}

unsigned int CXmlNode::GetCount( void )
{
	if (NULL == m_pNodeList)
	{
		assert(NULL != m_pNodeList);
		return 0;
	}

	unsigned int uiCount = 0l;
	HRESULT hr = S_OK;
	MSXML2::DOMNodeType NodeType;
	MSXML2::IXMLDOMNodePtr pNode = NULL;

	for( int i = 0; i < m_pNodeList->length; i++)
	{
		pNode = m_pNodeList->item[i];
		hr = pNode->get_nodeType(&NodeType);
		if (!SUCCEEDED(hr))
		{
			assert( SUCCEEDED(hr) );
			return 0;
		}

		if( NodeType == MSXML2::NODE_ELEMENT )
		{
			uiCount ++;
		}

		SAFE_RELEASE(pNode);
	}

	return uiCount;
}

CXmlSubNode* CXmlNode::GetItem( unsigned int uiIndex )
{
	if (uiIndex < 0 || uiIndex >= GetCount())
	{
		assert( uiIndex >= 0 && uiIndex < GetCount() );
		return NULL;
	}

	unsigned int uiCount = 0l;
	HRESULT hr = S_OK;
	MSXML2::DOMNodeType NodeType;
	MSXML2::IXMLDOMNodePtr pItem = NULL;

	CXmlSubNode* pNode ( new CXmlSubNode() );
	if (NULL == pNode)
	{
		assert(NULL != pNode);
		return NULL;
	}

	for( int i = 0; i < m_pNodeList->length; i++)
	{
		pItem = m_pNodeList->item[i];

		hr = pItem->get_nodeType(&NodeType);
		if (!SUCCEEDED(hr))
		{
			assert( SUCCEEDED(hr) );
			return NULL;
		}

		if( NodeType == MSXML2::NODE_ELEMENT )
		{
			if( uiCount == uiIndex )
			{
				pNode->SetNode(pItem);
				SAFE_RELEASE(pItem);

				return pNode;
			}

			uiCount ++;
		}

		SAFE_RELEASE(pItem);
	}

	return pNode;
}

CXmlSubNode* CXmlNode::GetItem( const std::string& strName )
{
	HRESULT hr = S_OK;
	std::string strTemp;
	BSTR bstr = NULL;
	MSXML2::IXMLDOMNodePtr pItem = NULL;

	CXmlSubNode* pNode ( new CXmlSubNode() );
	if (NULL == pNode)
	{
		assert(NULL != pNode);
		return NULL;
	}

	for( int i = 0; i < m_pNodeList->length; i++)
	{
		pItem = m_pNodeList->item[i];
		hr = pItem->get_nodeName(&bstr);
		if (!SUCCEEDED(hr))
		{
			assert( SUCCEEDED(hr) );
			return NULL;
		}

		strTemp = _bstr_t( bstr, true);
		if( bstr != NULL )
		{
			SysFreeString(bstr);
			bstr = NULL;
		}

		if( strTemp == strName )
		{
			pNode->SetNode(pItem);
			SAFE_RELEASE(pItem);

			return pNode;
		}

		SAFE_RELEASE(pItem);
	}

	return pNode;
}
