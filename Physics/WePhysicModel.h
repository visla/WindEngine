/*
    WePhysicModel.h

	Autor: Nebojsa Vislavski
	Date: 5.12.2005.
	Version: 1.0
	Platform: Win32/Linux


	This is physic model that contains all needed data for physic simulation
   

*/

#ifndef _WEPHYSICMODEL_H
#define _WEPHYSICMODEL_H


#include <NxPhysics.h>
#include <CString.h>
#include <SmartList.h>
#include <WindEngine.h>
#include <WeFileStream.h>

class WeActor;
class WePhysicHandle;

enum PHYSICMODEL_TYPE
{
	PHYSICMODEL_NONE,
	PHYSICMODEL_STATIC_BOX,
	PHYSICMODEL_STATIC_SPHERE,
	PHYSICMODEL_STATIC_CYLINDER,
	PHYSICMODEL_STATIC_SHAPE,

	PHYSICMODEL_DYNAMIC_BOX,
	PHYSICMODEL_DYNAMIC_SPHERE,
	PHYSICMODEL_DYNAMIC_CYLINDER,
	PHYSICMODEL_DYNAMIC_SHAPE
};



class WINDENGINE_API WePhysicModel
{
friend class WePhysics;
protected:
			WeActor *Owner;
			int		Version;

public:
			WePhysicModel();
			virtual ~WePhysicModel();
               			   
			void					 Serialize(WeFileStream &FS);
			void					 Deserialize(WeFileStream &FS);

			PHYSICMODEL_TYPE		 Type;

			WeVector				 BoxDimensions;
			float					 SphereRadius;
			float					 CapsuleRadius;
			float					 CapsuleHeight;
			WORD					 ShapeGroup;
			WORD					 TriggerType;
			float					 Mass;
			bool					 SleepOnStart;

			WeActor*				 GetOwner() { return Owner; }
			WePhysicHandle*			 MakePhysicHandle(WeActor *Owner);


};


#endif