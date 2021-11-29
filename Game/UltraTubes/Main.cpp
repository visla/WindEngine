/*
    Main.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    

*/


#include <windows.h>
#include <exception>
#include <CUTApplication.h>


#define MUTEX_NAME	"EIPIX_APP_ULTRA_TUBES"


bool CheckAppInstance()
{
	CreateMutex(NULL, false, MUTEX_NAME);
	if ( ERROR_ALREADY_EXISTS == GetLastError() )
		return false;

	return true;
}


int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
 	if (!CheckAppInstance())
	{
		MessageBox(0, "Game is already running!", 0, 0);
	}
	else
	{
#ifdef _DEBUG
		CUTApplication::Instance->Run();
#else
		try
		{
			CUTApplication::Instance->Run();
		}
		catch (std::exception* e)
		{
			MessageBox(0, e->what(), "Exception", 0);
		}
#endif
	}

	return 0;
}
