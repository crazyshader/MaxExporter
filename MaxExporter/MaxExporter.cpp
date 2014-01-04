#include "resource.h"
#include "MaxExporter.h"

#include "IGame/IGame.h"
#include "IGame/IGameError.h"

#include "BaseData.h"
#include "MeshExporter.h"
#include "SkeletonExporter.h"
#include "AnimationExporter.h"
#include "MaterialExporter.h"
#include "SceneManager.h"

#include "3dsmaxport.h"

using namespace MaxExporter;

DWORD WINAPI fn(LPVOID arg)
{
	return(0);
}

class ExportErrorProc : public IGameErrorCallBack
{
public:
	void ErrorProc(IGameError error)
	{
		TCHAR * buf = GetLastIGameErrorText();
		DebugPrint("ErrorCode = %d ErrorText = %s\n", error,buf);
	}
};

INT_PTR CALLBACK MaxExporterOptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	unsigned int uiID = 0;
	MaxExport *pMaxExport = DLGetWindowLongPtr<MaxExport*>(hWnd); 

	switch(message)
	{
	case WM_INITDIALOG:
		{
			pMaxExport = (MaxExport *)lParam;
			if (pMaxExport == NULL)
			{
				MessageBox(NULL, "Error: Cannot initialize exporter options dialog, aborting", "Error", MB_ICONEXCLAMATION);
				EndDialog(hWnd, 0);
				return FALSE;
			}

			pMaxExport->SetWnd(hWnd);
			DLSetWindowLongPtr(hWnd, lParam);
			CenterWindow(hWnd,GetParent(hWnd));

			TCHAR szBuf[32] = {0};
			sprintf_s(szBuf, "%f", pMaxExport->GetConfig().GetConfigData().m_fExportScale);
			SetWindowText(GetDlgItem(hWnd, IDC_EDIT_SCALE), szBuf);
			sprintf_s(szBuf, "%d", pMaxExport->GetConfig().GetConfigData().m_iSampleFrame);
			SetWindowText(GetDlgItem(hWnd, IDC_EDIT_SAMPLE), szBuf);

			CheckDlgButton(hWnd,IDC_CHECK_MESH, pMaxExport->GetConfig().GetConfigData().m_bExportMesh);
			CheckDlgButton(hWnd,IDC_CHECK_MATERIAL, pMaxExport->GetConfig().GetConfigData().m_bExportMaterial);
			CheckDlgButton(hWnd,IDC_CHECK_SKELETON, pMaxExport->GetConfig().GetConfigData().m_bExportSkeleton);
			CheckDlgButton(hWnd,IDC_CHECK_ANIMATION, pMaxExport->GetConfig().GetConfigData().m_bExportAnimation);

			CheckDlgButton(hWnd,IDC_CHECK_NORMAL, pMaxExport->GetConfig().GetConfigData().m_bExportNormal);
			CheckDlgButton(hWnd,IDC_CHECK_COLOR, pMaxExport->GetConfig().GetConfigData().m_bExportVertexColors);
			CheckDlgButton(hWnd,IDC_CHECK_SAVEXML, pMaxExport->GetConfig().GetConfigData().m_bExportXMLFile);
			EnableWindow(GetDlgItem(hWnd, IDC_CHECK_SAVEXML), FALSE);

			return TRUE;
		}
		break;

	case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case IDC_CHECK_MESH:
				EnableWindow(GetDlgItem(hWnd, IDC_CHECK_NORMAL), IsDlgButtonChecked(hWnd, IDC_CHECK_MESH));
				EnableWindow(GetDlgItem(hWnd, IDC_CHECK_COLOR), IsDlgButtonChecked(hWnd, IDC_CHECK_MESH));
				return TRUE;

			case IDC_CHECK_ANIMATION:
				EnableWindow(GetDlgItem(hWnd, IDC_EDIT_SAMPLE), IsDlgButtonChecked(hWnd, IDC_CHECK_ANIMATION));
				return TRUE;

			case IDOK:
				{
					TCHAR szBuf[32] = {0};
					GetWindowText(GetDlgItem(hWnd, IDC_EDIT_SCALE), szBuf, 32);
					pMaxExport->GetConfig().GetConfigData().m_fExportScale = static_cast<float>(atof(szBuf));

					GetWindowText(GetDlgItem(hWnd, IDC_EDIT_SAMPLE), szBuf, 32);
					pMaxExport->GetConfig().GetConfigData().m_iSampleFrame = atoi(szBuf);

					pMaxExport->GetConfig().GetConfigData().m_bExportMesh		= IsDlgButtonChecked(hWnd, IDC_CHECK_MESH) != 0 ? true : false;
					pMaxExport->GetConfig().GetConfigData().m_bExportMaterial	= IsDlgButtonChecked(hWnd, IDC_CHECK_MATERIAL) != 0 ? true : false;
					pMaxExport->GetConfig().GetConfigData().m_bExportSkeleton	= IsDlgButtonChecked(hWnd, IDC_CHECK_SKELETON) != 0 ? true : false;
					pMaxExport->GetConfig().GetConfigData().m_bExportAnimation	= IsDlgButtonChecked(hWnd, IDC_CHECK_ANIMATION) != 0 ? true : false;
					pMaxExport->GetConfig().GetConfigData().m_bExportNormal		= IsDlgButtonChecked(hWnd, IDC_CHECK_NORMAL) != 0 ? true : false;
					pMaxExport->GetConfig().GetConfigData().m_bExportVertexColors = IsDlgButtonChecked(hWnd, IDC_CHECK_COLOR) != 0 ? true : false;
					pMaxExport->GetConfig().GetConfigData().m_bExportXMLFile	= IsDlgButtonChecked(hWnd, IDC_CHECK_SAVEXML) != 0 ? true : false;

					if (pMaxExport->DoExport())
					{
						EndDialog(hWnd, 1);
					}
					else
					{
						EndDialog(hWnd, 2);
					}

					return TRUE;
				}
				break;

			case IDCANCEL:
				{
					EndDialog(hWnd,0);
					return FALSE;
				}
			}

			return TRUE;
		}

	case WM_CLOSE:
		{
			EndDialog(hWnd, 0);
			return FALSE;
		}

	default:
		{
			return FALSE;
		}
	}

	return FALSE;
}

