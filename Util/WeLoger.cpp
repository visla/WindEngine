/*

        WeLoger.cpp

		Author: Nebojsa Vislavski
		Date: 19.4.2005
		Version: 1.11


		This is logging system
*/




#include <WeLoger.h>
#include <WeTimer.h>
#include <cstring.h>
#include <stdio.h>
#include <time.h>


WeLoger::WeLoger(StringParam FileName)
{
	FileHandle = NULL;				
	this->FileName = FileName;
	Started = false;
}

WeLoger::WeLoger()
{
	FileHandle = NULL;				
	FileName = "engine.log";
	Started = false;
}


WeLoger::~WeLoger()
{
	LastMessage = "";
	Stop();
}



bool WeLoger::Start(int Level)
{ 
   char timebuf[30];
   char datebuf[30];

    if (LogLevel != Level) Stop(); 
	
	if (Started) return true;	
  
	LogLevel = Level;	

	FileHandle = fopen(FileName, "wt+");

	_strtime(timebuf);
	_strdate(datebuf);
	fprintf(FileHandle, "Log: Log file opened: %s %s (Log level is: %d) \n", datebuf,timebuf, LogLevel);	

	LastMessage = "";
	Started = true;
	return true;
}



void WeLoger::Stop()
{
   char timebuf[30];
   char datebuf[30];

  if (!Started) return;
	

  _strtime(timebuf);
  _strdate(datebuf);
  fprintf(FileHandle, "Log: Log file closed: %s %s\n", datebuf,timebuf, LogLevel);	
  fclose(FileHandle);
  LastMessage = "";
  Started = false;   
}




void WeLoger::LogString(int MsgLevel, char *str, ...)
{
	char tmp[500];

    if (!Started) return;    
	if (MsgLevel > LogLevel) return;

	va_list ap;
	va_start(ap, str);
	vsprintf(tmp, str, ap);
	va_end(ap);



	fprintf(FileHandle, "(%d) %s\n", WeTimer::Instance()->GetSystemTime(), tmp);
	fflush(FileHandle);
	LastMessage = tmp;
}


void WeLoger::DebugLog(char *fmt, ...)
{
#ifdef _DEBUG
	static bool inited = false;

	char tmp[500];

	va_list ap;
	va_start(ap, fmt);
	vsprintf(tmp, fmt, ap);
	va_end(ap);

	FILE *f;

	if (!inited)
	{
		f = fopen("debug.log", "wt");
		inited = true;
	}
	else
	{
		f = fopen("debug.log", "at");
	}

	fputs(tmp, f);
	fflush(f);
	fclose(f);
#endif
}
