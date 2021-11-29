/*
     WePhysicModelManager.h

	 Autor: Nebojsa Vislavski
	 Date: 26.4.2006.
	 Version: 1.0
     
*/


#ifndef _WwPhysicModelManager_h
#define _WwPhysicModelManager_h

#include <SmartList.h>
#include <CString.h>
#include <WePhysicModel.h>
#include <WindEngine.h>

/*
class WINDENGINE_API WePhysicModelManager
{
private:
	static WePhysicModelManager *_instance;
protected:

	WePhysicModelManager(void);
	virtual ~WePhysicModelManager(void){}
	

public:
	CSmartList<WePhysicModel*> Models;

	static WePhysicModelManager *Instance();

	virtual void FreeAll();
	virtual void Free(WePhysicModel* Model);			
	
	WePhysicModel* Load(StringParam HoleName, bool FromPackage = false);	
};
*/

#endif