MaxExport::MaxExport(HINSTANCE hInst)
{
	m_hInstance				 = hInst;
	m_hWndDlgExport			 = NULL;
	m_bExportSelectedObjects = false;

	m_pMeshExporter		 = new MeshExporter(m_config);
	m_SkeletonExporter		 = new SkeletonExporter(m_config);
	m_pAnimationExporter = new AnimationExporter(m_config);
	m_pMaterialExporter  = new MaterialExporter(m_config);
	m_pSceneManager		 = new MaxExporter::SceneManager;
}

MaxExport::~MaxExport()
{
	if (NULL != m_pMeshExporter)
	{
		delete m_pMeshExporter;
		m_pMeshExporter = NULL;
	}

	if (NULL != m_SkeletonExporter)
	{
		delete m_SkeletonExporter;
		m_SkeletonExporter = NULL;
	}

	if (NULL != m_pAnimationExporter)
	{
		delete m_pAnimationExporter;
		m_pAnimationExporter = NULL;
	}

	if (NULL != m_pMaterialExporter)
	{
		delete m_pMaterialExporter;
		m_pMaterialExporter = NULL;
	}

	if (NULL != m_pSceneManager)
	{
		delete m_pSceneManager;
		m_pSceneManager = NULL;
	}
}

int MaxExport::ExtCount()
{
	return 1;
}

const TCHAR* MaxExport::Ext(int n)
{
	switch (n)
	{
	case 0:
		return _T("xml");
		break;
	default:
		return 0;
		break;
	}
}

const TCHAR* MaxExport::LongDesc()
{ 
	return _T("Max Exporter");
}

const TCHAR* MaxExport::ShortDesc()
{
	return _T("Max Exporter");
}

const TCHAR* MaxExport::AuthorName()
{ 
	return _T("Wu Jun");
}

const TCHAR* MaxExport::CopyrightMessage()
{ 
	return _T("The Aolan 3D Team (c) 2010");
}

const TCHAR* MaxExport::OtherMessage1()
{ 
	return 0;
}

const TCHAR* MaxExport::OtherMessage2()
{ 
	return 0;
}

unsigned int MaxExport::Version()
{ 
	return 100;
}

void MaxExport::ShowAbout(HWND hWnd)
{
	MessageBox(hWnd, _T("Aolan 3D Mesh, Material and Animation Exporter"), _T("About"), 0);
}

BOOL MaxExport::SupportsOptions(int ext, DWORD options)
{

	return TRUE;
}

int	MaxExport::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options) 
{
	m_config.SetExportPath(name);
	m_config.LoadConfig();

	ExportErrorProc pErrorProc;
	SetErrorCallBack(&pErrorProc);

	Interface* ip = GetCOREInterface();
	ip->ProgressStart(_T("Exporting to MaxExporter XML .."), TRUE, fn, NULL);

	m_pMeshExporter->setMaxInterface(ei, i);
	m_SkeletonExporter->setMaxInterface(ei, i);
	m_pAnimationExporter->setMaxInterface(ei, i);
	m_pMaterialExporter->setMaxInterface(ei, i);

	m_bExportSelectedObjects = (options & SCENE_EXPORT_SELECTED) ? true : false;

	INT_PTR iResult = DialogBoxParam(m_hInstance,
		MAKEINTRESOURCE(IDD_PANEL),
		i->GetMAXHWnd(),
		MaxExporterOptionsDlgProc,
		(LPARAM) this);

	ip->ProgressEnd();	

	switch (iResult)
	{
		case 0:
			return IMPEXP_CANCEL;
			break;
		case 1:
			MessageBox(GetActiveWindow(), "Export Succeeded", "Sucessful Export", MB_ICONINFORMATION);
			return IMPEXP_SUCCESS;
			break;
		default:
			MessageBox(GetActiveWindow(), "Export Failed", "Fail Export", MB_ICONINFORMATION);
			return IMPEXP_FAIL;
			break;
	}
}

