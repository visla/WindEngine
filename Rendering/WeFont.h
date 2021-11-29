/*
       File: WeFont.h
	   Author: Tomislav Kukic
	   Date: 10.10.2005.
	   Version: 1.0
	   Platform: Windows
*/



#ifndef _WEFONT
#define _WEFONT

#include <WindEngine.h>
#include <CString.h>



class WINDENGINE_API WeFont
{
protected:
	ID3DXFont		*Font;
	D3DXFONT_DESC	FontDesc;
	bool			Initialized;
	String			SavedName;

public:
	WeFont(): Initialized(false) { }	
	~WeFont(){ ShutDown();}

	void	Initialize(StringParam FontName, int FontSize);
	void	ShutDown(); 

	void	Draw2DText(StringParam Text, int x, int y, int w, int h, D3DCOLOR FontColor);

	void	OnLostDevice();
	void	OnResetDevice();

	int		GetHeight() { return FontDesc.Height; }
	UINT	GetWidth()	{ return FontDesc.Width; }
	UINT	GetWeight()	{ return FontDesc.Weight; }
	String	GetName()	{ return SavedName; }

};



#endif