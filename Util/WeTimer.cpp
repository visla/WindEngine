/*

    WeTimer.h

	Author: Vislavski Nebojsa
	Date: 18.6.2005
	Version: 1.3

	

 */



#include <WeTimer.h>
#include <windows.h>
#include <MMsystem.h>


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeTimer::Start(bool UseHighResTimer)
{
	if (!bStarted)
	{
		TimeStarted = timeGetTime(); //GetTickCount(); //timeGetTime();
		TimeCurrent = TimeStarted;

		dwElapsedTime = 0;
		dwDeltaTime = 0;
		fElapsedTime = 0.0f;
		fDeltaTime = 0.0f;
		FPS = 0.0f;	
		FPSTime = 0.0;
		FPSCounter = 0;
		HighFPS = 0.0f;
		bUseHighResTimer = UseHighResTimer;
		bStarted = true;
		
		if (bUseHighResTimer)
		{
			QueryPerformanceFrequency((LARGE_INTEGER *)&HighResFrequency);
			QueryPerformanceCounter((LARGE_INTEGER *)&HighTimeStart);
			HighTimeCurrent = HighTimeStart;
			HighFPS = 0;

		}
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeTimer::Stop()
{
	bStarted = false;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
int WeTimer::GetSystemTime()
{
	return timeGetTime();
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
WeTimer *WeTimer::_instance = NULL;

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
WeTimer *WeTimer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeTimer;
	return _instance;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeTimer::Update()
{
	if(bStarted)
	{
		

		dwDeltaTime = timeGetTime() - TimeCurrent;
		TimeCurrent = timeGetTime(); //GetTickCount();

		dwElapsedTime += dwDeltaTime;
		fDeltaTime = (float)dwDeltaTime/1000.0f;
		fElapsedTime += fDeltaTime;

		if (bUseHighResTimer) 
		{
			QueryPerformanceCounter((LARGE_INTEGER *)&HighTimeTmp);
			HighDeltaTime = (double)(HighTimeTmp - HighTimeCurrent) / (double)HighResFrequency * 1000000;
			QueryPerformanceCounter((LARGE_INTEGER *)&HighTimeCurrent);	
			HighFPS = 1000000.0 / HighDeltaTime;
		}

		//================ Changed by Toma ======================
		FPSTime += (float)dwDeltaTime;
		FPSCounter++;

		if(FPSTime >= 1000.0)
		{
			FPS = (float)FPSCounter;
			FPSCounter = 0;
			FPSTime = 0.0;
		}
		//========================================================
	}
}



float WeTimer::GetAbsoluteTime()
{
	LARGE_INTEGER HighResFrequency, HighResCounter;

	QueryPerformanceFrequency(&HighResFrequency);
	QueryPerformanceCounter(&HighResCounter);

	return (float)((double)(HighResCounter.QuadPart - ZeroResCounter.QuadPart) / HighResFrequency.QuadPart);
}
