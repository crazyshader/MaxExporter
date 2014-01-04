#include "BaseData.h"
#include "IGame/IGame.h"
#include "SceneManager.h"

using namespace MaxExporter;

SceneManager::SceneManager(void)
{
}

SceneManager::~SceneManager(void)
{
}

bool MaxExporter::SceneManager::SaveData(const char* pszFileName, const Config& configFile)
{
	if( !m_XmlDoc.Create(pszFileName))
	{
		MessageBox( NULL, m_XmlDoc.GetLastError().c_str(), "Create XML file failed!", MB_OK );
		return false;
	}

	m_configFile = configFile;

	CXmlSubNode* pNode = NULL;
	TCHAR szBuf[MAX_PATH] = {0};
	IGameScene* pGameScene = GetIGameInterface();
	CXmlSubNode* pRootNode = m_XmlDoc.AddRoot("SceneInfo");
	pRootNode->SetAttribute("FileName", pGameScene->GetSceneFileName());
	pRootNode->SetAttribute("Version", "1.0");

	time_t aclock;
	struct tm newtime;
	time( &aclock );
	localtime_s(&newtime, &aclock);
	asctime_s(szBuf, &newtime);
	std::string strTime =  szBuf;
	strTime = strTime.substr(0, strTime.length()-1);
	pRootNode->SetAttribute("Data", strTime);

	// 保存材质
	if (m_configFile.GetConfigData().m_bExportMaterial)
	{
		sprintf_s(szBuf, "%d", m_vecMaterial.size());
		pNode = pRootNode->AddChild("MaterialList");
		pNode->SetAttribute("Count", szBuf);
		for (unsigned int i = 0; i < m_vecMaterial.size(); ++i)
		{
			if (!ExportMaterial(pNode, i, m_vecMaterial[i]))
			{
				assert(false);
				return false;
			}
		}
	}

	// 保存网格
	if (m_configFile.GetConfigData().m_bExportMesh)
	{
		sprintf_s(szBuf, "%d", m_vecMesh.size());
		pNode = pRootNode->AddChild("NodeList");
		pNode->SetAttribute("Count", szBuf);
		for (unsigned int i = 0; i < m_vecMesh.size(); ++i)
		{
			if (!ExportMesh(pNode, m_vecMesh[i]) )
			{
				assert(false);
				return false;
			}
		}
	}

	// 保存骨骼
	if (m_configFile.GetConfigData().m_bExportSkeleton)
	{
		sprintf_s(szBuf, "%d", m_vecBone.size());
		pNode = pRootNode->AddChild("Skeleton");
		pNode->SetAttribute("Count", szBuf);
		for (unsigned int i = 0; i < m_vecBone.size(); ++i)
		{
			if (!ExportSkeleton(pNode, m_vecBone[i]))
			{
				assert(false);
				return false;
			}
		}
	}

	// 保存动画
	if (m_configFile.GetConfigData().m_bExportAnimation)
	{
		const std::vector<stAnimationTrack> aniTrack = m_Animation.m_vecTrack;
		if (!aniTrack.empty())
		{
			pNode = pRootNode->AddChild("Animation");
// 			pNode->SetAttribute("Name", m_Animation.GetName());
			sprintf_s(szBuf, "%f", m_Animation.m_fLength);
			pNode->SetAttribute("Length", szBuf);
			sprintf_s(szBuf, "%d", m_Animation.m_iFrameCount);
			pNode->SetAttribute("FrameCount", szBuf);
			pNode = pNode->AddChild("AnimationTrack");
			sprintf_s(szBuf, "%d", aniTrack.size());
			pNode->SetAttribute("Count", szBuf);
			for (unsigned int i = 0; i < aniTrack.size(); ++i)
			{
				if (!ExportAnimation(pNode, aniTrack[i]))
				{
					assert(false);
					return false;
				}
			}
		}
	}

	m_XmlDoc.Save();

	return true;
}

bool MaxExporter::SceneManager::LoadData(const char* pszFileName, const Config& configFile)
{
	return true;
}

void MaxExporter::SceneManager::SetMaterial(const std::vector<stMaterial>& vecMaterial)
{
	m_vecMaterial = vecMaterial;
}

