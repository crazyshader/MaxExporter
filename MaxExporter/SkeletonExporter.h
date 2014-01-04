#ifndef _SKELETON_EXPORTOR_H_
#define _SKELETON_EXPORTOR_H_
#include "Exporter.h"

class IGameNode;

class SkeletonExporter  : public Exporter
{
public:
	SkeletonExporter(Config& config);
	virtual ~SkeletonExporter(void);

public:
	bool DoExport(IGameNode* pNode);
	const std::vector<stBone>& GetSkeleton() const;

private:
	std::vector<stBone>	m_vecSkeleton;
};

#endif
