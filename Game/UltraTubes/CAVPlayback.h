/*
    Filename:	CAVPlayback.h
	Author:		Goran Bogdanovic
*/


#include <CString.h>
#include <DShow.h>

#ifndef _CAVPlayback_h_
#define _CAVPlayback_h_


#define WM_GRAPHNOTIFY	WM_APP + 1


class CAVPlayback
{
public:
	static CAVPlayback		*Instance;

	CAVPlayback();
	~CAVPlayback();

	bool					Play(String Filename);
	void					Stop();
	void					HandleGraphEvent();
	void					Shutdown();

	bool					Playing;


protected:
	bool					Initialize(String Filename);

	bool					Initialized;

	IGraphBuilder			*GraphBuilder;
	IMediaControl			*MediaControl;
	IMediaEvent				*MediaEvent;
	IMediaEventEx			*MediaEventEx;
	IVideoWindow			*VideoWindow;
	IBasicVideo				*BasicVideo;
};


#endif