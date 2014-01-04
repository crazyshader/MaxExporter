#include "Xml.h"

CXmlDoc::CXmlDoc(void)
	: m_strFilePath("")
	, m_pDoc(NULL)
{
	CoInitialize(NULL);
}

CXmlDoc::~CXmlDoc(void)
{
	this->Close();
	CoUninitialize();
}

bool CXmlDoc::Create( const std::string& strXmlFilePath )
{
	HRESULT hr = S_OK;
	hr = m_pDoc.CreateInstance( __uuidof(MSXML2::DOMDocument60) );
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return false;
	}

	if( IsFileExist(strXmlFilePath) )
	{
		if (!DeleteFileA(strXmlFilePath.c_str()))
		{
			assert(false);
			return false;
		}
	}

	m_pDoc->preserveWhiteSpace = VARIANT_FALSE;
	m_pDoc->validateOnParse = VARIANT_FALSE;
	m_strFilePath = strXmlFilePath;

	MSXML2::IXMLDOMProcessingInstructionPtr pPI = NULL;
	pPI = m_pDoc->createProcessingInstruction( _bstr_t(_T("xml")), _bstr_t(_T("version=\"1.0\"")) );
	if (NULL == pPI)
	{
		assert(NULL != pPI);
		return false;
	}

	hr = m_pDoc->appendChild(pPI);
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return false;
	}

	return true;
}

bool CXmlDoc::Open( const std::string& strXmlFilePath )
{
	HRESULT hr = S_OK;
	hr = m_pDoc.CreateInstance( __uuidof(MSXML2::DOMDocument60) );
	if( !SUCCEEDED(hr) )
	{
		assert( SUCCEEDED(hr) );
		return false;
	}

	if( !IsFileExist(strXmlFilePath) )
	{
		assert(false);
		return false;
	}

	m_pDoc->preserveWhiteSpace = VARIANT_FALSE;
	m_pDoc->validateOnParse = VARIANT_FALSE;
	m_strFilePath = strXmlFilePath;

	VARIANT_BOOL vbSuccessful = VARIANT_FALSE;
	vbSuccessful = m_pDoc->load( (_variant_t)strXmlFilePath.c_str() );
	if ( vbSuccessful == VARIANT_FALSE)
	{
		assert(false);
		return false;
	}

	return true;
}

bool CXmlDoc::Save( const std::string& strXmlFilePath/* = "" */)
{
 	m_pDoc = Normalize(m_pDoc);
	if (NULL == m_pDoc)
	{
		assert( m_pDoc != NULL );
		return false;
	}

	HRESULT hr = S_OK;
	if( strXmlFilePath == "" )
	{
		hr = m_pDoc->save( _variant_t( m_strFilePath.c_str()) );
	}
	else
	{
		hr = m_pDoc->save( _variant_t( strXmlFilePath.c_str() ) );
	}

	if ( hr != S_OK)
	{
		assert( SUCCEEDED(hr) );
		return false;
	}

	return  true;
}

bool CXmlDoc::Close( void )
{
	m_pDoc = NULL;
	m_strFilePath = "";

	return true;
}

std::string CXmlDoc::GetLastError( void )
{
	if (NULL == m_pDoc)
	{
		assert( m_pDoc != NULL );
		return "";;
	}

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMParseErrorPtr pIParseError = NULL;
	hr = m_pDoc->get_parseError(&pIParseError);
	if( !SUCCEEDED(hr) )
	{
		assert( SUCCEEDED(hr) );
		return "";
	}

	std::string strRet;
	if( pIParseError != NULL )
	{
		LONG value;
		BSTR bstr = NULL;
		hr = pIParseError->get_errorCode(&value);
		if( !SUCCEEDED(hr) )
		{
			assert( SUCCEEDED(hr) );
			return "";;
		}

		hr = pIParseError->get_reason(&bstr);
		if( !SUCCEEDED(hr) )
		{
			assert( SUCCEEDED(hr) );
			return "";;
		}

		strRet = _bstr_t( bstr, true);
		if( bstr != NULL )
		{
			SysFreeString(bstr);
			bstr = NULL;
		}	

		SAFE_RELEASE(pIParseError);		
	}

	return strRet;
}

CXmlSubNode* CXmlDoc::AddRoot( const std::string& strName )
{
	if (NULL == m_pDoc)
	{
		assert( m_pDoc != NULL );
		return NULL;
	}

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMElementPtr pElement = NULL;
	hr = m_pDoc->get_documentElement(&pElement);
	if( !SUCCEEDED(hr) )
	{
		assert( SUCCEEDED(hr) );
		return NULL;
	}

	if( pElement != NULL )
	{
		assert(  pElement == NULL);
		return NULL;
	}

	if (strName != "")
	{
		pElement = m_pDoc->createElement( _bstr_t(strName.c_str()) );
	}
	else
	{
		pElement = m_pDoc->createElement( _bstr_t(_T("xmlRoot")) );
	}

	if( pElement == NULL )
	{
		assert(  pElement != NULL);
		return NULL;
	}

	m_pDoc->appendChild(pElement);
	CXmlSubNode* pNode( new CXmlSubNode(pElement) );
	SAFE_RELEASE(pElement);
	if( pNode == NULL )
	{
		assert(  pNode != NULL);
		return NULL;
	}

	return pNode;
}

