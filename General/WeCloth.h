/*
     WeCloth.h

	 Autor: Nebojsa Vislavski
	 Date: 30.11.2005.
	 Version: 1.0
	 Platform: Linux/Win32

*/


#ifndef _WECLOTH_H
#define _WECLOTH_H

#include <WindEngine.h>
#include <WeVector.h>
#include <WePhysicHandle.h>
#include <WeActor.h>
#include <WeMesh.h>


class WINDENGINE_API WeClothFixedVertex
{
friend class WeCloth;
    int GridX;
	int GridY;
};


class WINDENGINE_API WeCloth: public WeActor
{
friend class WePhysics;
protected:
	int				LocalVersion;

	WeMesh				*ClothMesh;
	WeNormalMappedMeshVertex  *Vertices;

	DWORD			*Indices;
	DWORD			*ParentIndices;

	DWORD			NumOriginalVertices;
	DWORD			NumVertices;
	DWORD			NumIndices;
	DWORD			NumParentIndices;

	bool			Simulating;


public:
					WeCloth();
					~WeCloth();

	String			LowModelName;
	String			MediumModelName;
	String			HighModelName;


	float			BendingStiffness;
	float			StretchingStiffness;
	float			DampingCoef;
	float			Friction;
	float			Pressure;
	float			TearFactor;
	float			Thickness;
	float			Density;
	int				Group;
	bool			AutoAttachToColliding;
	bool			Tearable;
	bool			Gravity;
	float			CollisionResponseCoef;
	float			AttachmentResponseCoef;
	bool			OnlyHardware;   // this is displayable only when physx card is present



	void			Initialize();
	void			Tick();

	void			Clone(WeActor *Destination, bool DeepCopy = false);
	void			OnRender(WeCamera *Cam);

	void			Serialize(WeFileStream &FS);
	void			Deserialize(WeFileStream &FS);


	//void			PreRender(WeCamera* Cam);
	//void			PostRender(WeCamera* Cam);

};

#endif