bool MaxExport::DoExport()
{
	IGameScene*	m_pGame = GetIGameInterface();
	IGameConversionManager * cm = GetConversionManager();
	cm->SetCoordSystem(IGameConversionManager::IGAME_D3D);
	m_pGame->InitialiseIGame(m_bExportSelectedObjects);
	m_pGame->SetStaticFrame(0);

	if (!m_pMaterialExporter->DoExport())
	{
		assert(false);
		return false;
	}

	unsigned int iNodeCount = m_pGame->GetTopLevelNodeCount();
	for (unsigned int i = 0;i < iNodeCount;i++)
	{
		IGameNode* pNode = m_pGame->GetTopLevelNode(i);
		if (!NodeExport(pNode))
		{
			assert(false);
			return false;
		}

		TCHAR szTitle[MAX_PATH] = {0}; 
		sprintf_s(szTitle, "Processing: %s", pNode->GetName());
		GetCOREInterface()->ProgressUpdate((int)((float)(i+1)/(float)iNodeCount*100.0f),FALSE, szTitle); 
	}

	const std::vector<stMaterial>& vecMaterial = m_pMaterialExporter->GetMaterial();
	m_pSceneManager->SetMaterial(vecMaterial);
	const std::vector<stMesh>& vecMesh = m_pMeshExporter->GetMesh();
	m_pSceneManager->SetMesh(vecMesh);
	const std::vector<stBone>& vecBone = m_SkeletonExporter->GetSkeleton();
	m_pSceneManager->SetSkeleton(vecBone);
	const stAnimation& aimation = m_pAnimationExporter->GetAnimation();
	m_pSceneManager->SetAnimation(aimation);
	m_pSceneManager->SaveData(m_config.GetExportPath().c_str(), m_config);

	m_config.SaveConfig();
	return true;
}

bool MaxExport::NodeExport(IGameNode* pNode)
{
	static std::map<int, float> mapBone;
	IGameObject* pObj = pNode->GetIGameObject();
	if (pObj->GetIGameType() == IGameMesh::IGAME_MESH)
	{
		if (!m_pMeshExporter->DoExport(pNode, mapBone))
		{
			assert(false);
			return false;
		}	
	}
	else if (pObj->GetIGameType() == IGameMesh::IGAME_BONE)
	{
		int iNodeID = pNode->GetNodeID();
		std::map<int, float>::const_iterator iter = mapBone.find(iNodeID);
		if (iter != mapBone.end())
		{
			m_pAnimationExporter->InitAnimation("Attack");
			if (!m_SkeletonExporter->DoExport(pNode) ||
				!m_pAnimationExporter->DoExport(pNode))
			{
				assert(false);
				return false;
			}
		}
	}

	for(int i=0; i < pNode->GetChildCount(); ++i)
	{
		IGameNode * pNewchild = pNode->GetNodeChild(i);		
		if (!NodeExport(pNewchild))
		{
			assert(false);
			return false;
		}
	}

	return true;
}

void MaxExport::SetWnd( HWND hWnd )
{
	m_hWndDlgExport = hWnd;
}

Config& MaxExport::GetConfig()
{
	return m_config;
}

static MaxExportClassDesc MaxExportDescInst;
ClassDesc2* GetIGameExporterDesc()
{
	return &MaxExportDescInst;
}

int MaxExportClassDesc::IsPublic() 
{
	return 1; 
}

void* MaxExportClassDesc::Create(BOOL loading)
{ 
	MaxExport* pMaxExport = new MaxExport(hInstance); 
	return pMaxExport;
}

const TCHAR* MaxExportClassDesc::ClassName()
{ 
	return _T("3DSMax Exporter"); 
}

SClass_ID MaxExportClassDesc::SuperClassID()
{ 
	return SCENE_EXPORT_CLASS_ID; 
}

Class_ID MaxExportClassDesc::ClassID()
{ 
	return Class_ID(0x2a961d1d, 0x8160db1); 
}

const TCHAR* MaxExportClassDesc::Category()
{ 
	return _T("3DSMax Exporter"); 
}


TCHAR *GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}

extern "C"
{
	__declspec( dllexport ) const TCHAR* LibDescription()
	{ 
		return _T("3DSMax Exporter"); 
	}

	__declspec( dllexport ) int LibNumberClasses()
	{ 
		return 1; 
	}

	__declspec( dllexport ) ClassDesc* LibClassDesc(int i) 
	{
		switch(i)
		{
		case 0: 
			return GetIGameExporterDesc(); 
			break;
		default: 
			return 0; 
			break;
		}
	}

	__declspec( dllexport ) ULONG LibVersion()
	{ 
		return VERSION_3DSMAX; 
	}

	__declspec( dllexport ) ULONG CanAutoDefer()
	{
		return 1;
	}

}
