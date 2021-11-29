/*
		File: WeAnimTexture.h
		Author: Tomislav Kukic
		Date: 31.05.2006
		Version: 1.0
*/



#ifndef _WEANIMTEXTURE_H_
#define _WEANIMTEXTURE_H_


#include <d3dx9.h>
#include <SmartList.h>
#include <CString.h>
#include <WeFileStream.h>
#include <WindEngine.h>


class WINDENGINE_API WeAnimTexture
{
protected:
	float		AnimationTime;
	float		AnimationTimer;
	float		FramesPerSecond;
	float		ElapsedTime;
	int			FrameCounter;
	bool		Looped;
	bool		IsPlaying;
	bool		IsPaused;
	int			Width;
	int			Height;


	LPDIRECT3DTEXTURE9 CurrentTexture;
	CSmartList<LPDIRECT3DTEXTURE9> Textures;

	LPDIRECT3DTEXTURE9 LoadTextureFromFile(StringParam _Name);

public:
	WeAnimTexture();
	~WeAnimTexture();
	
	String Name;

	void				SetAnimationTime(float);
	float				GetAnimationTime();

	void				SetFramesPerSecond(float);
	float				GetFramesPerSecon();

	void				SetElapsedTime(float);
	int					GetCurrentFrameNum();

	void				Play();
	void				Stop();
	void				Pause();
	void				StepUp();
	void				StepDown();

	void				Tick();
	void				ClearAll();

	void				SetLooped(bool _Loop);
	bool				GetLooped();
	bool				GetIsPlayin();
	bool				GetIsPaused();

	int					GetWidth(){ return Width; }
	int					GetHeight(){ return Height; }

	void				InsertTextureAtStart(StringParam _Name);
	void				InsertTextureAtEnd(StringParam _Name);

	int					GetTexturesCount() { return Textures.GetItemsCount(); }
	LPDIRECT3DTEXTURE9	GetCurrentTexture();
	LPDIRECT3DTEXTURE9	GetTextureAtTime(float);

	WeColor				TextureColor;

	void				Serialize(WeFileStream &FS);
	void				DeSerialize(WeFileStream &FS);
};





#endif
