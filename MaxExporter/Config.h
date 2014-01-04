#ifndef _CONFIG_H_
#define _CONFIG_H_
#include <string>

struct ConfigData
{
	float				m_fExportScale;
	int					m_iSampleFrame;
	bool				m_bExportMesh;
	bool				m_bExportMaterial;
	bool				m_bExportSkeleton;
	bool				m_bExportAnimation;
	bool				m_bExportNormal;
	bool				m_bExportVertexColors;
	bool				m_bExportXMLFile;

	ConfigData();
};

class Config
{
public:
	Config();
	virtual ~Config();

	void SaveConfig();
	void LoadConfig();

	static std::string Config::GetCfgFilename();

	void SetConfigData(const ConfigData& configdata){ m_ConfigData = configdata; }
	ConfigData& GetConfigData() { return m_ConfigData; }

	void SetExportPath(const std::string& strExportPath){ m_strExportPath = strExportPath; }
	const std::string& GetExportPath()const { return m_strExportPath; }

private:
	std::string m_strExportPath;
	ConfigData  m_ConfigData;
};

#endif
