/*
      WeSkyBox.h

	  Autor: Nebojsa Vislavski
	  Date: 3.3.2006.
	  Version: 1.0
	  Platform: Win32/Linux			:P "linux" lol! =)

	  this is sky box used in game. You can set different properties here

*/

#ifndef _WESKYBOX_H
#define _WESKYBOX_H


#include <WindEngine.h>
#include <WeActor.h>
#include <WeMaterial.h>


class WeMesh;


class WeSkyBox: public WeActor
{
friend class WeWorld;

protected:
	int			Version;

	WeSkyBox();  // no one can create sky box. If you need one. use World->GetSkyBox

public:

	WeMesh		*SkyBoxMesh;
	String		SkyBoxMeshName;

	void Initialize();
	void OnRender(WeCamera *Cam, WeShader *UseEffects = NULL);
	void Serialize(WeFileStream &FS);
	void Deserialize(WeFileStream &FS);
};


#endif