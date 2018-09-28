#pragma once
#include <cstdio>
#include <cstdarg>
#include <string>

static class Logger
{
public:
	Logger(const char *logName = "Log");
	~Logger();

	void writeLog(const char *);

private:
	FILE * log;
	std::string fileName;
};

