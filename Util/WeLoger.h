/*  
    WeLoger.h

	Author: Nebojsa Vislavski
	Date: 19.4.2005
	Version: 1.11

	History: 
	  1.1 - added new constructor
	  1.11 - added DisplayMessage method that displays last message




*/

#ifndef _WELOGER_H
#define _WELOGER_H


#include <cstring.h>
#include <WindEngine.h>


#define MAX_LOGLEVEL 10
#define LOG_ERROR 1
#define LOG_WARNING 2
#define LOG_DEBUG	3
#define LOG_NOTICE  3
#define LOG_INFO	3

class WINDENGINE_API WeLoger
{
protected:
    
	FILE *FileHandle;
	bool Started;
	String LastMessage;
	int LogLevel; // if 0 there is no logging	
	String FileName;

	// like printf command. if MsgLevel is higher than LogLevel than this message is not displayed
public:
	WeLoger();
    WeLoger(StringParam FileName);	
    virtual ~WeLoger();

	virtual bool Start(int Level = MAX_LOGLEVEL);
	virtual void Stop();

	virtual void LogString(int MsgLevel, char *str, ...);
	
	virtual String GetLastMessage() { return LastMessage; }        // Gets Last log message 	
    
	static void DebugLog(char *fmt, ...);
};


#endif