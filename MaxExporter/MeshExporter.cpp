#include "IGame/IGame.h"
#include "IGame/IGameModifier.h"
#include <decomp.h> 

#include "BaseData.h"
#include "MeshExporter.h"
#include <map>

MeshExporter::MeshExporter( Config& config)
 : Exporter(config)
{
}

MeshExporter::~MeshExporter(void)
{
}

bool MeshExporter::DoExport(IGameNode* pNode, std::map<int, float>& mapBone)
{
	IGameObject* pObj = pNode->GetIGameObject();
	if (pObj->GetIGameType() == IGameMesh::IGAME_MESH)
	{
		if (pObj->InitializeData())
		{
			stMesh mesh;

			mesh.m_iParentID = -1;
			IGameNode *pParentNode = pNode->GetNodeParent();
			if( NULL != pParentNode )
			{
				mesh.m_iParentID = pParentNode->GetNodeID();
			}

			mesh.m_iID = pNode->GetNodeID();
			mesh.m_strName = pNode->GetName();
			mesh.m_strType = "Mesh";
			mesh.m_iMatlIndex = pNode->GetMaterialIndex();

			AffineParts ap;
			float rotAngle;
			Point3 rotAxis;
			float scaleAxAngle;
			Point3 scaleAxis;
			Matrix3 matWorldTM = pNode->GetWorldTM(0).ExtractMatrix3();

			decomp_affine(matWorldTM, &ap);
			AngAxisFromQ(ap.q, &rotAngle, rotAxis);
			AngAxisFromQ(ap.u, &scaleAxAngle, scaleAxis);

			mesh.m_Translation.m_Translation = Point3(ap.t.x, ap.t.y, ap.t.z);
			mesh.m_Translation.m_Rotation = Point4(ap.q.x, ap.q.y, ap.q.z, ap.q.w);
			mesh.m_Translation.m_Scale = Point3(ap.k.x, ap.k.y, ap.k.z);

			IGameMesh* pMesh = (IGameMesh*) pObj;
			for (int i = 0; i < pMesh->GetNumberOfVerts(); ++i)
			{
				mesh.m_vecVertex.push_back(pMesh->GetVertex(i));
			}

			for (int i = 0; i < pMesh->GetNumberOfNormals(); ++i)
			{
				mesh.m_vecNormal.push_back(pMesh->GetNormal(i));
			}

			for (int i = 0; i < pMesh->GetNumberOfTexVerts(); ++i)
			{
				mesh.m_vecTexCoord.push_back(pMesh->GetTexVertex(i));
			}

			mesh.m_vecFace.resize(pMesh->GetNumberOfFaces());
			for (int i = 0; i < pMesh->GetNumberOfFaces(); ++i)
			{
				FaceEx *pFace = pMesh->GetFace(i);
				mesh.m_vecFace[i].m_iVertex[0] = pFace->vert[0];
				mesh.m_vecFace[i].m_iVertex[1] = pFace->vert[1];
				mesh.m_vecFace[i].m_iVertex[2] = pFace->vert[2];

				mesh.m_vecFace[i].m_iNormal[0] = pFace->norm[0];
				mesh.m_vecFace[i].m_iNormal[1] = pFace->norm[1];
				mesh.m_vecFace[i].m_iNormal[2] = pFace->norm[2];

				mesh.m_vecFace[i].m_iTexCoord[0] = pFace->texCoord[0];
				mesh.m_vecFace[i].m_iTexCoord[1] = pFace->texCoord[1];
				mesh.m_vecFace[i].m_iTexCoord[2] = pFace->texCoord[2];
			}

			if (pMesh->IsObjectSkinned())
			{
				IGameSkin *pSkin = pMesh->GetIGameSkin();
				int iNum =pSkin->GetNumOfSkinnedVerts();
				for (int i = 0; i < pSkin->GetNumOfSkinnedVerts(); ++i)
				{
					std::vector<stSkin> vecSkin;
					for (int j = 0; j < pSkin->GetNumberOfBones(i); ++j)
					{
						stSkin skin;
						skin.m_iBoneID = pSkin->GetBoneID(i, j);
						skin.m_fWeight = pSkin->GetWeight(i, j);
						vecSkin.push_back(skin);
						mapBone[skin.m_iBoneID] = skin.m_fWeight;
					}

					mesh.m_vecSkin.push_back(vecSkin);
				}
			}

			m_vecMesh.push_back(mesh);
			return true;
		}
	}

	return false;
}

const std::vector<stMesh>& MeshExporter::GetMesh() const
{
	return m_vecMesh;
}