void MaxExporter::SceneManager::SetMesh( const std::vector<stMesh>& vecNode )
{
	m_vecMesh = vecNode;
}

void MaxExporter::SceneManager::SetAnimation( const stAnimation& animation )
{
	m_Animation = animation;
}

void MaxExporter::SceneManager::SetSkeleton( const std::vector<stBone>& vecBone )
{
	m_vecBone = vecBone;
}

bool MaxExporter::SceneManager::ExportMesh( CXmlSubNode* const pMeshNode, const stMesh& mesh )
{
	if (NULL == pMeshNode && mesh.m_strType != "Mesh")
	{
		assert(false);
		return false;
	}

	TCHAR szBuf[MAX_PATH] = {0};
	sprintf_s(szBuf, "%d", mesh.m_iID);
	CXmlSubNode* pSubNode = pMeshNode->AddChild("Node");
	pSubNode->SetAttribute("Name", mesh.m_strName);
	pSubNode->SetAttribute("ID", szBuf);
	sprintf_s(szBuf, "%d",  mesh.m_iParentID);
	pSubNode->SetAttribute("ParentID", szBuf);
	sprintf_s(szBuf, "%d",  mesh.m_iMatlIndex);
	pSubNode->SetAttribute("MaterialID", szBuf);
	pSubNode->SetAttribute("NodeType", mesh.m_strType);

	CXmlSubNode* pSubNodeTM = pSubNode->AddChild("NodeTM");
	Point3 translation = mesh.m_Translation.m_Translation;
	sprintf_s(szBuf, "%f %f %f", translation.x, translation.y, translation.z);
	pSubNodeTM->AddChild("Translation")->SetString(szBuf);
	Point4 rotation = mesh.m_Translation.m_Rotation;
	sprintf_s(szBuf, "%f %f %f %f", rotation.x, rotation.y, rotation.z, rotation.w);
	pSubNodeTM->AddChild("Rotation")->SetString(szBuf);
	Point3 scale = mesh.m_Translation.m_Scale;
	sprintf_s(szBuf, "%f %f %f", scale.x, scale.y, scale.z);
	pSubNodeTM->AddChild("Scale")->SetString(szBuf);

	std::string strVertex;
		const std::vector<Point3>& vecVertex =  mesh.m_vecVertex;
		sprintf_s(szBuf, "%d", vecVertex.size());
		CXmlSubNode* pSubMeshNode = pSubNode->AddChild("Mesh");
		pSubMeshNode->AddChild("Vertices")->SetAttribute("Count", szBuf);
		for (unsigned int i = 0; i < vecVertex.size(); ++i )
		{
			sprintf_s(szBuf, "%f %f %f ", vecVertex[i].x, vecVertex[i].y, vecVertex[i].z);
			strVertex += szBuf;
		}
		pSubMeshNode->GetChild("Vertices")->SetString(strVertex);


		if (m_configFile.GetConfigData().m_bExportNormal)
		{
			const std::vector<Point3>& vecNormal =  mesh.m_vecNormal;
			sprintf_s(szBuf, "%d", vecNormal.size());
			pSubMeshNode->AddChild("Normals")->SetAttribute("Count", szBuf);

			std::string strNormal;
			for (unsigned int i = 0; i < vecNormal.size(); ++i )
			{
				sprintf_s(szBuf, "%f %f %f ", vecNormal[i].x, vecNormal[i].y, vecNormal[i].z);
				strNormal += szBuf;
			}

			pSubMeshNode->GetChild("Normals")->SetString(strNormal);
	}

		const std::vector<Point2>& vecTexCoords = mesh.m_vecTexCoord;
		sprintf_s(szBuf, "%d", vecTexCoords.size());
		pSubMeshNode->AddChild("Texture")->SetAttribute("Count", szBuf);
		std::string strTexture;
		for (unsigned int i = 0; i < vecTexCoords.size(); ++i )
		{
			sprintf_s(szBuf, "%f %f ", vecTexCoords[i].x, vecTexCoords[i].y);
			strTexture += szBuf;
		}
		pSubMeshNode->GetChild("Texture")->SetString(strTexture);

		const std::vector<stFace>& vecFace = mesh.m_vecFace;
		sprintf_s(szBuf, "%d", vecFace.size() );
		pSubMeshNode->AddChild("Face")->SetAttribute("Count", szBuf);

		std::string strFaceVertex, strFaceNormal, strFaceTexture;
		for (unsigned i = 0; i < vecFace.size(); ++i)
		{
			sprintf_s(szBuf, "%d %d %d ", vecFace[i].m_iVertex[0], vecFace[i].m_iVertex[1], vecFace[i].m_iVertex[2] );
			strFaceVertex += szBuf;

			sprintf_s(szBuf, "%d %d %d ", vecFace[i].m_iNormal[0], vecFace[i].m_iNormal[1], vecFace[i].m_iNormal[2] );
			strFaceNormal += szBuf;

			sprintf_s(szBuf, "%d %d %d ", vecFace[i].m_iTexCoord[0], vecFace[i].m_iTexCoord[1], vecFace[i].m_iTexCoord[2] );
			strFaceTexture += szBuf;
		}
		pSubMeshNode->GetChild("Face")->AddChild("Vertices")->SetString(strFaceVertex);
		pSubMeshNode->GetChild("Face")->AddChild("Noramls")->SetString(strFaceNormal);
		pSubMeshNode->GetChild("Face")->AddChild("TexCoords")->SetString(strFaceTexture);

		const std::vector<std::vector<stSkin>>& vecSkin = mesh.m_vecSkin;
		if (vecSkin.size() > 0)
		{			
			std::string strSkin = "";
			sprintf_s(szBuf, "%d", vecSkin.size());
			pSubMeshNode->AddChild("BoneAssignment")->SetAttribute("Count", szBuf);

			for (unsigned int i = 0; i < vecSkin.size(); ++i)
			{
				const std::vector<stSkin>& vecBoneSkin = vecSkin[i];
				for (unsigned int j = 0; j < vecBoneSkin.size(); ++j)
				{
					sprintf_s(szBuf, "%d %f ", vecBoneSkin[j].m_iBoneID,  vecBoneSkin[j].m_fWeight);
					strSkin += szBuf;
				}

				strSkin += "\n";
			}

			pSubMeshNode->GetChild("BoneAssignment")->SetString(strSkin);
		}

	return true;
}

