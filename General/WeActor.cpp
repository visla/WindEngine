/*
      File: WeActor.cpp
	  Author: Nebojsa Vislavski
	  Date: 13.09.2005.
	  Version: 1.0
*/





#include <WeActor.h>
#include <WeStaticModel.h>
#include <WeTimer.h>
#include <WeWorld.h>
#include <WePhysicHandle.h>
#include <WeModelManager.h>
#include <WePhysics.h>
#include <WeMesh.h>
#include <WeFrustum.h>
#include <WeProfiler.h>
#include <WeCamera.h>
#include <WeRenderDevice.h>
#include <WeLoger.h>
#include <WeSound.h>
#include <WeAudioDevice.h>
#include <WeRenderTree.h>



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
//WeActor::WeActor(WeWorld* WorldToBeIn)
//{
	//Model = NULL;
	//Renderable = true;
	//IsNetworkActor = false;
	//IsPrefab = false;

	//PrevActor = this;
	//NextActor = this;
	//

	//LifeTime = 0;
	//ObjectAge = 0;
	//EngineTag = 0;		
	//OccludedTime = 0;
	//// Register in world
	//WorldToBeIn->AddActor(this);
	//World = WorldToBeIn;
	//PhysicModelName = "";
	//ModelName = "";
//}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
WeActor::WeActor()
{
	initialize();
}


