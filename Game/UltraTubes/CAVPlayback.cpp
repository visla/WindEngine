/*
    Filename:	CAVPlayback.cpp
	Author:		Goran Bogdanovic
*/


#include <CAVPlayback.h>
#include <CUTWindow.h>

CAVPlayback *CAVPlayback::Instance = new CAVPlayback();


CAVPlayback::CAVPlayback()
{
	Initialized = false;
	Playing = false;

	CoInitialize(NULL);

	GraphBuilder = NULL;
	MediaControl = NULL;
	MediaEvent = NULL;
	MediaEventEx = NULL;
	VideoWindow = NULL;
	BasicVideo = NULL;
}

CAVPlayback::~CAVPlayback()
{
	Shutdown();
	CoUninitialize();
}


bool CAVPlayback::Initialize(String Filename)
{
	if (Initialized) return true;


	if (!GraphBuilder)
		if (!SUCCEEDED(
			CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&GraphBuilder)
			)) return false;
	if (!MediaControl)
		if (!SUCCEEDED(
			GraphBuilder->QueryInterface(IID_IMediaControl, (void**)&MediaControl)
			)) return false;
	if (!MediaEvent)
		if (!SUCCEEDED(
			GraphBuilder->QueryInterface(IID_IMediaEvent, (void**)&MediaEvent)
			)) return false;
	if (!MediaEventEx)
		if (!SUCCEEDED(
			GraphBuilder->QueryInterface(IID_IMediaEventEx, (void**)&MediaEventEx)
			)) return false;
	if (!SUCCEEDED(
		MediaEventEx->SetNotifyWindow((OAHWND)CUTWindow::Instance->GetHWND(), WM_GRAPHNOTIFY, 0)
		)) return false;

	if (!VideoWindow)
		if (!SUCCEEDED(
			GraphBuilder->QueryInterface(IID_IVideoWindow, (void**)&VideoWindow)
		)) return false;

	if (!BasicVideo)
		if (!SUCCEEDED(
			GraphBuilder->QueryInterface(IID_IBasicVideo, (void**)&BasicVideo)
		)) return false;

	Initialized = true;

	return Initialized;
}

bool CAVPlayback::Play(String Filename)
{
	Playing = false;

	if (!Initialize(Filename)) return false;

	int Len = MultiByteToWideChar(CP_UTF8, 0, Filename, -1, NULL, 0);
	LPWSTR fname = new wchar_t[Len + 1];
	MultiByteToWideChar(CP_UTF8, 0, Filename, -1, fname, Len);

    if (!SUCCEEDED(
		GraphBuilder->RenderFile(fname, NULL)
		)) return false;

	HRESULT hr = S_OK;
	hr |= VideoWindow->put_Owner((OAHWND)CUTWindow::Instance->GetHWND());
	hr |= VideoWindow->put_MessageDrain((OAHWND)CUTWindow::Instance->GetHWND());
	UpdateWindow(CUTWindow::Instance->GetHWND());
	hr |= VideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	RECT rect;
	GetClientRect(CUTWindow::Instance->GetHWND(), &rect);
	//long w, h;
	//BasicVideo->GetVideoSize(&w, &h);
	//int dh = (int)((float)(h * rect.right) / (float)w);
	//int t = (rect.top + ((rect.bottom - dh) / 2));
	//int b = (rect.bottom - (rect.bottom - dh));

	//hr |= VideoWindow->SetWindowPosition(rect.left, t, rect.right, b);
	hr |= VideoWindow->SetWindowPosition(rect.left, rect.top, rect.right, rect.bottom);
	hr |= VideoWindow->SetWindowForeground(-1);
	//hr |= VideoWindow->put_FullScreenMode(OATRUE);

	if (!SUCCEEDED(hr)) return false;

	UpdateWindow(CUTWindow::Instance->GetHWND());
	if (!SUCCEEDED(
		MediaControl->Run()
		)) return false;
	UpdateWindow(CUTWindow::Instance->GetHWND());

	Playing = true;

	return true;
}

void CAVPlayback::Stop()
{
	Shutdown();
}


void CAVPlayback::HandleGraphEvent()
{
	if( (!Initialized) || (!MediaEvent) || (!MediaEventEx) ) return;

	long evCode;
	LONG_PTR param1, param2;
	while (SUCCEEDED(MediaEvent->GetEvent(&evCode, &param1, &param2, 0)))
	{
		MediaEvent->FreeEventParams(evCode, param1, param2);
		switch (evCode)
		{
		case EC_COMPLETE:
		case EC_USERABORT:
		case EC_ERRORABORT:
			Stop();
			return;
        }
	}
}

void CAVPlayback::Shutdown()
{
	if (MediaControl)
	{
		MediaControl->Stop();
		SAFE_RELEASE(MediaControl);
	}
	if (MediaEvent)
	{
		SAFE_RELEASE(MediaEvent);
	}
	if (GraphBuilder)
	{
		SAFE_RELEASE(GraphBuilder);
	}
	if (MediaEventEx)
	{
		MediaEventEx->SetNotifyWindow(NULL, 0, 0);
		SAFE_RELEASE(MediaEventEx);
	}
	if (VideoWindow)
	{
		//VideoWindow->put_Owner(NULL);
		SAFE_RELEASE(VideoWindow);
	}
	if (BasicVideo)
	{
		SAFE_RELEASE(BasicVideo);
	}

	Initialized = false;
	Playing = false;
}