bool MaxExporter::SceneManager::ExportMaterial(CXmlSubNode* const pMaterialNode, unsigned int uiIndex, const stMaterial& material )
{
	if (NULL == pMaterialNode)
	{
		return false;
	}

	TCHAR szBuf[MAX_PATH] = {0};
	sprintf_s(szBuf, "%d", uiIndex);
	CXmlSubNode* pSubMaterialNode = pMaterialNode->AddChild("Material");
	pSubMaterialNode->SetAttribute("Index", szBuf);
	pSubMaterialNode->SetAttribute("Name", material.m_strName);

	if (-1 != material.m_iID)
	{
		sprintf_s(szBuf, "%d", material.m_iID);
		pSubMaterialNode->SetAttribute("ID", szBuf);
	}

	const std::vector<stMaterial>& vecSubMaterial = material.m_vecChildMaterial;
	if (vecSubMaterial.empty())
	{
		Color color = material.m_Diffuse;
		sprintf_s(szBuf, "%f %f %f", color.r, color.g, color.b);
		pSubMaterialNode->AddChild("Diffuse")->SetAttribute("Value", szBuf);
		color = material.m_Ambient;
		sprintf_s(szBuf, "%f %f %f", color.r, color.g, color.b);
		pSubMaterialNode->AddChild("Ambient")->SetAttribute("Value", szBuf);
		color = material.m_Specular;
		sprintf_s(szBuf, "%f %f %f", color.r, color.g, color.b);
		pSubMaterialNode->AddChild("Specular")->SetAttribute("Value", szBuf);
		sprintf_s(szBuf, "%f", material.m_fSpecularLevel);
		pSubMaterialNode->AddChild("SpecularLevel")->SetAttribute("Value", szBuf);

		const std::vector<std::string>& vecTexture = material.m_vecTextureName;
		sprintf_s(szBuf, "%d", vecTexture.size());
		pSubMaterialNode->AddChild("TextureMaps")->SetAttribute("Count", szBuf);

		for (unsigned int i = 0; i < vecTexture.size(); ++i)
		{
			CXmlSubNode* pTextureNode = pSubMaterialNode->GetChild("TextureMaps")->AddChild("Texture");
			pTextureNode->SetAttribute("Filename", vecTexture[i]);
		}
	}
	else
	{
		sprintf_s(szBuf, "%d", vecSubMaterial.size());
		CXmlSubNode* pSubMaterialNode = pMaterialNode->AddChild("SubMaterials");
		pSubMaterialNode->SetAttribute("Count", szBuf);
		for ( unsigned int i = 0; i < vecSubMaterial.size(); ++i)
		{
			if (!ExportMaterial(pSubMaterialNode, i, vecSubMaterial[i]))
			{
				return false;
			}
		}
	}

	return true;
}

