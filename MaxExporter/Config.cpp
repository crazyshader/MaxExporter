#include <fstream>
#include "IGame/IGame.h"

#include "Config.h"

ConfigData::ConfigData()
{
	m_fExportScale = 1.0f;
	m_iSampleFrame = 5;
	m_bExportMesh = true;
	m_bExportMaterial = true;
	m_bExportSkeleton = true;
	m_bExportAnimation = true;
	m_bExportVertexColors = true;
	m_bExportNormal = true;
	m_bExportXMLFile = true;
}

Config::Config()
{
	ZeroMemory(&m_ConfigData, sizeof(ConfigData));
}

Config::~Config()
{
}


std::string Config::GetCfgFilename()
{
	std::string strFileName;

	strFileName += GetCOREInterface()->GetDir(APP_PLUGCFG_DIR);
	strFileName += "\\";
	strFileName += "MaxExport.cfg";
	return strFileName;
}

void Config::LoadConfig()
{
	std::ifstream fstream;
	fstream.open(GetCfgFilename().c_str(), std::ios::binary | std::ios::in);
	if (fstream.is_open())
	{
		fstream.read((char *)&m_ConfigData, sizeof(ConfigData));
		fstream.close();
	}
}

void Config::SaveConfig()
{
	std::ofstream fstream;
	fstream.open(GetCfgFilename().c_str(), std::ios::binary | std::ios::out);
	if (fstream.is_open())
	{
		fstream.write((char *)&m_ConfigData, sizeof(ConfigData));
		fstream.close();
	}
}
