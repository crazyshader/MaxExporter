#ifndef _MESH_EXPORTER_H
#define _MESH_EXPORTER_H
#include "Exporter.h"

class IGameNode;

class MeshExporter  : public Exporter
{
public:
	MeshExporter(Config& config);
	virtual~MeshExporter(void);

public:
	bool DoExport(IGameNode* pNode, std::map<int, float>& mapBone);
	const std::vector<stMesh>& GetMesh() const;

private:
	std::vector<stMesh>	m_vecMesh;
};

#endif