bool MaxExporter::SceneManager::ExportAnimation( CXmlSubNode* const pAnimationNode, const stAnimationTrack& aniTrack )
{
	if (NULL == pAnimationNode)
	{
		return false;
	}

	TCHAR szBuf[MAX_PATH] = {0};
	sprintf_s(szBuf, "%d", aniTrack.m_iBoneID);
	CXmlSubNode* pAniTrackNode = pAnimationNode->AddChild("AniTrack");
	pAniTrackNode->SetAttribute("Bone", szBuf);
	sprintf_s(szBuf, "%d", aniTrack.m_vecKeyframe.size());
	pAniTrackNode->SetAttribute("Count", szBuf);

	for (unsigned int i = 0; i < aniTrack.m_vecKeyframe.size(); ++i)
	{
		CXmlSubNode* pSampleNode = pAniTrackNode->AddChild("SampleKey");
		sprintf_s(szBuf, "%f", aniTrack.m_vecKeyframe[i].m_fTimeIndex);
		pSampleNode->SetAttribute("Frame", szBuf);

		Point3 translation = aniTrack.m_vecKeyframe[i].m_Translation.m_Translation;
		sprintf_s(szBuf, "%f %f %f", translation.x, translation.y, translation.z);
		pSampleNode->AddChild("Translation")->SetString(szBuf);
		Point4 rotation = aniTrack.m_vecKeyframe[i].m_Translation.m_Rotation;
		sprintf_s(szBuf, "%f %f %f %f", rotation.x, rotation.y, rotation.z, rotation.w);
		pSampleNode->AddChild("Rotation")->SetString(szBuf);
		Point3 scale = aniTrack.m_vecKeyframe[i].m_Translation.m_Scale;
		sprintf_s(szBuf, "%f %f %f", scale.x, scale.y, scale.z);
		pSampleNode->AddChild("Scale")->SetString(szBuf);
	}

	return true;
}

bool MaxExporter::SceneManager::ExportSkeleton( CXmlSubNode* const pMeshNode, const stBone& bone )
{
	if (NULL == pMeshNode && bone.m_strType != "Bone")
	{
		assert(false);
		return false;
	}

	TCHAR szBuf[MAX_PATH] = {0};
	sprintf_s(szBuf, "%d", bone.m_iID);
	CXmlSubNode* pSubNode = pMeshNode->AddChild("Bone");
	pSubNode->SetAttribute("Name", bone.m_strName);
	pSubNode->SetAttribute("ID", szBuf);
	sprintf_s(szBuf, "%d",  bone.m_iParentID);
	pSubNode->SetAttribute("ParentID", szBuf);
	pSubNode->SetAttribute("NodeType", bone.m_strType);

	CXmlSubNode* pSubNodeTM = pSubNode->AddChild("BoneTM");
	Point3 translation = bone.m_Translation.m_Translation;
	sprintf_s(szBuf, "%f %f %f", translation.x, translation.y, translation.z);
	pSubNodeTM->AddChild("Translation")->SetString(szBuf);
	Point4 rotation = bone.m_Translation.m_Rotation;
	sprintf_s(szBuf, "%f %f %f %f", rotation.x, rotation.y, rotation.z, rotation.w);
	pSubNodeTM->AddChild("Rotation")->SetString(szBuf);
	Point3 scale = bone.m_Translation.m_Scale;
	sprintf_s(szBuf, "%f %f %f", scale.x, scale.y, scale.z);
	pSubNodeTM->AddChild("Scale")->SetString(szBuf);

	return true;
}