void WeActor::initialize()
{
	Version = 15;
	InEditor = false;
	IsSelection = false;
	Renderable = true;
	Editor_Renderable = true;
	Model = NULL;
	ObjectAge = 0;
	LifeTime = 0;
	AutoDestroy = true;
	Model = NULL;
	World = NULL;
	Type = WETYPE_ACTOR;
	Serializable = true;
	MaterialPreset = 0;
	GhostObject = false;
	LastRenderedFrame = 0;
	CurrentZone = NULL;
	AutoUpdateTransformations = true;
	LightOccluder = true;
	MaxRenderDistance = 0.0f; // default is infinite

	IsNetworkActor = false;
	IsPrefab = false;
	IsLight = false;
	IsParticleSystem = false;
	IsCloth = false;
	IsParticleSystem = false;
	HasTicked = false;
	CastShadowVolume = false;
	ReceiveShadowVolume = true;
	ReceiveProjectors = true;	
	Sound = NULL;
	SoundDistance = 100.0f;
	PhysicModel.Type = PHYSICMODEL_NONE;

	PhysicHandle = NULL;
	EngineTag = 0;

	IsTrigger = false;
	Teleport = false;
	TeleportRelative = false;

	WantLight = true;
	PassFlares = false;

	Yaw = 0.0f;
	Pitch = 0.0f;
	Roll = 0.0f;
	Scale = WeVector(1,1,1);

	PrevActor = this;
	NextActor = this;

	Location = WeVector(0,0,0);
	Direction = WeVector(0,0,1);
	Velocity = WeVector(0,0,0);
	AngularVelocity = WeVector(0, 0, 0);

	OldLocation = Location;
	OldDirection = Direction;
	OldVelocity = Velocity;
	OldYaw = Yaw;
	OldPitch = Pitch;
	OldRoll = Roll;

	UseMaterialParams = false;
	MaterialParam1 = 0;
	MaterialParam2 = 0;
	MaterialParam3 = 0;
	MaterialParam4 = 0;
	MaterialColor = WeColor(1.0,1.0,1.0,1.0);

	PhysicModelName = " ";
	//ModelName = " ";

	D3DXMatrixIdentity(&TM);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
WeActor::~WeActor(void)
{
	
	if (Sound) 
	{
		Sound->Stop();
		SAFE_DELETE(Sound);
	}

	// unregister from world
	if (World)
		World->RemoveActor(this);

	// remove model 
	SAFE_DELETE( Model );

	// remove physic associations
	SAFE_DELETE( PhysicHandle );

}


void WeActor::ReInsertIntoWorld()
{  
	if (IsPrefab)
	{
		if (World)
		{
			World->RemoveActor(this);
			World->AddActor(this);
		}
	}
}

void WeActor::Initialize()
{
	if (PhysicModel.Type == PHYSICMODEL_STATIC_SHAPE)
	{
		if (World) World->RenderTree->PushPrefab(this);
		IsPrefab = true;
	}
	else 
	{
		if (World) World->RenderTree->PopPrefab(this);
		IsPrefab = false;
	}

	if (Model) SAFE_DELETE(Model);

	Model = WeModelManager::Instance()->Load(ModelName, false);
	if (Model)
	{		
		Model->ComputeBoundingBox(this);
		if (IsPrefab) Model->LoadMaterials(this);
	}

	SAFE_DELETE(PhysicHandle);
	PhysicHandle = PhysicModel.MakePhysicHandle(this);

	//if (IsPrefab)
	//{
		//LightMap.Initialize();
	//}

	if (GhostObject)
	{
		if (PhysicHandle)
		{
			PhysicHandle->SetCollision(false);
		}
	}

	InitializeSound();

	if (Model)
		Model->UpdateTransformations(TM, TM);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeActor::InitializeSound()
{
	SAFE_DELETE(Sound);
	if (SoundName.GetLen() > 0)
	{
		Sound = WeAudioDevice::Instance()->LoadSoundAuto(SoundName, true);
		if (!Sound) return;
		Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));		
		Sound->Play(true);
		Sound->SetMaxDistance(SoundDistance);
		
	}
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeActor::SetWorld(WeWorld* World_)
{
	if (World != NULL)
		World->RemoveActor(this);

	if (World_ != NULL)
		World_->AddActor(this);

	World = World_;
}


void WeActor::OnTeleported()
{
}


void WeActor::TickPrefab()
{
	PrevTM = TM;

	if (PhysicHandle)
		PhysicHandle->GetTransformation(TM);		
	Location.x = TM._41;
	Location.y = TM._42;
	Location.z = TM._43;

	TMWithScale = TM;
	if (Model) Model->UpdateTransformations(TM, PrevTM);			
	HasTicked = true;	 
	ObjectAge += WeTimer::Instance()->GetDeltaTimeF();
	OccludedTime += WeTimer::Instance()->GetDeltaTimeF(); 

	//// suppose this actor is occluded. If it is not BatchRenderer will reset this to 0	
	if (LifeTime != 0.0f)
	{
		LifeTime -= WeTimer::Instance()->GetDeltaTimeF();
		if (LifeTime <= 0.0f)
		{
			delete this;
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeActor::Tick()
{	
	// do prefab logic if it is prefab

	if (PhysicModel.Type == PHYSICMODEL_STATIC_SHAPE) IsPrefab = true;
	else IsPrefab = false;

	if (!HasTicked) ReInsertIntoWorld();
	HasTicked = true;


	if (IsPrefab) 
	{
		TickPrefab();
	} else
	{

		WeMatrix TM2, RotationMatrix, TransMatrix, ScaleMatrix, OldTM;

		if (IsLight)
			OldTM = TM;

		PrevTM = TM;
		ObjectAge += WeTimer::Instance()->GetDeltaTimeF();

		// suppose this actor is occluded. If it is not BatchRenderer will reset this to 0
		OccludedTime += WeTimer::Instance()->GetDeltaTimeF(); 
		if (LifeTime != 0.0f)
		{
			LifeTime -= WeTimer::Instance()->GetDeltaTimeF();
			if (LifeTime <= 0.0f)
			{
				destroy();

				if (AutoDestroy)
					delete this;
				else
				{
					// ----- ~WeActor() -----
					if (World)
						World->RemoveActor(this);

					SAFE_DELETE( Model );
					SAFE_DELETE( PhysicHandle );
					// ----------------------
				}
				return;
			}
		}

		// if object needs new translation matrix than make it
		bool NeedScale;

		if (Scale.x != 1 || Scale.y != 1 || Scale.z != 1)
		{
			D3DXMatrixScaling(&ScaleMatrix, Scale.x, Scale.y, Scale.z);
			NeedScale = true;
		} else NeedScale = false;


		// if we dont use physic handle then make matrices
		TMWithScale = TM;
		if (!PhysicHandle && AutoUpdateTransformations)
		{
			D3DXMatrixTranslation(&TransMatrix, Location.x, Location.y, Location.z);
			D3DXMatrixRotationYawPitchRoll(&RotationMatrix, Yaw, Pitch, Roll);

			if (NeedScale) D3DXMatrixMultiply(&TM, &ScaleMatrix, &RotationMatrix);
			else TM = RotationMatrix;
			D3DXMatrixMultiply(&TM, &TM, &TransMatrix);		
			OldLocation = Location;
			

			D3DXVECTOR4 tmpVec4;
			D3DXMatrixRotationYawPitchRoll(&RotationMatrix, Yaw, Pitch, Roll);
			D3DXVec3Transform(&tmpVec4, &WeVector(0,0,1), &RotationMatrix);
			Direction = (WeVector)tmpVec4;
			OldYaw = Yaw;
			OldPitch = Pitch;
			OldRoll = Roll;
			TMWithScale = TM;
		}
		else if (PhysicHandle)
		{
			PhysicHandle->GetTransformation(TM);
			
			if (NeedScale) D3DXMatrixMultiply(&TMWithScale, &ScaleMatrix, &TM);				
			else TMWithScale = TM;

			//D3DXVECTOR4 tmpVec4;
			//D3DXMatrixRotationYawPitchRoll(&RotationMatrix,Yaw, Pitch, Roll);
			//D3DXVec3Transform(&tmpVec4, &WeVector(0,0,1), &RotationMatrix);
			////Direction = (WeVector)tmpVec4;
			//OldYaw = Yaw;
			//OldPitch = Pitch;
			//OldRoll = Roll;

			Direction.x = TM._31;
			Direction.y = TM._32;
			Direction.z = TM._33;
		}	

		Location.x = TM._41;
		Location.y = TM._42;
		Location.z = TM._43;

		//BSphere.Position = Location;
		
		if (Model && !IsSelection)
			Model->UpdateTransformations(TMWithScale, PrevTM);


		if (Sound)
		{
			Sound->Update(Location, WeVector(0.0f, 0.0f, 0.0f));
			Sound->SetMaxDistance(SoundDistance);
		}

		if (IsLight && !InEditor)
			TM = OldTM;
	}		
		


}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeActor::MoveTo(WeVector &location)
{
	// If this is prefab then remove it from world and insert it again
	if (IsPrefab) World->RemoveActor(this);

	Location = location;

	if (PhysicHandle)
	{		
		PhysicHandle->SetLocation(location);		
	}

	if (IsPrefab) World->AddActor(this);

}


void WeActor::SetTransformation(WeMatrix &NewTransform)
{
	if (IsPrefab)
	{
		if (World) World->RemoveActor(this);
	}

	TM = NewTransform;
	if (PhysicHandle)
	{
		PhysicHandle->SetTransformation(NewTransform);
	}

	Location.x = TM._41;
	Location.y = TM._42;
	Location.z = TM._43;

	if (IsPrefab) 
	{
		if (World) World->AddActor(this);
	}

}


void WeActor::GetTransformation(WeMatrix &outM)
{
	outM = TM;
}

void WeActor::SetOrientation(float yaw, float pitch, float roll)
{
	if (IsPrefab) World->RemoveActor(this);

	Yaw = yaw;
	Pitch = pitch;
	Roll = roll;
	if (PhysicHandle)
	{
		PhysicHandle->SetOrientation(yaw, pitch, roll);

	}

	if (IsPrefab) World->AddActor(this);

}

void WeActor::GetOrientation(float &yaw, float &pitch, float &roll)
{
	yaw = Yaw;
    pitch = Pitch;
	roll = Roll;
}


void WeActor::GetLocalAxis(WeVector &XAxis, WeVector &YAxis, WeVector &ZAxis)
{
	// extract axis from transformation matrix

	XAxis.x = TM._11;
	XAxis.y = TM._12;
	XAxis.z = TM._13;

	YAxis.x = TM._21;
	YAxis.y = TM._22;
	YAxis.z = TM._23;

	ZAxis.x = TM._31;
	ZAxis.y = TM._32;
	ZAxis.z = TM._33;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeActor::OnRender(WeCamera* Cam)
{
	if (!Renderable || !Editor_Renderable) return;
	//if (World) 
	//	if (LastRenderedFrame >= World->GetWorldFrame()) return;		

		
	if(Model) 
	{	
		// if lower then max distance
		if ((MaxRenderDistance > 0.0f) && WeRenderDevice::Instance()->GetUseMaxRenderingDistance())
		{
			WeVector v;
			if (IsPrefab)
				v = Cam->Location - BSphere.Position;
			else
				v = Cam->Location - Location;

			if (D3DXVec3LengthSq(&v) > MaxRenderDistance*MaxRenderDistance) return;
		}

		Model->Draw(World, Cam, this);
		LastRenderedFrame++;
	}
	
}







/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeActor::PreRender(WeCamera* Cam)
{
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeActor::PostRender(WeCamera* Cam)
{
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Serialize
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  This will save state of object
//
//////////////////////////////////////////////////////////////////////////
void WeActor::Serialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

    
	FS.WriteInt(Version);
	FS.WriteInt(Type);
	//if (Type == WETYPE_PREFAB) MessageBox(0, "Snimam Prefab", 0, 0);
	FS.WriteBool(IsPrefab);
	FS.WriteInt(Tag);
	FS.WriteInt(EngineTag);

	FS.WriteFloat(LifeTime);
	FS.WriteFloat(ObjectAge);

	FS.WriteBool(GhostObject);
	FS.WriteBool(StaticObject);
	FS.WriteBool(Renderable);
	FS.WriteBool(HasTicked);

	FS.WriteVector(BSphere.Position);
	FS.WriteFloat(BSphere.Radius);

	FS.WriteVector(BBox.Min);
	FS.WriteVector(BBox.Max);

	FS.WriteVector(Location);
	FS.WriteMatrix(TM);
	FS.WriteVector(Scale);
	FS.WriteVector(Direction);
	FS.WriteFloat(Yaw);
	FS.WriteFloat(Pitch);
	FS.WriteFloat(Roll);

	FS.WriteBool(Teleport);
	FS.WriteBool(TeleportRelative);
	FS.WriteVector(Location); // TeleportPoint
	FS.WriteVector(Location); // TeleportDirection
	FS.WriteBool(false); // TeleportChangeDirection
	FS.WriteBool(IsTrigger);
	FS.WriteInt(MaterialPreset);
	FS.WriteString(ModelName);
	FS.WriteString(PhysicModelName);
	FS.WriteString(Name);
	FS.WriteString(TeleportDestination);

	FS.WriteBool(UseMaterialParams);
	FS.WriteFloat(MaterialParam1);
	FS.WriteFloat(MaterialParam2);
	FS.WriteFloat(MaterialParam3);
	FS.WriteFloat(MaterialParam4);
	FS.WriteColor(MaterialColor);

	FS.WriteBool(CastShadowVolume);
	FS.WriteBool(ReceiveShadowVolume);
	FS.WriteBool(ReceiveProjectors);
	FS.WriteString(SoundName);
	FS.WriteFloat(SoundDistance);
	FS.WriteBool(LightOccluder);
	FS.WriteFloat(MaxRenderDistance);
	LightMap.Serialize(FS);
	
	FS.WriteBool(PassFlares);
	
	PhysicModel.Serialize(FS);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeActor::Deserialize(WeFileStream &FS)
{
	WeVector dummy;

	if (!FS.IsOpened) return;

	int v;
	v = FS.ReadInt();
	if (v == 1)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		GhostObject = false;
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		FS.ReadBool(); // Teleport
		FS.ReadBool(); // TeleportRelative
		FS.ReadVector(dummy); // TeleportPoint
		IsTrigger = FS.ReadBool();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
	} // end of version 1
	if (v == 2)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		GhostObject = false;

		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		FS.ReadBool(); // Teleport
		FS.ReadBool(); // TeleportRelative
		FS.ReadVector(dummy); // TeleportPoint
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
	} // end of version 2
	if (v == 3 || v == 4)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		if (v == 3)
			GhostObject = false;
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		FS.ReadBool(); // Teleport
		FS.ReadBool(); // TeleportRelative
		FS.ReadVector(dummy); // TeleportPoint
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		LightMap.Deserialize(FS);
	} // end of version 3 && 4
	if (v == 5)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		Teleport = FS.ReadBool();
		TeleportRelative = FS.ReadBool();
		FS.ReadVector(dummy); // TeleportPoint
		FS.ReadVector(dummy); // TeleportDirection
		FS.ReadBool(); // TeleportChangeDirection
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		LightMap.Deserialize(FS);
	}	//end of version 5
	if (v == 6)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		Teleport = FS.ReadBool();
		TeleportRelative = FS.ReadBool();
		FS.ReadVector(dummy); // TeleportPoint
		FS.ReadVector(dummy); // TeleportDirection
		FS.ReadBool(); // TeleportChangeDirection
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		Name = FS.ReadString();
		LightMap.Deserialize(FS);
	}	//end of version 6
	if (v == 7)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		Teleport = FS.ReadBool();
		TeleportRelative = FS.ReadBool();
		FS.ReadVector(dummy); // TeleportPoint
		FS.ReadVector(dummy); // TeleportDirection
		FS.ReadBool(); // TeleportChangeDirection
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		Name = FS.ReadString();
		TeleportDestination = FS.ReadString();

		LightMap.Owner = this;
		LightMap.Deserialize(FS);

	}	//end of version 7
	if (v == 8)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		Teleport = FS.ReadBool();
		TeleportRelative = FS.ReadBool();
		FS.ReadVector(dummy); // TeleportPoint
		FS.ReadVector(dummy); // TeleportDirection
		FS.ReadBool(); // TeleportChangeDirection
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		Name = FS.ReadString();
		TeleportDestination = FS.ReadString();
		UseMaterialParams = FS.ReadBool();
		MaterialParam1 = FS.ReadFloat();
		MaterialParam2 = FS.ReadFloat();
		MaterialParam3 = FS.ReadFloat();
		MaterialParam4 = FS.ReadFloat();

		LightMap.Owner = this;
		LightMap.Deserialize(FS);


	}	//end of version 8
	if (v == 9)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		Teleport = FS.ReadBool();
		TeleportRelative = FS.ReadBool();
		FS.ReadVector(dummy); // TeleportPoint
		FS.ReadVector(dummy); // TeleportDirection
		FS.ReadBool(); // TeleportChangeDirection
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		Name = FS.ReadString();
		TeleportDestination = FS.ReadString();
		UseMaterialParams = FS.ReadBool();
		MaterialParam1 = FS.ReadFloat();
		MaterialParam2 = FS.ReadFloat();
		MaterialParam3 = FS.ReadFloat();
		MaterialParam4 = FS.ReadFloat();

		CastShadowVolume = FS.ReadBool();
		ReceiveShadowVolume = FS.ReadBool();

		LightMap.Owner = this;
		LightMap.Deserialize(FS);


	}	//end of version 9
	if (v == 10)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		Teleport = FS.ReadBool();
		TeleportRelative = FS.ReadBool();
		FS.ReadVector(dummy); // TeleportPoint
		FS.ReadVector(dummy); // TeleportDirection
		FS.ReadBool(); // TeleportChangeDirection
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		Name = FS.ReadString();
		TeleportDestination = FS.ReadString();
		UseMaterialParams = FS.ReadBool();
		MaterialParam1 = FS.ReadFloat();
		MaterialParam2 = FS.ReadFloat();
		MaterialParam3 = FS.ReadFloat();
		MaterialParam4 = FS.ReadFloat();

		CastShadowVolume = FS.ReadBool();
		ReceiveShadowVolume = FS.ReadBool();
		ReceiveProjectors = FS.ReadBool();
		ReceiveProjectors = true;
		
		LightMap.Owner = this;
		LightMap.Deserialize(FS);
	}	//end of version 10

	if (v == 11)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		Teleport = FS.ReadBool();
		TeleportRelative = FS.ReadBool();
		FS.ReadVector(dummy); // TeleportPoint
		FS.ReadVector(dummy); // TeleportDirection
		FS.ReadBool(); // TeleportChangeDirection
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		Name = FS.ReadString();
		TeleportDestination = FS.ReadString();
		UseMaterialParams = FS.ReadBool();
		MaterialParam1 = FS.ReadFloat();
		MaterialParam2 = FS.ReadFloat();
		MaterialParam3 = FS.ReadFloat();
		MaterialParam4 = FS.ReadFloat();

		CastShadowVolume = FS.ReadBool();
		ReceiveShadowVolume = FS.ReadBool();
		ReceiveProjectors = FS.ReadBool();
		ReceiveProjectors = true;

		LightMap.Owner = this;
		LightMap.Deserialize(FS);

		// new in version 11
		PassFlares = FS.ReadBool();
	}	//end of version 11

	if (v == 12)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		Teleport = FS.ReadBool();
		TeleportRelative = FS.ReadBool();
		FS.ReadVector(dummy); // TeleportPoint
		FS.ReadVector(dummy); // TeleportDirection
		FS.ReadBool(); // TeleportChangeDirection
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		Name = FS.ReadString();
		TeleportDestination = FS.ReadString();
		UseMaterialParams = FS.ReadBool();
		MaterialParam1 = FS.ReadFloat();
		MaterialParam2 = FS.ReadFloat();
		MaterialParam3 = FS.ReadFloat();
		MaterialParam4 = FS.ReadFloat();
		// new in version 12
		FS.ReadColor(MaterialColor);

		CastShadowVolume = FS.ReadBool();
		ReceiveShadowVolume = FS.ReadBool();
		ReceiveProjectors = FS.ReadBool();
		ReceiveProjectors = true;

		LightMap.Owner = this;
		LightMap.Deserialize(FS);

		// new in version 11
		PassFlares = FS.ReadBool();

	}	//end of version 12

	if (v == 13)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		Teleport = FS.ReadBool();
		TeleportRelative = FS.ReadBool();
		FS.ReadVector(dummy); // TeleportPoint
		FS.ReadVector(dummy); // TeleportDirection
		FS.ReadBool(); // TeleportChangeDirection
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		Name = FS.ReadString();
		TeleportDestination = FS.ReadString();
		UseMaterialParams = FS.ReadBool();
		MaterialParam1 = FS.ReadFloat();
		MaterialParam2 = FS.ReadFloat();
		MaterialParam3 = FS.ReadFloat();
		MaterialParam4 = FS.ReadFloat();
		// new in version 12
		FS.ReadColor(MaterialColor);

		CastShadowVolume = FS.ReadBool();
		ReceiveShadowVolume = FS.ReadBool();
		ReceiveProjectors = FS.ReadBool();
		//ReceiveProjectors = true;

		// new in version 13
		LightOccluder = FS.ReadBool();
		MaxRenderDistance = FS.ReadFloat();

		LightMap.Owner = this;
		LightMap.Deserialize(FS);

		// new in version 11
		PassFlares = FS.ReadBool();

	}	//end of version 13

	if (v == 14)
	{
		Type = FS.ReadInt();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		Teleport = FS.ReadBool();
		TeleportRelative = FS.ReadBool();
		FS.ReadVector(dummy); // TeleportPoint
		FS.ReadVector(dummy); // TeleportDirection
		FS.ReadBool(); // TeleportChangeDirection
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		Name = FS.ReadString();
		TeleportDestination = FS.ReadString();
		UseMaterialParams = FS.ReadBool();
		MaterialParam1 = FS.ReadFloat();
		MaterialParam2 = FS.ReadFloat();
		MaterialParam3 = FS.ReadFloat();
		MaterialParam4 = FS.ReadFloat();
		// new in version 12
		FS.ReadColor(MaterialColor);

		CastShadowVolume = FS.ReadBool();
		ReceiveShadowVolume = FS.ReadBool();
		ReceiveProjectors = FS.ReadBool();
		//ReceiveProjectors = true;

        // new in version 14
		SoundName = FS.ReadString();
		SoundDistance = FS.ReadFloat();

		// new in version 13
		LightOccluder = FS.ReadBool();
		MaxRenderDistance = FS.ReadFloat();

		LightMap.Owner = this;
		LightMap.Deserialize(FS);

		// new in version 11
		PassFlares = FS.ReadBool();
		

	}	//end of version 14

	if (v == 15)
	{
		Type = FS.ReadInt();
		IsPrefab = FS.ReadBool();
		Tag = FS.ReadInt();
		EngineTag = FS.ReadInt();

		LifeTime = FS.ReadFloat();
		ObjectAge = FS.ReadFloat();

		GhostObject = FS.ReadBool();
		StaticObject = FS.ReadBool();
		Renderable = FS.ReadBool();
		HasTicked = FS.ReadBool();

		FS.ReadVector(BSphere.Position);
		BSphere.Radius = FS.ReadFloat();

		FS.ReadVector(BBox.Min);
		FS.ReadVector(BBox.Max);

		FS.ReadVector(Location);
		FS.ReadMatrix(TM);
		FS.ReadVector(Scale);
		FS.ReadVector(Direction);
		Yaw  = FS.ReadFloat();
		Pitch = FS.ReadFloat();
		Roll = FS.ReadFloat();

		Teleport = FS.ReadBool();
		TeleportRelative = FS.ReadBool();
		FS.ReadVector(dummy); // TeleportPoint
		FS.ReadVector(dummy); // TeleportDirection
		FS.ReadBool(); // TeleportChangeDirection
		IsTrigger = FS.ReadBool();
		MaterialPreset = FS.ReadInt();
		ModelName = FS.ReadString();
		PhysicModelName = FS.ReadString();
		Name = FS.ReadString();
		TeleportDestination = FS.ReadString();
		UseMaterialParams = FS.ReadBool();
		MaterialParam1 = FS.ReadFloat();
		MaterialParam2 = FS.ReadFloat();
		MaterialParam3 = FS.ReadFloat();
		MaterialParam4 = FS.ReadFloat();
		// new in version 12
		FS.ReadColor(MaterialColor);

		CastShadowVolume = FS.ReadBool();
		ReceiveShadowVolume = FS.ReadBool();
		ReceiveProjectors = FS.ReadBool();
		//ReceiveProjectors = true;

		// new in version 14
		SoundName = FS.ReadString();
		SoundDistance = FS.ReadFloat();

		// new in version 13
		LightOccluder = FS.ReadBool();
		MaxRenderDistance = FS.ReadFloat();

		LightMap.Owner = this;
		LightMap.Deserialize(FS);

		// new in version 11
		PassFlares = FS.ReadBool();

		// new in version 15
		PhysicModel.Deserialize(FS);

	}	//end of version 15


	//Initialize();
	//SetTransformation(TM);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Clone
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  This will make this actor to have all settings from Original but not 
//								you have to manually put him into world
//
//////////////////////////////////////////////////////////////////////////
void WeActor::Clone(WeActor *Clone, bool DeepCopy)
{		
	Clone->Renderable = Renderable;
    Clone->LifeTime = LifeTime;
	Clone->ObjectAge = ObjectAge;		
	Clone->BBox = BBox;
	Clone->BSphere = BSphere;
	//Clone->Model = WeModelManager::Instance()->Clone(Model, DeepCopy);
	//Clone->Model = WeModelManager::Instance()->Load(ModelName);
	Clone->ModelName = ModelName;
	Clone->SoundName = SoundName;
	Clone->SoundDistance = SoundDistance;
	Clone->PhysicModelName = PhysicModelName;
	Clone->Type = Type;
	Clone->Tag = Tag;
	Clone->IsNetworkActor = IsNetworkActor;
	Clone->IsPrefab = IsPrefab;
	Clone->IsLight = IsLight;
	Clone->IsParticleSystem = IsParticleSystem;
	Clone->IsCloth = IsCloth;
	Clone->PhysicModel = PhysicModel;
	
	//Clone->PhysicHandle = PhysicHandle;
	Clone->MaxRenderDistance = MaxRenderDistance;
	Clone->LightOccluder = LightOccluder;
	Clone->GhostObject = GhostObject;
	Clone->EngineTag = EngineTag;
	Clone->IsTrigger = IsTrigger;
	Clone->Teleport = Teleport;
	Clone->TeleportRelative = TeleportRelative;
	Clone->TeleportDestination = TeleportDestination;
	Clone->Yaw = Yaw;
	Clone->Pitch = Pitch;
	Clone->Roll = Roll;
	Clone->TM = TM;
	Clone->Scale = Scale;
	Clone->Location = Location;
	Clone->Direction = Direction;
	Clone->Velocity = Velocity;
	Clone->AngularVelocity = AngularVelocity;
	Clone->OldLocation = Clone->Location;
	Clone->OldDirection = Clone->Direction;
	Clone->OldVelocity = Clone->Velocity;
	Clone->OldYaw = Clone->Yaw;
	Clone->OldPitch = Clone->Pitch;
	Clone->OldRoll = Clone->Roll;	
	Clone->InEditor = InEditor;
	Clone->Version = Version;
	Clone->MaterialPreset = MaterialPreset;
	Clone->MaterialParam1 = MaterialParam1;
	Clone->MaterialParam2 = MaterialParam2;
	Clone->MaterialParam3 = MaterialParam3;
	Clone->MaterialParam4 = MaterialParam4;
	Clone->UseMaterialParams = UseMaterialParams;
	Clone->CastShadowVolume = CastShadowVolume;
	Clone->ReceiveShadowVolume = ReceiveShadowVolume;
	Clone->ReceiveProjectors = ReceiveProjectors;

	Clone->PassFlares = PassFlares;

	Clone->Initialize();
	//SAFE_DELETE(Clone->PhysicHandle);
	//Clone->PhysicHandle = WePhysics::Instance()->CreateTriangleMesh(Clone, Clone->Location, Clone->Model, 10, WE_PHYSICHANDLE_KINEMATIC);	
	Clone->SetTransformation(TM);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeActor::SetScaling(WeVector &scale)
{	
	if (Scale == scale)
	{
		return;
	} else
	{
		Scale = scale;	
	}	
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeActor::Editor_Hide()
{
	if (!InEditor) return;

	if (PhysicHandle) 
	{		
		PhysicHandle->SetRaycasting(false);		
	}
	Editor_Renderable = false;

}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
void WeActor::Editor_Show()
{
	if (!InEditor) return;
    
	if (PhysicHandle) 
	{
		PhysicHandle->SetRaycasting(true);		
	}
	Editor_Renderable = true;
}



bool WeActor::IntersectFrustum(WeFrustum *frustum)
{
	if (World)
	{
		int iType = -1 - Type;

		if ((iType >= 0) && (iType < 10))
		{
			if (!World->ShowActorTypes[iType])
				return false;
		}

		if (Type >= 0)
		{
			if (!World->ShowActorTypes[-1 - WETYPE_PREFAB])
				return false;
		}
	}

	WeSphere s;
	if (IsPrefab)
	{
		return frustum->Collide(BSphere);
	} else
	{
		if (Model)
		{
			if (!Model->IsModel)
			{
				if (Model->Mesh)
				{
					s = Model->Mesh->BSphere;
					s.Position += Location;			
					return frustum->Collide(s);
				}
			} else
			{
				return frustum->Collide(BSphere);			
			}
		}
	}
	return false;
}


float WeActor::GetFinalAge()
{
	return (ObjectAge + LifeTime);
}



void WeActor::destroy()
{
}
