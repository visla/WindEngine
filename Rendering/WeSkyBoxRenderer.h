/*
    WeSkyBoxRenderer.h

	Autor: Nebojsa Vislavski
	Date: 2.3.2006.
	Version: 1.0

	this is renderer used for sky boxes. 
*/

#ifndef _WESKYBOXRENDERER_H
#define _WESKYBOXRENDERER_H


#include <WindEngine.h>
#include <WeBaseRenderer.h>
#include <d3dx9.h>
#include <d3d9.h>

class WeSkyBox;
class WeCamera;
class WeShader;

class WINDENGINE_API WeSkyBoxRenderer: public WeBaseRenderer
{
private:
	static WeSkyBoxRenderer *_instance;	

protected:
	LPDIRECT3DVERTEXBUFFER9 lpSkyBoxVB;
	bool					bInitialized;



public:
	static WeSkyBoxRenderer *Instance();
	WeSkyBoxRenderer();


	void Render(WeSkyBox *skyBox, WeCamera *Cam, WeShader *UseEffect = NULL); // use effects indicated if it should use effects from materials

    
	void OnLostDevice();
	void OnResetDevice();

	bool Initialize();
	void ShutDown();

};


#endif