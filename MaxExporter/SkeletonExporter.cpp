#include "IGame/IGame.h"
#include <decomp.h> 

#include "BaseData.h"
#include "SkeletonExporter.h"

SkeletonExporter::SkeletonExporter(Config& config)
	 : Exporter(config)
{
}


SkeletonExporter::~SkeletonExporter(void)
{
}

bool SkeletonExporter::DoExport( IGameNode* pNode )
{
	IGameObject* pObj = pNode->GetIGameObject();
	if (pObj->GetIGameType() == IGameMesh::IGAME_BONE)
	{
		IGameSupportObject * pSupportObject = (IGameSupportObject*)pObj;
		IGameMesh *pMesh = pSupportObject->GetMeshObject();
		pMesh->InitializeData();
		{
			stBone bone;

			bone.m_iParentID = -1;
			IGameNode *pParentNode = pNode->GetNodeParent();
			if( NULL != pParentNode )
			{
				bone.m_iParentID = pParentNode->GetNodeID();
			}

			bone.m_iID = pNode->GetNodeID();
			bone.m_strName = pNode->GetName();
			bone.m_strType = "Bone";

			AffineParts ap;
			float rotAngle;
			Point3 rotAxis;
			float scaleAxAngle;
			Point3 scaleAxis;
			Matrix3 matWorldTM;
			Matrix3 matParentWorldTM;
			matWorldTM = pNode->GetWorldTM(0).ExtractMatrix3();

			decomp_affine(matWorldTM, &ap);
			AngAxisFromQ(ap.q, &rotAngle, rotAxis);
			AngAxisFromQ(ap.u, &scaleAxAngle, scaleAxis);

			bone.m_Translation.m_Translation = Point3(ap.t.x, ap.t.y, ap.t.z);
			bone.m_Translation.m_Rotation = Point4(ap.q.x, ap.q.y, ap.q.z, ap.q.w);
			bone.m_Translation.m_Scale = Point3(ap.k.x, ap.k.y, ap.k.z);

			m_vecSkeleton.push_back(bone);
			return true;
		}
	}

	return false;
}

const std::vector<stBone>& SkeletonExporter::GetSkeleton() const
{
	return m_vecSkeleton;
}