CXmlSubNode* CXmlDoc::GetRoot( const std::string& strName )
{
	if (NULL == m_pDoc)
	{
		assert( m_pDoc != NULL );
		return NULL;
	}

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMElementPtr pElement = NULL;
	hr = m_pDoc->get_documentElement(&pElement);
	if( !SUCCEEDED(hr) )
	{
		assert( SUCCEEDED(hr) );
		return NULL;
	}

	if( pElement == NULL )
	{
		assert(  pElement != NULL);
		return NULL;
	}

	CXmlSubNode* pNode( new CXmlSubNode(pElement) );
	SAFE_RELEASE(pElement);
	if( pNode == NULL )
	{
		assert(  pNode != NULL);
		return NULL;
	}

	return pNode;
}

bool CXmlDoc::IsFileExist( const std::string& strFilePath )
{
	bool bExist  = false;
	HANDLE hFile = NULL;

	hFile = CreateFileA(strFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		bExist = true;
	}

	return bExist;
}

CXmlSubNode* CXmlDoc::GetSubNode( const std::string& strPath )
{
	if (NULL == m_pDoc)
	{
		assert( m_pDoc != NULL );
		return NULL;
	}

	MSXML2::IXMLDOMNodePtr pItem = NULL;
	pItem = m_pDoc->selectSingleNode( _bstr_t(strPath.c_str()) );
	if( pItem == NULL )
	{
		assert(  pItem != NULL);
		return NULL;
	}

	CXmlSubNode* pNode ( new CXmlSubNode(pItem) );
	SAFE_RELEASE(pItem);
	if( pNode == NULL )
	{
		assert(  pNode != NULL);
		return NULL;
	}

	return pNode;
}

CXmlNode* CXmlDoc::GetNode( const std::string& strPath )
{
	if (NULL == m_pDoc)
	{
		assert( m_pDoc != NULL );
		return NULL;
	}

	MSXML2::IXMLDOMNodeListPtr pNodeList = NULL;
	pNodeList =	m_pDoc->selectNodes( _bstr_t(strPath.c_str()) );
	if( pNodeList == NULL )
	{
		assert(  pNodeList != NULL);
		return NULL;
	}

	CXmlNode* pNodes ( new CXmlNode(pNodeList) );
	SAFE_RELEASE(pNodeList);
	if( pNodes == NULL )
	{
		assert(  pNodes != NULL);
		return NULL;
	}

	return pNodes;
}

MSXML2::IXMLDOMDocument2Ptr CXmlDoc::Normalize( MSXML2::IXMLDOMDocument2Ptr pXMLDocOld )
{
	static const std::string& strStyle=
		"<xsl:stylesheet version=\"1.0\""
		"      xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\">"
		"   <xsl:output method=\"xml\"/>"
		"   <xsl:param name=\"indent-increment\" select=\"'\t'\" />"
		"   <xsl:template match=\"node()\">"
		"      <xsl:param name=\"indent\" select=\"'&#xA;'\"/>"
		"      <xsl:value-of select=\"$indent\"/>"
		"      <xsl:copy>"
		"        <xsl:copy-of select=\"@*\" />"
		"        <xsl:apply-templates>"
		"          <xsl:with-param name=\"indent\""
		"               select=\"concat($indent, $indent-increment)\"/>"
		"        </xsl:apply-templates>"
		"        <xsl:if test=\"node()\">"
		"          <xsl:value-of select=\"$indent\"/>"
		"        </xsl:if>"
		"      </xsl:copy>"
		"   </xsl:template>"
		"</xsl:stylesheet>";

	if (NULL == pXMLDocOld)
	{
		assert(NULL != pXMLDocOld);
		return NULL;
	}

	HRESULT hr = S_OK;
	MSXML2::IXMLDOMDocument2Ptr pStyle=NULL;
	hr = pStyle.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return NULL;
	}

	VARIANT_BOOL vbSuccessful = VARIANT_FALSE;
	vbSuccessful = pStyle->loadXML( _bstr_t(strStyle.c_str()));
	if ( vbSuccessful == VARIANT_FALSE)
	{
		assert(false);
		return NULL;
	}

	MSXML2::IXMLDOMDocument2Ptr pNew=NULL;
	hr = pNew.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (!SUCCEEDED(hr))
	{
		assert( SUCCEEDED(hr) );
		return NULL;
	}

	vbSuccessful = VARIANT_FALSE;
	_bstr_t bstrXml = pXMLDocOld->transformNode(pStyle);
	vbSuccessful = pNew->loadXML(bstrXml);
	if ( vbSuccessful == VARIANT_FALSE)
	{
		assert(false);
		return NULL;
	}

	return pNew;
}
