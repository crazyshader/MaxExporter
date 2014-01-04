#ifndef _EXPORTER_H_
#define _EXPORTER_H_

class ExpInterface;
class Interface;
class Config;

class Exporter
{
public:
	Exporter(Config& config) : m_config(config) {}

	void setMaxInterface(ExpInterface* ei, Interface* i) { m_ei = ei; m_i = i; }

protected:
	Config&	m_config;
	ExpInterface *m_ei;
	Interface *m_i;
};

#endif