/*
		File: WeMeshContainer.h
		Author: Tomislav Kukic
		Date: 11.07.2006
		Version: 1.0
*/



#ifndef _WEMESHCONTAINER_H_
#define _WEMESHCONTAINER_H_


#include <d3dx9.h>
#include <WeMatrix.h>
#include <WeMesh.h>
#include <WindEngine.h>

class WeMesh;
class WeFileStream;


class WINDENGINE_API WeMeshContainer : public D3DXMESHCONTAINER
{
public:
	WeMeshContainer();
	~WeMeshContainer();
	
	WeMesh*				Mesh;
	WeMatrix*			BoneOffsets;
	WeMatrix**			FrameCombinedMatrices;

	void Save(WeFileStream* FS);
	void Load(WeFileStream* FS);
};





#endif

