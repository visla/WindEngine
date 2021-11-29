/*
    WePhysicModelManager.cpp

	Autor: Nebojsa Vislavski
	Date: 26.4.2006.
	Version: 1.0

*/

#include <WePhysicModelManager.h>
#include <WePackage.h>
//#include <NxuStreamFactory.h>
//#include <NxuStream.h>
#include <WePhysics.h>
//#include <NxuPhysicsImport.h>
/*
WePhysicModelManager* WePhysicModelManager::_instance = NULL;

WePhysicModelManager* WePhysicModelManager::Instance()
{
	if (!_instance)	_instance = new WePhysicModelManager;
	return _instance;
}


WePhysicModelManager::WePhysicModelManager()
{
}



WePhysicModel* WePhysicModelManager::Load(StringParam HoleName, bool FromPackage)
{
	
	for (Models.First(); !Models.End(); Models.Next())
	{
		if (Models.GetItem()->Name == HoleName)
		{
			return Models.GetItem();
		}
	}

	if (!FromPackage)
	{		
		NxuInputStream *is;
		is = NxuStreamFactory::getInputStream(NxuStreamFactory::BINARY, HoleName);
		NxuPhysicsImport *pi = new NxuPhysicsImport(WePhysics::Instance()->PhysicsSDK); 
		pi->reset();
		NxuPhysicsCollection *collection;

		collection = pi->getCollection();
		collection->read(*is);
		WePhysicModel *model;
		model = new WePhysicModel;				
		model->Collection = *collection;
		model->ModelNames = new String[model->GetActorsCount()];
		String s;
		Models.PushBack(model);
		return model;
	} else
	{
		WePhysicModel *model;
		model = new WePhysicModel;
		if (WePackage::Instance()->BeginReading(HoleName, 10200))
		{
			model->Deserialize(WePackage::Instance()->DataStream);    
			WePackage::Instance()->EndReading();
			Models.PushBack(model);
			return model;
		}	
		delete model;
		return NULL;
	}
}

void WePhysicModelManager::Free(WePhysicModel *Model)
{
	for (Models.First(); !Models.End(); Models.Next())
	{
		if (Models.GetItem() == Model)
		{
			SAFE_DELETE(Model);			
			Models.Delete();
			return;
		}
	}
}

void WePhysicModelManager::FreeAll()
{

}

*/