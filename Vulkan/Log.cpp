#include "Log.h"

#include <string>

using namespace std;

Log::Log(time_t _timestamp,
	string _pcId,
	string _engine,
	string _objectName,
	float _frames,
	float _cpuUsage)
{
	m_timestamp = _timestamp;
	m_pcId = _pcId;
	m_engine = _engine;
	m_objectName = _objectName;
	m_frames = _frames;
	m_cpuUsage = _cpuUsage;
}

Log::~Log()
{
}
