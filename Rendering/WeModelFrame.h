/*
		File: WeModelFrame.h
		Author: Tomislav Kukic
		Date: 10.07.2006
		Version: 1.0
*/





#ifndef _WEMODELFRAME_H_
#define _WEMODELFRAME_H_



#include <d3dx9.h>
#include <WeMatrix.h>
#include <WindEngine.h>
#include <CString.h>
#include <WeBox.h>

class WeMesh;
class WeMeshContainer;
class WeActor;
class WeWorld;
class WeCamera;
class WeFileStream;



class WINDENGINE_API WeModelFrame : public D3DXFRAME
{
public:
	WeModelFrame();
	~WeModelFrame();

	WeMatrix TM;
	WeMatrix PrevTM;
	WeBox BBox;

	void ComputeBoundingBox(WeActor *Owner, WeMatrix &TM, WeModelFrame *ParentFrame);
	void Draw(WeWorld* World, WeCamera* Cam, WeActor* Owner = NULL);
	void DrawMeshContainer(WeWorld *World, WeCamera *Cam, WeActor* Actor, WeMesh* Mesh);

	void Update(WeMatrix &pTM, WeMatrix &pPrevTM, WeModelFrame* ParentFrame);

	void LoadMaterials(int MaterialPreset);
	void LoadMaterials(WeActor* Owner);

	void Save(WeFileStream* FS, WeModelFrame* frame);
	void Load(WeFileStream* FS, WeModelFrame* frame);
	void Clone(WeModelFrame* Original, bool DeepCopy);
	void SetUseNormalMaps(bool Use);
};






#endif
