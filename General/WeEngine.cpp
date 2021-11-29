/*
		File: WeEngine.cpp
		Author: Tomislav Kukic
		Date: 12.09.2005.

		Version: 1.0
*/



#include <Windows.h>
#include <CString.h>
#include <WeInput.h>
#include <WeRenderDevice.h>
#include <WeTimer.h>
#include <WeEngine.h>
#include <WeProfiler.h>
#include <WeMaterialManager.h>
#include <WeTextureManager.H>
#include <weShaderManager.h>
#include <WeMeshManager.h>
#include <WeModelManager.h>
#include <WeTextureManager.h>



WeEngine* WeEngine::_instance = NULL;

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeEngine::WeEngine(void)
{
	AudioDevice = NULL;
	RenderDevice = NULL;
	Timer = NULL;
	Input = NULL;
	Physics = NULL;
	Paused = false;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeEngine::~WeEngine(void)
{
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeEngine* WeEngine::Instance()
{
	if(_instance != NULL) return _instance;
	else{
		_instance = new WeEngine();
		return _instance;
	}
}



//LRESULT HandleMessages(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//{
//	LRESULT tmp;
//	return tmp;
//}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeEngine::ValidateSerial(String Serial)
{
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeEngine::Update(WeCamera *Cam)
{
	// reset counters
	WeProfiler::Instance()->Reset();
	WeMaterialManager::Instance()->Tick();

	Timer->Update();
	Input->Update();
	Server->Tick();
	Client->Tick();

	if (!Paused)
		Physics->BeginUpdate();
	RenderDevice->Update(Cam);	
	AudioDevice->Update();
	if (!Paused)
		Physics->EndUpdate();

	WeTextureManager::Instance()->Tick();
	if (Desc.AutomaticRendering)
		RenderDevice->DoRender();
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeEngine::Stop()
{
	WeTextureManager::Instance()->FreeAll();
	WeMeshManager::Instance()->FreeAll();
	WeShaderManager::Instance()->FreeAll();
	WeMaterialManager::Instance()->FreeAll();
	WeModelManager::Instance()->FreeAll();

	if (Physics) Physics->ShutDown();
	if (AudioDevice) AudioDevice->ShutDown();
	if (RenderDevice) RenderDevice->ShutDown();
	if (Input) Input->ShutDown();
	if (Server) Server->ShutDown();
	if (Client) Client->ShutDown();
	if (Timer) Timer->Stop();
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeEngine::Start(WeEngineDesc &desc, HINSTANCE hInst, HWND hWnd, HWND hTopWindow, 
		               const char* AppName, String CmdLine)
{
    
	Desc = desc;

	this->hWnd = hWnd;
	this->hWndTop = hTopWindow;


	// Start audio device
	AudioDevice = WeAudioDevice::Instance();
	AudioDevice->Initialize(hWnd);

	// Start input processing
	Input = WeInput::Instance();
	if(Input->Initialize(hInst, hWnd) == false) return false;

	// Start Timer
	Timer = WeTimer::Instance();
	Timer->Start(true);

	// Initialize Server
	Server = WeServer::Instance();
	if (!Server->Initialize()) return false;

	// Initialize Client
	Client = WeClient::Instance();
	if (!Client->Initialize()) return false;

	Physics = WePhysics::Instance();
	if (!Physics->Initialize()) return false;

	// Start Render device
	RenderDevice = WeRenderDevice::Instance();
	if(RenderDevice->Initialize(hInst, hWnd, CmdLine, desc.ResWidth, desc.ResHeight, desc.FullScreen, desc.DefaultShaderName) == false) return false;


	return true;
}

