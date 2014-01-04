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
	// 材质ID
	int				m_iID;
	// 材质名
	std::string		m_strName;
	// 漫反射颜色
	Color			m_Diffuse;
	// 环境光颜色
	Color			m_Ambient;
	// 镜面反射颜色
	Color			m_Specular;
	// 镜面反射
	float			m_fSpecularLevel;
	// 材质纹理
	std::vector<std::string>	m_vecTextureName;
	std::vector<stMaterial>	m_vecChildMaterial;
};

struct stTranslation
{
	// 平移
	Point3		 m_Translation;
	// 旋转
	Point4		 m_Rotation;
	// 比例
	Point3		 m_Scale;
};

struct stNode
{
	// 接点ID
	int m_iID;
	// 父接点
	int m_iParentID;
	// 接点名
	std::string	 m_strName;
	// 接点类型
	std::string	 m_strType;
	// 网格变换
	stTranslation m_Translation;
};

struct stFace
{
	// 顶点
	int		m_iVertex[3];
	// 法线
	int		m_iNormal[3];
	// 纹理
	int		m_iTexCoord[3];
};

struct stSkin
{
	int    m_iBoneID;
	float m_fWeight;
};

// 网格模型
struct stMesh : public stNode
{
	// 网格材质
	int			 m_iMatlIndex;
	// 顶点数据
	std::vector<Point3>		m_vecVertex;
	// 法线数据
	std::vector<Point3>		m_vecNormal;
	// 纹理数据
	std::vector<Point2>		m_vecTexCoord;
	// 三角面数据
	std::vector<stFace>		m_vecFace;
	// 蒙皮数据
	std::vector<std::vector<stSkin>>		m_vecSkin;
};

struct stBone : public stNode
{
	std::map<int, stBone>		m_mapChildBone;
};

struct stKeyframe
{
	// 时间索引
	float					 m_fTimeIndex; 
	// 变换
	stTranslation		 m_Translation;
};

struct stAnimationTrack
{
	// 骨头ID
	int						m_iBoneID;
	// 骨头关键帧
	std::vector<stKeyframe>	m_vecKeyframe;
};

struct stAnimation
{
	// 名字
	std::string					m_strName;
	// 长度
	float							m_fLength;
	//
	int							m_iFrameCount;
	// 数据
	std::vector<stAnimationTrack>	m_vecTrack;
};

#endif
