/*

    WeTimer.h

	Author: Vislavski Nebojsa
	Date: 16.8.2005
	Version: 1.3

	History: 
	1.1 - added GetFPS and also fixed some timing problems
	1.2 - added High Resolution timer. 
	      GetHighDelta returns number of microseconds between Updates
		  GetHighFPS is mush more precise for calculating FPS
    1.3 - Changed from CTimer to WeTimer by Toma (19.09.2005.)

 */



#ifndef _WETIMER
#define _WETIMER


#include <windows.h>
#include <WindEngine.h>



class WINDENGINE_API WeTimer
{
private:
	static WeTimer *_instance;

protected:
	DWORD TimeStarted;
	DWORD TimeCurrent;
	

	bool bStarted;
	bool bUseHighResTimer;
	DWORD dwElapsedTime;
	DWORD dwDeltaTime;
	float fElapsedTime;
	float fDeltaTime;
	 
	float FPS;	
	DWORD OneSecondCounter;
	DWORD UpdateCounter;

	__int64 HighTimeStart;
	double HighDeltaTime;
	double HighFPS;
	__int64 HighTimeTmp;

	float FPSTime;
	int FPSCounter;

	LARGE_INTEGER ZeroResCounter;

	WeTimer() {	bStarted = false; QueryPerformanceCounter(&ZeroResCounter); }	
	
public:
	__int64 HighTimeCurrent;
	__int64 HighResFrequency;

	static WeTimer *Instance();

	bool GetStarted() { 	return bStarted; }

	void Start(bool UseHighResTimer = false);
	void Stop();

	void Update();

	int		GetSystemTime(); // this is mainly called by networking components

	float GetAbsoluteTime();


	DWORD  GetElapsedTimeDW() { return dwElapsedTime; }
	DWORD  GetDeltaTimeDW() { return dwDeltaTime; }
	float  GetElapsedTimeF() { return fElapsedTime; }
	float  GetDeltaTimeF() { return fDeltaTime; }
	float  GetFPS() { return FPS; }
	double GetHighDelta() { return HighDeltaTime; }
	double GetHighFPS() { return HighFPS; }

};


#endif