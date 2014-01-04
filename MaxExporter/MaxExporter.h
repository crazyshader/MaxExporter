#ifndef _MAX_EXPORTER_H_
#define _MAX_EXPORTER_H_

#include "Max.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"

#include "Config.h"

extern HINSTANCE hInstance;

class IGameNode;
class MeshExporter;
class SkeletonExporter;
class MaterialExporter;
class AnimationExporter;

namespace MaxExporter
{
	class SceneManager;
}

class MaxExport : public SceneExport
{ 

	friend INT_PTR CALLBACK GameExporterOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam);

public:
	MaxExport(HINSTANCE hInst);
	virtual ~MaxExport();
	virtual int ExtCount();					// Number of extemsions supported
	virtual const TCHAR * Ext(int n);					// Extension #n (i.e. "3DS")
	virtual const TCHAR * LongDesc();					// Long ASCII description (i.e. "Autodesk 3D Studio File")
	virtual const TCHAR * ShortDesc();				// Short ASCII description (i.e. "3D Studio")
	virtual const TCHAR * AuthorName();				// ASCII Author name
	virtual const TCHAR * CopyrightMessage();			// ASCII Copyright message
	virtual const TCHAR * OtherMessage1();			// Other message #1
	virtual const TCHAR * OtherMessage2();			// Other message #2
	virtual unsigned int Version();					// Version number * 100 (i.e. v3.01 = 301)
	virtual void ShowAbout(HWND hWnd);		// Show DLL's "About..." box
	virtual int	DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0);	// Export file
	virtual BOOL SupportsOptions(int ext, DWORD options); // Returns TRUE if all option bits set are supported for this extension

public:
	void SetWnd(HWND hWnd);
	Config& GetConfig();

	bool DoExport();
	bool NodeExport(IGameNode* pNode);

protected:
	Config m_config;
	MaxExporter::SceneManager* m_pSceneManager;

	HINSTANCE			m_hInstance;
	HWND				m_hWndDlgExport;
	bool				m_bExportSelectedObjects;

	MeshExporter*		m_pMeshExporter;
	SkeletonExporter* m_SkeletonExporter;
	AnimationExporter*	m_pAnimationExporter;
	MaterialExporter*	m_pMaterialExporter;
};

class MaxExportClassDesc : public ClassDesc2
{
public:
	int IsPublic();
	void * Create(BOOL loading = FALSE);
	const TCHAR * ClassName();
	SClass_ID SuperClassID();
	Class_ID ClassID();
	const TCHAR* Category();

	const TCHAR*	InternalName() { return _T("IGameExporter"); }
	HINSTANCE		HInstance() { return hInstance; }
};

#endif
