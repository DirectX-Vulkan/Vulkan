#include "Logger.h"

#include <iostream>
#include <fstream>

using namespace std;

Logger::Logger(string engine, string objectName, TimeType timeType, char separator)
{
	m_engine = engine;
	m_objectName = objectName;
	m_logs = vector<Log>();
	m_timeType = timeType;
	m_separator = separator;
}

Logger::~Logger()
{
}

void Logger::ExportLogFile()
{
	ofstream file;
	file.open(getTimeTypeName(m_timeType) + "-data-" + m_engine + "-" + m_objectName + ".csv");

#pragma region --- csv header ---
	file << "timestamp" << m_separator
		<< "pc-id" << m_separator
		<< "engine" << m_separator
		<< "object" << m_separator;
	switch (m_timeType)
	{
	case TimeType::rt:
		file << "fps";
		break;
	case TimeType::nrt:
		file << "spf";
		break;
	default:
		throw runtime_error("unhandled time type");
		break;
	}
	file << m_separator << "cpu\n";
#pragma endregion

	//skip first logs as it's always inaccurate as the program will be in the process of booting up
	//by the time the logs are kept it should've stabilised
	for (size_t i = 2; i < m_logs.size(); i++)
	{
		file << m_logs[i].m_timestamp << m_separator
			<< m_logs[i].m_pcId << m_separator
			<< m_logs[i].m_engine << m_separator
			<< m_logs[i].m_objectName << m_separator
			<< m_logs[i].m_frames << m_separator
			<< m_logs[i].m_cpuUsage;
		//if not the last log, start a new line for the next log
		if (i < m_logs.size() - 1)
		{
			file << '\n';
		}
	}

	file.close();
}

void Logger::AddLog(Log log)
{
	m_logs.push_back(log);
}
