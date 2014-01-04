#ifndef _BASE_DATA_H_
#define _BASE_DATA_H_

#include <limits>
#include <assert.h>
#include <string>

#include <vector>
#include <map>

#include "Max.h"

static bool IsEqual(float f1, float f2)
{
// return ( fabs(f1-f2) <= std::numeric_limits<float>::epsilon() );
	return ( fabs(f1-f2) <= 1e-6 );
}

struct stMaterial
{
	// ����ID
	int				m_iID;
	// ������
	std::string		m_strName;
	// ��������ɫ
	Color			m_Diffuse;
	// ��������ɫ
	Color			m_Ambient;
	// ���淴����ɫ
	Color			m_Specular;
	// ���淴��
	float			m_fSpecularLevel;
	// ��������
	std::vector<std::string>	m_vecTextureName;
	std::vector<stMaterial>	m_vecChildMaterial;
};

struct stTranslation
{
	// ƽ��
	Point3		 m_Translation;
	// ��ת
	Point4		 m_Rotation;
	// ����
	Point3		 m_Scale;
};

struct stNode
{
	// �ӵ�ID
	int m_iID;
	// ���ӵ�
	int m_iParentID;
	// �ӵ���
	std::string	 m_strName;
	// �ӵ�����
	std::string	 m_strType;
	// ����任
	stTranslation m_Translation;
};

struct stFace
{
	// ����
	int		m_iVertex[3];
	// ����
	int		m_iNormal[3];
	// ����
	int		m_iTexCoord[3];
};

struct stSkin
{
	int    m_iBoneID;
	float m_fWeight;
};

// ����ģ��
struct stMesh : public stNode
{
	// �������
	int			 m_iMatlIndex;
	// ��������
	std::vector<Point3>		m_vecVertex;
	// ��������
	std::vector<Point3>		m_vecNormal;
	// ��������
	std::vector<Point2>		m_vecTexCoord;
	// ����������
	std::vector<stFace>		m_vecFace;
	// ��Ƥ����
	std::vector<std::vector<stSkin>>		m_vecSkin;
};

struct stBone : public stNode
{
	std::map<int, stBone>		m_mapChildBone;
};

struct stKeyframe
{
	// ʱ������
	float					 m_fTimeIndex; 
	// �任
	stTranslation		 m_Translation;
};

struct stAnimationTrack
{
	// ��ͷID
	int						m_iBoneID;
	// ��ͷ�ؼ�֡
	std::vector<stKeyframe>	m_vecKeyframe;
};

struct stAnimation
{
	// ����
	std::string					m_strName;
	// ����
	float							m_fLength;
	//
	int							m_iFrameCount;
	// ����
	std::vector<stAnimationTrack>	m_vecTrack;
};

#endif
