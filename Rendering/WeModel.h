/*
		File: WeModel.h
		Author: Tomislav Kukic
		Date: 10.07.2006
		Version: 1.0
*/




#ifndef _WEMODEL_H_
#define _WEMODEL_H_



#include <WeStaticModel.h>
#include <WeModelFrame.h>
#include <WeMeshContainer.h>
#include <d3dx9.h>
#include <WeVector.h>
#include <WeMatrix.h>
#include <WindEngine.h>



class WINDENGINE_API WeModel: public WeStaticModel
{
private:
	int NumAnimationSets;
	int CurrentAnimationSet;
	int Version;

	Strings AnimationNames;

	WeMatrix* BoneMatrices;
	int maxBones;
	WeMeshContainer* FirstContainer;

	bool UseNormalMap;

	void DrawAllFrames(WeWorld* World, WeCamera* Cam, WeActor* Owner, WeModelFrame* Frame);
	WeModelFrame* CloneAllFrames(WeModelFrame* frame, bool DeepClone);
	void SetMaterial(StringParam MaterialName, int MatPreset, WeModelFrame* frame);
	void SetupBoneMatrices(WeModelFrame* frame, WeMatrix* ParentMatrix);
	void FindFirstContainer(WeModelFrame* frame);
	void ComputeBoundingBox(WeActor *Owner, WeBox *outBox, WeModelFrame* Frame);

public:
	WeModel(void);
	~WeModel(void);

	float AnimationSpeed;

	WeModelFrame* RootFrame;

	LPD3DXANIMATIONCONTROLLER   AnimController;

	void SetAnimation(int Index);
	int GetAnimation(){ return CurrentAnimationSet; }

	bool Load(StringParam name, bool FromPackage, bool ForceReload);  // returns false if not loaded
	virtual void LoadMaterials(WeActor* Owner);
	virtual void LoadMaterials(int Preset);
	bool Save(StringParam HoleName, bool Replace = false);

	void Draw(WeWorld* World, WeCamera* Cam, WeActor* Owner = NULL);
	void UpdateTransformations(WeMatrix &TM, WeMatrix &PrevTM);
	void ComputeBoundingBox(WeActor *Owner);
	void Clone(WeModel* Original, bool DeepCopy);
	void SetMaterial(StringParam MaterialName, int MaterialPreset);
	void SetUseNormalMaps(WeModelFrame* frame, bool Use);

	String GetCurrentMaterialName(WeActor *Owner){ return (String)"Unknown material!!!"; }
	Strings* GetAnimationNames(); 
};



#endif
