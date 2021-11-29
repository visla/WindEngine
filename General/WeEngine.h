/*
		File: WeEngine.h
		Author: Tomislav Kukic
		Date: 12.09.2005.

		Version: 1.0
*/


#ifndef _WEENGINE
#define _WEENGINE


#include <Windows.h>
#include <CString.h>
#include <WindEngine.h>
#include <WeServer.h>
#include <WeClient.h>
#include <WeAudioDevice.h>
#include <WePhysics.h>
#include <WeEngineDesc.h>
#include <WeRenderDevice.h>
#include <WeInput.h>
#include <WeMaterialManager.h>
#include <WeShaderManager.h>
#include <WeModelManager.h>


class WeAudioDevice;
class WeRenderDevice;
class WeInput;
class WeTimer;
class WeCamera;
class WeConfig;
class WeConfigFile;





class WINDENGINE_API WeEngine
{
protected:
	WeEngine();
	virtual ~WeEngine(void);

	bool			Dedicated;
	bool			Paused;
	
	WeAudioDevice*	AudioDevice;
	WeRenderDevice* RenderDevice;
	WeInput*		Input;
	WeTimer*		Timer;
	WeServer*		Server;
	WeClient*		Client;
	WePhysics*		Physics;
	
	HWND			hWnd;
	HWND			hWndTop;



private:
	static WeEngine* _instance;

public:
	WeEngineDesc	Desc;


	static WeEngine* Instance();
	
	virtual void ValidateSerial(String Serial);
	virtual void Update(WeCamera *Cam);
	virtual void Stop();
	void Pause() { Paused = true; };
	void Unpause() { Paused = false; };
	virtual bool Start(WeEngineDesc &desc, HINSTANCE hInst, HWND hWnd, HWND hTopWindow, 
		               const char* AppName, String CmdLine);
	
	//virtual LRESULT HandleMessages(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	
	WeAudioDevice*	GetAudioDevice(){ return AudioDevice; }
	WeRenderDevice* GetRenderDevice(){ return RenderDevice; }
	WeInput*		GetInput(){ return Input; }
	WeTimer*		GetTimer(){ return Timer; }
	HWND			GetHwnd(){ return hWnd; }
	HWND			GetHwndTop(){ return hWndTop; }
};


#endif