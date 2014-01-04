#include "IGame/IGame.h"
#include "IGame/IGameModifier.h"
#include <decomp.h> 

#include "Config.h"
#include "BaseData.h"
#include "AnimationExporter.h"

AnimationExporter::AnimationExporter(Config& config)
: Exporter(config)
{
}

AnimationExporter::~AnimationExporter(void)
{
}

const stAnimation& AnimationExporter::GetAnimation() const
{
	return m_Animation;
}

bool AnimationExporter::InitAnimation( const std::string& strName)
{
	IGameScene* pGameScene = GetIGameInterface();	
	Interval interval	 = m_i->GetAnimRange();

	m_Animation.m_strName = strName;
	m_Animation.m_iFrameCount = (interval.End() - interval.Start()) / GetTicksPerFrame() + 1; 
	m_Animation.m_fLength = (float)(interval.End() - interval.Start()) / (float)(pGameScene->GetSceneTicks() * GetFrameRate());

	return true;
}

bool AnimationExporter::DoExport(IGameNode* pNode)
{
	IGameObject* pObj = pNode->GetIGameObject();
	if (pObj->GetIGameType() == IGameMesh::IGAME_BONE)
	{
		stAnimationTrack aniTrack;

		IGameKeyTab SampleKey;
		IGameControl *pGameControl = pNode->GetIGameControl();

		int iFrameRate = m_config.GetConfigData().m_iSampleFrame;
		IGameScene* pGameScene = GetIGameInterface();
		int iTick = pGameScene->GetSceneTicks();

		iFrameRate = iFrameRate > 0 ? iFrameRate : 5;
		aniTrack.m_iBoneID = pNode->GetNodeID();
		if( pGameControl->GetFullSampledKeys(SampleKey, iFrameRate, IGameControlType(IGAME_TM), true) )
		{
			for(int i=0;i<SampleKey.Count();i++)
			{
				stKeyframe keyframe;
				keyframe.m_fTimeIndex = (float)(SampleKey[i].t) / (float)(iTick * GetFrameRate());

				AffineParts ap;
				float rotAngle;
				Point3 rotAxis;
				float scaleAxAngle;
				Point3 scaleAxis;
				Matrix3 matSampleKey;
				matSampleKey = SampleKey[i].sampleKey.gval.ExtractMatrix3();

				Point3 row1 = matSampleKey.GetRow(0);
				Point3 row2 = matSampleKey.GetRow(1);
				Point3 row3 = matSampleKey.GetRow(2);
				Point3 row4 = matSampleKey.GetRow(3);

				decomp_affine(matSampleKey, &ap);
				AngAxisFromQ(ap.q, &rotAngle, rotAxis);
				AngAxisFromQ(ap.u, &scaleAxAngle, scaleAxis);

				keyframe.m_Translation.m_Translation = Point3(ap.t.x, ap.t.y, ap.t.z);
				keyframe.m_Translation.m_Rotation	   = Point4(ap.q.x, ap.q.y, ap.q.z, ap.q.w);
				keyframe.m_Translation.m_Scale	   = Point3(ap.k.x, ap.k.y, ap.k.z);

				aniTrack.m_vecKeyframe.push_back(keyframe);
			}

			m_Animation.m_vecTrack.push_back(aniTrack);
			return true;
		}
	}

	return false;
}
