#ifndef _XML_H_
#define _XML_H_
#include <assert.h>
#include <tchar.h>
#import <msxml6.dll>

#pragma warning(disable: 4251)

#ifdef XML_STATIC_EXPORTS
	#define XML_API
#else
	#ifdef XML_EXPORTS
		#define XML_API __declspec(dllexport)
	#else
		#define XML_API __declspec(dllimport)
	#endif
#endif

#define SAFE_RELEASE(x)	\
	if( x  != NULL )		\
	{					\
		x.Release();	\
		x = NULL;		\
	}

#include "XmlDoc.h"
#include "XmlNode.h"
#include "XmlSubNode.h"

#endif
