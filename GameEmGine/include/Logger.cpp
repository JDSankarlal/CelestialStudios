#include "Logger.h"


Logger::Logger(const char *name)
{
	//creates the log file
	fopen_s(&log, (fileName = std::string("Logs/") + std::string(name) + ".log", fileName.c_str()), "w+");
	fclose(log);
}


Logger::~Logger()
{

}

void Logger::writeLog(const char *str)
{

	fopen_s(&log, fileName.c_str(), "a");
	fputs(str, log);
	fclose(log);
}