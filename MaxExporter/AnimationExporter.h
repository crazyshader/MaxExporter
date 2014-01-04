#ifndef _ANIMATION_EXPORTER_H_
#define _ANIMATION_EXPORTER_H_
#include "Exporter.h"

class IGameNode;

class AnimationExporter  : public Exporter
{
public:
	AnimationExporter(Config& config);
	~AnimationExporter(void);

public:
	bool DoExport(IGameNode* pNode);
	bool InitAnimation(const std::string& strName);
	const stAnimation& GetAnimation() const;

private:
	stAnimation				m_Animation;
};

#endif
