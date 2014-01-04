#include "IGame/IGame.h"
#include "BaseData.h"
#include "MaterialExporter.h"

MaterialExporter::MaterialExporter(Config& config)
:Exporter(config)
{
}

MaterialExporter::~MaterialExporter(void)
{
}

const std::vector<stMaterial>& MaterialExporter::GetMaterial() const
{
	return m_vecMaterial;
}

bool MaterialExporter::DoExport()
{
	IGameScene*	m_pGame = GetIGameInterface();
	unsigned int uiMatlCount = m_pGame->GetRootMaterialCount();
	for(unsigned int j = 0;j<uiMatlCount;j++)
	{
		IGameMaterial* mat = m_pGame->GetRootMaterial(j);
		stMaterial material = DoExport(mat, j);
		m_vecMaterial.push_back(material);
	}
	
	return true;
}

stMaterial MaterialExporter::DoExport(IGameMaterial* mat, int index, int matID )
{
	stMaterial material;
	material.m_iID = matID;
	material.m_strName = mat->GetMaterialName();

	if(!mat->IsMultiType())
	{
		float fValue;
		Point3 point; 
		IGameProperty *prop = NULL;

		prop = mat->GetDiffuseData();
		prop->GetPropertyValue(point);
		material.m_Diffuse = Color(point.x, point.y, point.z);

		prop = mat->GetAmbientData();
		prop->GetPropertyValue(point);
		material.m_Ambient = Color(point.x, point.y, point.z);

		prop = mat->GetSpecularData();
		prop->GetPropertyValue(point);
		material.m_Specular = Color(point.x, point.y, point.z);

		prop = mat->GetSpecularLevelData();
		prop->GetPropertyValue(fValue);
		material.m_fSpecularLevel = fValue;

		int texCount = mat->GetNumberOfTextureMaps();
		for(int i=0; i < texCount; i++)
		{
			IGameTextureMap* pTextureMap = mat->GetIGameTextureMap(i);
			material.m_vecTextureName.push_back(pTextureMap->GetBitmapFileName());
		}
	}
	else
	{
		int iSubMatlCount = mat->GetSubMaterialCount();
		for(int k=0;k<iSubMatlCount;k++)
		{
			int mID = mat->GetMaterialID(k);
			IGameMaterial *pSubMat = mat->GetSubMaterial(k);
			stMaterial subMaterial = DoExport(pSubMat,k, mID);
			material.m_vecChildMaterial.push_back(subMaterial);
		}
	}

	return material;
}
