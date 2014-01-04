#ifndef _MATERIAL_EXPORT_H_
#define _MATERIAL_EXPORT_H_
#include "Exporter.h"

class IGameMaterial;

class MaterialExporter: public Exporter
{
public:
	MaterialExporter(Config& config);
	virtual ~MaterialExporter(void);

public:
	bool DoExport();
	const std::vector<stMaterial>& GetMaterial() const;

protected:
	stMaterial DoExport(IGameMaterial* mat, int index, int matID = -1);

private:
	std::vector<stMaterial> m_vecMaterial;
};

#endif
