/*
    WeControllerHandle.cpp

	Autor: Nebojsa Vislavski
	Date: 11.10.2005.
	Version: 1.0

	This is controller handle

*/

#include <WeControllerHandle.h>
#include <WeActor.h>


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
WeControllerHandle::WeControllerHandle(WeActor *parent)
{
	Parent = parent;
	ControllerReport.Owner = this;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeControllerHandle::GetLocation(WeVector &outLocation)
{
	outLocation.x = (float)nxController->getFilteredPosition().x;
	outLocation.y = (float)nxController->getFilteredPosition().y;
	outLocation.z = (float)nxController->getFilteredPosition().z;		
}


void WeControllerHandle::SetLocation(const WeVector &Location)
{
	//NxVec3 vec(Location.x, Location.y, Location.z);
	
	//nxController->setPosition(vec);
}





/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetCollision
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
NxControllerAction WePhysicControllerReport::onShapeHit(const NxControllerShapeHit &hit)
{
	WeVector Dir;
	Dir.x = hit.dir.x;
	Dir.y = hit.dir.y;
	Dir.z = hit.dir.z;

	if (hit.shape->getActor().userData)
	{
		Owner->OnContactNotify((WePhysicHandle *)(hit.shape->getActor().userData), Dir);
	}

	return NX_ACTION_NONE;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  SetCollision
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
NxControllerAction WePhysicControllerReport::onControllerHit(const NxControllersHit &hit)
{
	return NX_ACTION_NONE;
}