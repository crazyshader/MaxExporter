#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

#include "Xml.h"
#include "Config.h"

#pragma comment(lib, "Xml_Static_d.lib")

namespace MaxExporter
{
	class SceneManager
	{
	public:
		SceneManager(void);
		~SceneManager(void);

	public:
		void Release();

		bool	SaveData(const char* pszFileName, const Config& configFile);
		bool	LoadData(const char* pszFileName, const Config& configFile);

		void	SetMaterial(const std::vector<stMaterial>& vecMaterial);
		void	SetMesh(const std::vector<stMesh>& vecMesh);
		void	SetSkeleton(const std::vector<stBone>& vecBone);
		void	SetAnimation(const stAnimation& animation);

	private:
		bool	ExportMesh(CXmlSubNode* const pMeshNode, const stMesh& mesh);
		bool	ExportSkeleton(CXmlSubNode* const pMeshNode, const stBone& bone);
		bool	ExportMaterial(CXmlSubNode* const pMaterialNode, unsigned int uiIndex, const stMaterial& material);
		bool	ExportAnimation(CXmlSubNode* const pAnimationNode, const stAnimationTrack& aniTrack);

	private:

		Config	m_configFile;
		CXmlDoc m_XmlDoc;
		// 材质
		std::vector<stMaterial>	m_vecMaterial;
		// 节点
		std::vector<stMesh>		m_vecMesh;
		//
		std::vector<stBone>			m_vecBone;
		// 动画
		stAnimation						m_Animation;
	};
}

#endif
