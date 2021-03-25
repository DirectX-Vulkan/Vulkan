#pragma once

#include <map>
#include <string>
#include <vector>
#include "Log.h"

using namespace std;

enum class TimeType
{
	rt,
	nrt
};

string getTimeTypeName(TimeType timeType) {
	switch (timeType)
	{
	case TimeType::rt:
		return "rt";
	case TimeType::nrt:
		return "nrt";
	default:
		break;
	}
};

class Logger {
public:
	Logger(string engine, string objectName, TimeType timeType = TimeType::rt, char separator = ';');
	~Logger(); // destructor

	void ExportLogFile();
	void AddLog(Log log);

private:
	string m_engine;
	string m_objectName;
	vector<Log> m_logs;
	TimeType m_timeType;
	char m_separator;
};
