
#include "stdafx.h"
#include "log.h"

namespace common { namespace log {

	void Log_char( const char *label, const char *msg );
	void ErrorLog_char( const char *label, const char *msg );
	void Output_char( const char *label, const char *msg );
}}

using namespace common;
using namespace log;
using namespace std;


/**
 @brief 
 */
void log::Log_char( const char *label, const char *msg )
{
	ofstream fs("Log.log", ios_base::app);
	if (!fs.is_open()) return;
	fs << label << "   [" << common::GetTimeString() << "]  " << msg << endl;
}


/**
 @brief 
 */
void log::ErrorLog_char( const char *label, const char *msg )
{
	ofstream fs("ErrLog.log", ios_base::app);
	if (!fs.is_open()) return;
	fs << label << "   [" << common::GetTimeString() << "]  " << msg << endl;
}


/**
 @brief 
 */
void log::Output_char( const char *label, const char *msg )
{
	string str = string(label) + " " + msg;
	::OutputDebugStringA( str.c_str() );
}



//------------------------------------------------------------------------
// Log.log ���Ͽ� �α׸� �����.
// ��¥/�ð� + �޼���
//------------------------------------------------------------------------
void log::Log(const std::string &str)
{
	ofstream fs("Log.log", ios_base::out);
	if (!fs.is_open()) return;

	fs << "[" << common::GetTimeString() << "]	" << str << endl;
}


//------------------------------------------------------------------------
// �������� 
//------------------------------------------------------------------------
void log::Log( const char* fmt, ...)
{
	char textString[ 256] = {'\0'};
	va_list args;
	va_start ( args, fmt );
	vsnprintf_s( textString, sizeof(textString), _TRUNCATE, fmt, args );
	va_end ( args );

	ofstream fs("Log.log", ios_base::out);
	if (!fs.is_open()) return;
	fs << "[" << common::GetTimeString() << "]	" << textString << endl;
}


//------------------------------------------------------------------------
// ���� �޼���â ���
//------------------------------------------------------------------------
void log::ErrorMsg(const std::string &str)
{
	::MessageBoxA(NULL, str.c_str(), "Error", MB_OK );
}


//------------------------------------------------------------------------
// ErrorLog.log ���Ͽ� �α׸� �����.
// ��¥/�ð� + �����޼���
//------------------------------------------------------------------------
void log::ErrorLog(const std::string &str)
{
	std::ofstream fs("ErrorLog.log", std::ios_base::out);
	if (!fs.is_open()) return;

	fs << "[" << common::GetTimeString() << "]	" << str << std::endl;
}


//------------------------------------------------------------------------
// �޼���â�� ����Ѵ�.
//------------------------------------------------------------------------
void log::Msg(const std::string &str)
{
	::MessageBoxA(NULL, str.c_str(), "Msg", MB_OK );
}


/**
 @brief print log
 */
void log::Log( LOG_LEVEL level, const std::string &str )
{
	switch (level)
	{
	case LOG_FILE: Log_char("", str.c_str()); break;
	case LOG_OUTPUTWINDOW: Output_char("", str.c_str()); break;
	case LOG_FILE_N_OUTPUTWINDOW: 
		Log_char("", str.c_str());
		Output_char("", str.c_str());
		break;
	}
}


/**
 @brief print log
 */
void log::Log( LOG_LEVEL level, const char* fmt, ...)
{
	char textString[ 256] = {'\0'};
	va_list args;
	va_start ( args, fmt );
	vsnprintf_s( textString, sizeof(textString), _TRUNCATE, fmt, args );
	va_end ( args );

	switch (level)
	{
	case LOG_FILE:  Log_char("", textString); break;
	case LOG_OUTPUTWINDOW: Output_char("", textString); break;
	case LOG_FILE_N_OUTPUTWINDOW:
		Log_char("", textString);
		Output_char("", textString);
		break;
	}
}


/**
 @brief print error
 */
void log::Error( ERROR_LEVEL level, const std::string &str )
{
	switch (level)
	{
	case ERROR_CRITICAL: 
		ErrorLog_char( "Critical", str.c_str() );
		Output_char( "Critical", str.c_str() );
		break;

	case ERROR_PROBLEM:
		ErrorLog_char( "Problem", str.c_str() );
		Output_char( "Problem", str.c_str() );
		break;

	case ERROR_WARNING:
		Output_char( "Warning", str.c_str() );
		break;
	}
}


/**
 @brief print error
 */
void log::Error( ERROR_LEVEL level, const char* fmt, ... )
{
	char textString[ 256] = {'\0'};
	va_list args;
	va_start ( args, fmt );
	vsnprintf_s( textString, sizeof(textString), _TRUNCATE, fmt, args );
	va_end ( args );

	switch (level)
	{
	case ERROR_CRITICAL: 
		ErrorLog_char( "Critical",textString );
		Output_char( "Critical",textString );
		break;

	case ERROR_PROBLEM:
		ErrorLog_char( "Problem", textString );
		Output_char( "Problem",textString );
		break;

	case ERROR_WARNING:
		Output_char( "Warning", textString);
		break;
	}
}

