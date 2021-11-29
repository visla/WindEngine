/*

     WeParticleSystem.cpp

	 Autor: Nebojsa Vislavski
	 Date: 8.11.2005.
	 Version:1.0

	 This is main particle system class. Use this and if you want you can inherit it.

*/

#include <WeParticleSystem.h>
#include <WeBatchItem.h>
#include <WeParticleRenderer.h>
#include <WeTimer.h>
#include <WeRandomizer.h>
#include <WeBatchRenderer.h>
#include <WeCamera.h>
#include <WeStaticModel.h>
#include <WeEngine.h>
#include <WeMaterialManager.h>
#include <WePackage.h>


WeParticleSystem::SProperties::SProperties()
{
	Version = -4;
	MaxParticles = 1000;
	ParticleCount = 0;
	EmitRate = 0;
	EmitRateRandomness = 0;						
	SpawnType = WE_PARTICLE_SPAWNTYPE_POINT;
	MoveType = WE_PARTICLE_MOVETYPE_DIRECTIONAL;
		// params like dimension, radius, width, height for different spawn types
	SpawnParam1 = 0;
	SpawnParam2 = 0;
	SpawnParam3 = 0;
	SpawnParam4 = 0;
	RelativePosition = false;

	// visual representations params	
	BlendType = WEPARTICLE_BLEND_ADDITIVE;
	MaterialName = WeEngine::Instance()->Desc.DefaultMaterialName;	
	ColorFrom = WeColor(1,1,1,1);
	ColorTo = WeColor(1,1,1,1);

	SpinFrom = 0;
	SpinTo = 0;
	SpinRandomness = 0;
	SpinningInLife = true;

	AlphaFrom = 1.0;
	AlphaTo = 1.0;
	AlphaRandomness = 0;
	VisibleSphereRadius = 10.0f;

		// other params
	SizeFrom = 1.0;				
	SizeTo = 1.0;   
	SizeRandomenss = 0.0f;

	Gravity = WeVector(0,0,0);
	ParticleDirection = WeVector(0,1,0);

	StartVelocity = 0;			
	StartVelocityRandomness = 0;
	Spread = 0;		

	LifeTime = 5.0f;
	LifeTimeRandomness = 0.0f;

	AngleType = WEPARTICLE_ANGLE_SPECIFY;	
	ParticleAngle = 0;
	AnimType = WEPARTICLE_ANIM_NONE;
	Material = NULL;
}


void WeParticleSystem::SProperties::Serialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	FS.WriteInt(Version);
	FS.WriteInt(MaxParticles);
	FS.WriteInt(EmitRate);	
	FS.WriteInt(EmitRateRandomness);
	FS.WriteInt(SpawnType);
	FS.WriteInt(MoveType);	
	FS.WriteFloat(SpawnParam1);
	FS.WriteFloat(SpawnParam2);
	FS.WriteFloat(SpawnParam3);
	FS.WriteFloat(SpawnParam4);
	FS.WriteInt(BlendType);	
	FS.WriteString(MaterialName);
	FS.WriteInt(Colors.GetItemsCount());
	for (Colors.First(); !Colors.End(); Colors.Next())
	{
		FS.WriteColor(Colors.GetItem().Color);
		FS.WriteFloat(Colors.GetItem().GradientPoint);
	}
	FS.WriteColor(ColorFrom);
	FS.WriteColor(ColorTo);
	FS.WriteFloat(SpinFrom);
	FS.WriteFloat(SpinTo);
	FS.WriteFloat(SpinRandomness);	
	FS.WriteBool(SpinningInLife);
	FS.WriteFloat(AlphaFrom);
	FS.WriteFloat(AlphaTo);
	FS.WriteFloat(AlphaRandomness);
	FS.WriteFloat(SizeFrom);
	FS.WriteFloat(SizeTo);
	FS.WriteFloat(SizeRandomenss);
	FS.WriteVector(Gravity);
	FS.WriteVector(ParticleDirection);
	FS.WriteFloat(StartVelocity);
	FS.WriteFloat(StartVelocityRandomness);
	FS.WriteFloat(Spread);
	FS.WriteFloat(LifeTime);
	FS.WriteFloat(LifeTimeRandomness);
	FS.WriteInt(AngleType);
	FS.WriteFloat(ParticleAngle);
	FS.WriteInt(AnimType);	
	FS.WriteBool(RelativePosition);	
	FS.WriteFloat(VisibleSphereRadius);
}

void WeParticleSystem::SProperties::Deserialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;


	int ver;
	ver = FS.ReadInt();
	if (ver > 0)
	{
		FS.Seek(-4, SEEK_CUR);
		MaxParticles = FS.ReadInt();
		EmitRate = FS.ReadInt();	

		SpawnVolumeSize = FS.ReadFloat();
		FS.ReadVector(SpawnLine);
		SpawnPlaneWidth =  FS.ReadFloat();
		SpawnPlaneHeight = FS.ReadFloat();
		SpawnType = FS.ReadInt();
		MoveType = FS.ReadInt();
		FS.ReadColor(ColorFrom);
		FS.ReadColor(ColorTo);
		AlphaFrom = FS.ReadFloat();
		AlphaTo = FS.ReadFloat();
		SizeFrom = FS.ReadFloat();
		SizeTo = FS.ReadFloat();
		FS.ReadVector(Gravity);
		FS.ReadVector(ParticleDirection);
		StartVelocity = FS.ReadFloat();
		Spread = FS.ReadFloat();
		LifeTime = FS.ReadFloat();
		MaterialName = FS.ReadString();
	} 
	if (ver == -1)
	{
		MaxParticles	= FS.ReadInt();
		EmitRate		= FS.ReadInt();	
		EmitRateRandomness = FS.ReadInt();
		SpawnType = FS.ReadInt();
		MoveType = FS.ReadInt();	
		SpawnParam1 = FS.ReadFloat();
		SpawnParam2 = FS.ReadFloat();
		SpawnParam3 = FS.ReadFloat();
		SpawnParam4 = FS.ReadFloat();
		BlendType = (WEPARTICLE_BLEND_TYPE)FS.ReadInt();	
		MaterialName = FS.ReadString();
		int tmp;
		tmp = FS.ReadInt();
		for (int i = 0; i < tmp; ++i)
		{
			ColorState cs;
		
			FS.ReadColor(cs.Color);
			cs.GradientPoint = FS.ReadFloat();
			Colors.PushBack(cs);
		}
		FS.ReadColor(ColorFrom);
		FS.ReadColor(ColorTo);
		SpinFrom = FS.ReadFloat();
		SpinTo = FS.ReadFloat();
		SpinRandomness = FS.ReadFloat();	
		AlphaFrom = FS.ReadFloat();
		AlphaTo = FS.ReadFloat();
		AlphaRandomness = FS.ReadFloat();
		SizeFrom = FS.ReadFloat();
		SizeTo = FS.ReadFloat();
		SizeRandomenss = FS.ReadFloat();
		FS.ReadVector(Gravity);
		FS.ReadVector(ParticleDirection);
		StartVelocity = FS.ReadFloat();
		StartVelocityRandomness = FS.ReadFloat();
		Spread = FS.ReadFloat();
		LifeTime = FS.ReadFloat();
		LifeTimeRandomness = FS.ReadFloat();
		AngleType = (WEPARTICLE_ANGLE_TYPE)FS.ReadInt();
		ParticleAngle = FS.ReadFloat();
		AnimType = (WEPARTICLE_ANIM_TYPE)FS.ReadInt();	
	}
	if (ver == -2)
	{
		MaxParticles	= FS.ReadInt();
		EmitRate		= FS.ReadInt();	
		EmitRateRandomness = FS.ReadInt();
		SpawnType = FS.ReadInt();
		MoveType = FS.ReadInt();	
		SpawnParam1 = FS.ReadFloat();
		SpawnParam2 = FS.ReadFloat();
		SpawnParam3 = FS.ReadFloat();
		SpawnParam4 = FS.ReadFloat();
		BlendType = (WEPARTICLE_BLEND_TYPE)FS.ReadInt();	
		MaterialName = FS.ReadString();
		int tmp;
		tmp = FS.ReadInt();
		for (int i = 0; i < tmp; ++i)
		{
			ColorState cs;

			FS.ReadColor(cs.Color);
			cs.GradientPoint = FS.ReadFloat();
			Colors.PushBack(cs);
		}
		FS.ReadColor(ColorFrom);
		FS.ReadColor(ColorTo);
		SpinFrom = FS.ReadFloat();
		SpinTo = FS.ReadFloat();
		SpinRandomness = FS.ReadFloat();	
		AlphaFrom = FS.ReadFloat();
		AlphaTo = FS.ReadFloat();
		AlphaRandomness = FS.ReadFloat();
		SizeFrom = FS.ReadFloat();
		SizeTo = FS.ReadFloat();
		SizeRandomenss = FS.ReadFloat();
		FS.ReadVector(Gravity);
		FS.ReadVector(ParticleDirection);
		StartVelocity = FS.ReadFloat();
		StartVelocityRandomness = FS.ReadFloat();
		Spread = FS.ReadFloat();
		LifeTime = FS.ReadFloat();
		LifeTimeRandomness = FS.ReadFloat();
		AngleType = (WEPARTICLE_ANGLE_TYPE)FS.ReadInt();
		ParticleAngle = FS.ReadFloat();
		AnimType = (WEPARTICLE_ANIM_TYPE)FS.ReadInt();	
		RelativePosition = FS.ReadBool();
	}

	if (ver == -3)
	{
		MaxParticles	= FS.ReadInt();
		EmitRate		= FS.ReadInt();	
		EmitRateRandomness = FS.ReadInt();
		SpawnType = FS.ReadInt();
		MoveType = FS.ReadInt();	
		SpawnParam1 = FS.ReadFloat();
		SpawnParam2 = FS.ReadFloat();
		SpawnParam3 = FS.ReadFloat();
		SpawnParam4 = FS.ReadFloat();
		BlendType = (WEPARTICLE_BLEND_TYPE)FS.ReadInt();	
		MaterialName = FS.ReadString();
		int tmp;
		tmp = FS.ReadInt();
		for (int i = 0; i < tmp; ++i)
		{
			ColorState cs;

			FS.ReadColor(cs.Color);
			cs.GradientPoint = FS.ReadFloat();
			Colors.PushBack(cs);
		}
		FS.ReadColor(ColorFrom);
		FS.ReadColor(ColorTo);
		SpinFrom = FS.ReadFloat();
		SpinTo = FS.ReadFloat();
		SpinRandomness = FS.ReadFloat();	
		SpinningInLife = FS.ReadBool();
		AlphaFrom = FS.ReadFloat();
		AlphaTo = FS.ReadFloat();
		AlphaRandomness = FS.ReadFloat();
		SizeFrom = FS.ReadFloat();
		SizeTo = FS.ReadFloat();
		SizeRandomenss = FS.ReadFloat();
		FS.ReadVector(Gravity);
		FS.ReadVector(ParticleDirection);
		StartVelocity = FS.ReadFloat();
		StartVelocityRandomness = FS.ReadFloat();
		Spread = FS.ReadFloat();
		LifeTime = FS.ReadFloat();
		LifeTimeRandomness = FS.ReadFloat();
		AngleType = (WEPARTICLE_ANGLE_TYPE)FS.ReadInt();
		ParticleAngle = FS.ReadFloat();
		AnimType = (WEPARTICLE_ANIM_TYPE)FS.ReadInt();	
		RelativePosition = FS.ReadBool();
	} // end of version -3


	if (ver == -4)
	{
		MaxParticles	= FS.ReadInt();
		EmitRate		= FS.ReadInt();	
		EmitRateRandomness = FS.ReadInt();
		SpawnType = FS.ReadInt();
		MoveType = FS.ReadInt();	
		SpawnParam1 = FS.ReadFloat();
		SpawnParam2 = FS.ReadFloat();
		SpawnParam3 = FS.ReadFloat();
		SpawnParam4 = FS.ReadFloat();
		BlendType = (WEPARTICLE_BLEND_TYPE)FS.ReadInt();	
		MaterialName = FS.ReadString();
		int tmp;
		tmp = FS.ReadInt();
		for (int i = 0; i < tmp; ++i)
		{
			ColorState cs;

			FS.ReadColor(cs.Color);
			cs.GradientPoint = FS.ReadFloat();
			Colors.PushBack(cs);
		}
		FS.ReadColor(ColorFrom);
		FS.ReadColor(ColorTo);
		SpinFrom = FS.ReadFloat();
		SpinTo = FS.ReadFloat();
		SpinRandomness = FS.ReadFloat();	
		SpinningInLife = FS.ReadBool();
		AlphaFrom = FS.ReadFloat();
		AlphaTo = FS.ReadFloat();
		AlphaRandomness = FS.ReadFloat();
		SizeFrom = FS.ReadFloat();
		SizeTo = FS.ReadFloat();
		SizeRandomenss = FS.ReadFloat();
		FS.ReadVector(Gravity);
		FS.ReadVector(ParticleDirection);
		StartVelocity = FS.ReadFloat();
		StartVelocityRandomness = FS.ReadFloat();
		Spread = FS.ReadFloat();
		LifeTime = FS.ReadFloat();
		LifeTimeRandomness = FS.ReadFloat();
		AngleType = (WEPARTICLE_ANGLE_TYPE)FS.ReadInt();
		ParticleAngle = FS.ReadFloat();
		AnimType = (WEPARTICLE_ANIM_TYPE)FS.ReadInt();	
		RelativePosition = FS.ReadBool();
		VisibleSphereRadius = FS.ReadFloat();
	} // end of version -4

}

WeParticleSystem::WeParticleSystem()
{
	ModelName = "MapEditor.Geometry.ParticleSys";
	LastIndex = 0;
	ParticleData = NULL;
	ParticlePhysicHandles = NULL;
	ParticleIndex = NULL;
	IsParticleSystem = true;
	Type = WETYPE_PARTICLESYSTEM;

	//SpawnType = WE_PARTICLE_SPAWNTYPE_POINT;
	//ParticleMoveType = WE_PARTICLE_MOVETYPE_DIRECTIONAL;
	//SpawnPlaneHeight = 10;
	//SpawnPlaneWidth = 10;
	//EmitRate = 0;
	//SpawnLine = WeVector(0,0,1);
	//SpawnParticleDirection = WeVector(0,1,0);
	//SpawnVolumeSize = 10;
	NumToSpawn = 0.0f;
	//MaterialName = WeEngine::Instance()->Desc.DefaultMaterialName;
	BSphere.Radius = 10;

	setMaxParticles(Properties.MaxParticles);

	ParticlePlane = NULL;
}


void WeParticleSystem::Tick()
{
	if (!Properties.Material) return;

	if (Properties.SpawnType != WE_PARTICLE_SPAWNTYPE_HORIZPLANE)
	{
		if (ParticlePlane)
			ParticlePlane->SetWorld(NULL);
	}
	else
	{
		if (ParticlePlane)
		{
			ParticlePlane->SetWorld(World);
			ParticlePlane->Location = Location;
			ParticlePlane->Scale.x = Properties.SpawnParam1 * 2.0f;
			ParticlePlane->Scale.z = Properties.SpawnParam2 * 2.0f;
		}
	}


	// This should spawn and destroy all particles
	// if we have to spawn
	int Rnd = 0;

	if (Properties.EmitRateRandomness > 0)
		Rnd = WeRandomizer::Rand()%Properties.EmitRateRandomness*2 - Properties.EmitRateRandomness;

	NumToSpawn += (float)(Properties.EmitRate+Rnd) * WeTimer::Instance()->GetDeltaTimeF();
	
	// Update all particles and delete dead ones
	for (int i = 0; i < LastIndex; ++i)
	{
		if (ParticleData[i].Particle[0].Alive == 0.0f)
		{
			if (i == LastIndex-1) LastIndex--;
			if (NumToSpawn >= 1.0f)
			{
				NumToSpawn--;
				AddParticle(i);
			}
		} else
		{
			ParticleData[i].Update(this, WeTimer::Instance()->GetDeltaTimeF());
		}
	}

	// if we need some more to spawn then spawn them at the end 
	int j = LastIndex;
	while (NumToSpawn >= 1.0f)
	{
		if (LastIndex == Properties.MaxParticles) 
		{
			NumToSpawn = 0.0f;
			break;
		}			

		AddParticle(j);
		NumToSpawn--;
		j++;
	}

	WeActor::Tick();
}


void WeParticleSystem::Reset()
{
	if (ParticleData)
	{
		free(ParticleData);
		free(ParticleIndex);			
		ParticleData = NULL;
		ParticleIndex = NULL;
	}	
}


void WeParticleSystem::setMaxParticles(int maxParticles)
{
	if (maxParticles <= 0) return;

	free(ParticleData);
	free(ParticleIndex);

	Properties.MaxParticles = maxParticles;
	ParticleData = (WeParticle*)malloc(sizeof(WeParticle)*maxParticles);
	ParticleIndex = (WeParticleIndex*)malloc(sizeof(WeParticleIndex)*maxParticles);
	ZeroMemory(ParticleData, sizeof(WeParticle)*maxParticles);
	ZeroMemory(ParticleIndex, sizeof(WeParticleIndex)*maxParticles);	
}


void WeParticleSystem::SpawnDirection(WeVector &ParticleDirection)
{
	float theta, a2,a3;


	WeMatrix M;

	switch (Properties.MoveType)
	{
	case WE_PARTICLE_MOVETYPE_NONE: ParticleDirection = WeVector(0, 0, 0);
		break;
	case WE_PARTICLE_MOVETYPE_DIRECTIONAL: 
		ParticleDirection = Properties.ParticleDirection;
		if (Properties.Spread != 0)
		{
			float tmp = D3DX_PI /2.0f * Properties.Spread;
			theta = ((float)WeRandomizer::Rand() / (float)(RAND_MAX))* 2 * (tmp) - (tmp);
			a2 = ((float)WeRandomizer::Rand() / (float)(RAND_MAX)) * 2 *(tmp) - (tmp);
			a3 = ((float)WeRandomizer::Rand() / (float)(RAND_MAX)) * 2 * (tmp) - (tmp);
		}
		else 		
		{
			theta = 0.0f;
			a2 = 0.0f;
			a3 = 0.0f;
		}

		D3DXMatrixRotationYawPitchRoll(&M, theta, a2, a3);
		D3DXVec3TransformNormal(&ParticleDirection, &ParticleDirection, &M);		
		break;
	case WE_PARTICLE_MOVETYPE_SPHERICAL: 
		ParticleDirection = Properties.ParticleDirection;
		
		break;
	case WE_PARTICLE_MOVETYPE_CHAOTIC:
		ParticleDirection = Properties.ParticleDirection;

		if (Properties.Spread != 0)
		{
			theta = ((float)WeRandomizer::Rand() / (float)(RAND_MAX))*2*D3DX_PI - D3DX_PI;
			a2 = ((float)WeRandomizer::Rand() / (float)(RAND_MAX))*2*D3DX_PI  - D3DX_PI;
			a3 = ((float)WeRandomizer::Rand() / (float)(RAND_MAX))*2*D3DX_PI  - D3DX_PI;
		}
		else 		
		{
			theta = 0.0f;
			a2 = 0.0f;
			a3 = 0.0f;
		}

		D3DXMatrixRotationYawPitchRoll(&M, theta, a2, a3);
		D3DXVec3TransformNormal(&ParticleDirection, &ParticleDirection, &M);		
		break;		
	}
}



void WeParticleSystem::SpawnPosition(WeVector &ParticlePosition)
{
	float dx,dy,dz;
	WeVector V;

	switch (Properties.SpawnType)
	{
		case WE_PARTICLE_SPAWNTYPE_POINT:
			ParticlePosition = Location;
			break;			

		case WE_PARTICLE_SPAWNTYPE_LINE:
			float R; 
			V.x = Properties.SpawnParam1;
			V.y = Properties.SpawnParam2;
			V.z = Properties.SpawnParam3;			
			R = (float)WeRandomizer::Rand() / (float)(RAND_MAX);
			ParticlePosition = Location + V*R;
			break;


		case WE_PARTICLE_SPAWNTYPE_SPHERE:
			dx = ((float)WeRandomizer::Rand()*Properties.SpawnParam1*2 / (float)(RAND_MAX))-Properties.SpawnParam1;
			dy = ((float)WeRandomizer::Rand()*Properties.SpawnParam1*2 / (float)(RAND_MAX))-Properties.SpawnParam1;
			dz = ((float)WeRandomizer::Rand()*Properties.SpawnParam1*2 / (float)(RAND_MAX))-Properties.SpawnParam1;
			
			ParticlePosition = Location;			
			ParticlePosition.x += dx;
			ParticlePosition.y += dy;
			ParticlePosition.z += dz;
			break;

		case WE_PARTICLE_SPAWNTYPE_HORIZPLANE:
			dx = ((float)WeRandomizer::Rand()*Properties.SpawnParam1*2 / (float)(RAND_MAX))-Properties.SpawnParam1;
			dz = ((float)WeRandomizer::Rand()*Properties.SpawnParam2*2 / (float)(RAND_MAX))-Properties.SpawnParam2;

			ParticlePosition = Location;			
			ParticlePosition.x += dx;
			ParticlePosition.z += dz;
			break;
	}
}




void WeParticleSystem::AddParticle(int index)
{
	// Add this particle to this index
	if (index == LastIndex) LastIndex++;
	Properties.ParticleCount++;

	float Rnd;
	WeColor Col;
	Col = Properties.ColorFrom;
	Col.a = Properties.AlphaFrom;
	ParticleData[index].Particle[0].Color = Col;

	Rnd = (float)(WeRandomizer::Rand() % 1000)*Properties.LifeTimeRandomness*2.0f / 1000.0f;
	ParticleData[index].Particle[0].LifeTime = Properties.LifeTime+Rnd;

	Rnd = (float)(WeRandomizer::Rand() % 1000)*Properties.SpinRandomness*2.0f / 1000.0f;
	ParticleData[index].Particle[0].SpinTo = Properties.SpinTo+Rnd;

	WeVector tmpDir;
	SpawnDirection(tmpDir);

	Rnd = (float)(WeRandomizer::Rand() % 1000)*Properties.StartVelocityRandomness*2.0f / 1000.0f;
	ParticleData[index].Particle[0].Velocity = tmpDir*(Properties.StartVelocity+Rnd);

	// spawning position
    SpawnPosition(ParticleData[index].Particle[0].PPosition);
	if (Properties.RelativePosition)
		ParticleData[index].Particle[0].PPositionRel = ParticleData[index].Particle[0].PPosition - Location;


    
	Rnd = (float)(WeRandomizer::Rand() % 1000)*Properties.SizeRandomenss*2.0f / 1000.0f;

	ParticleData[index].Particle[0].SizeFrom = Properties.SizeFrom+Rnd;
	ParticleData[index].Particle[0].Size = Properties.SizeFrom+Rnd;
	ParticleData[index].Particle[0].Age = 0;
	ParticleData[index].Particle[0].Alive = 1.0f;


	// make other four vertices same as particle
	ParticleData[index].Particle[1] = ParticleData[index].Particle[0];
	ParticleData[index].Particle[2] = ParticleData[index].Particle[0];
	ParticleData[index].Particle[3] = ParticleData[index].Particle[0];

	// Fill Standard coordinates
	ParticleData[index].Particle[0].Position = WeVector(-1, 1, 0);
	ParticleData[index].Particle[0].u = 0.0f;
	ParticleData[index].Particle[0].v = 0.0f;

	ParticleData[index].Particle[1].Position = WeVector(1, 1, 0);
	ParticleData[index].Particle[1].u = 1.0f;
	ParticleData[index].Particle[1].v = 0.0f;

	ParticleData[index].Particle[2].Position = WeVector(-1, -1, 0);
	ParticleData[index].Particle[2].u = 0.0f;
	ParticleData[index].Particle[2].v = 1.0f;

	ParticleData[index].Particle[3].Position = WeVector(1, -1, 0);
	ParticleData[index].Particle[3].u = 1.0f;
	ParticleData[index].Particle[3].v = 1.0f;

	ParticleIndex[index].tri1[0] = index*4;
	ParticleIndex[index].tri1[1] = index*4 + 1;
	ParticleIndex[index].tri1[2] = index*4 + 2;

	ParticleIndex[index].tri2[0] = index*4 + 2;
	ParticleIndex[index].tri2[1] = index*4 + 1;
	ParticleIndex[index].tri2[2] = index*4 + 3;

	//ParticleData[index].Update(this, WeTimer::Instance()->GetDeltaTimeF());

}



bool WeParticleSystem::IntersectFrustum(WeFrustum *frustum)
{
	if (World)
	{
		if (!World->ShowActorTypes[-1 - WETYPE_PARTICLESYSTEM])
			return false;
	}

	// maybe is occluded by bsphere
	bool b;
	b = true;
	//if (InEditor) 
	//{
	//	b = WeActor::IntersectFrustum(frustum);
	//}

	BSphere.Position = Location;
	BSphere.Radius = Properties.VisibleSphereRadius;
	if (BSphere.Radius != 0)
	{
		return frustum->Collide(BSphere);
	}
	return b;
}

// Cause particle system dont have model this will make batch item
void WeParticleSystem::OnRender(WeCamera *Cam)
{
	if (!Renderable || !Editor_Renderable) return;
	if (World) 
		if (LastRenderedFrame >= World->GetWorldFrame()) return;		

	if (InEditor)
	{
		if (Model)
			Model->Draw(World, Cam, this);
	}

	if (!Properties.Material) return;
	// make alpha batch item and queue it	

	WeBatchItem *BatchItem = new WeBatchItem;

	BatchItem->Material = Properties.Material;
	BatchItem->Actor = this;
	BatchItem->Mesh = NULL;
	BatchItem->Alpha = true;

	WeVector tmp;
	D3DXVec3TransformCoord(&tmp, &Location, &Cam->View);
	BatchItem->ZDist = tmp.z;	


	if (BatchItem->Material->Type == WEMATERIALTYPE_REFRACTIVE)
		WeBatchRenderer::Instance()->QueueRefractiveBatch(BatchItem);
	else
		WeBatchRenderer::Instance()->QueueAlphaBatch(BatchItem);	

	LastRenderedFrame++;
}


void WeParticleSystem::Clone(WeActor *Destination, bool DeepCopy)
{
	WeActor::Clone(Destination, DeepCopy);

	WeParticleSystem *tmpPS;
	tmpPS = (WeParticleSystem *)Destination;

	tmpPS->Properties = Properties;
	tmpPS->setMaxParticles(Properties.MaxParticles);
}

void WeParticleSystem::Serialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;
	WeActor::Serialize(FS);

	Properties.Serialize(FS);
}



void WeParticleSystem::Deserialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;
	WeActor::Deserialize(FS);

	Properties.Deserialize(FS);
    
}

void WeParticleSystem::Initialize()
{
	if (InEditor)
	{
		PhysicModel.Type = PHYSICMODEL_STATIC_SHAPE;
		WeActor::Initialize();
		Properties.Material = WeMaterialManager::Instance()->Load(Properties.MaterialName);

		ParticlePlane = new WeActor();
		ParticlePlane->ModelName = "MapEditor.Geometry.ParticlePlane";
		ParticlePlane->Initialize();
		//ParticlePlane->InEditor = true;
	}
	else 
	{		
		Properties.Material = WeMaterialManager::Instance()->Load(Properties.MaterialName);
	}
}

bool WeParticleSystem::LoadPreset(StringParam _Name)
{   
	WePackage::Instance()->PackageExtension = ".ppp";
	if (WePackage::Instance()->BeginReading(_Name, 8))
	{
		Properties.Deserialize(WePackage::Instance()->DataStream);        				
		//MaxParticles = WePackage::Instance()->DataStream.ReadInt();
		//EmitRate = WePackage::Instance()->DataStream.ReadInt();
		//SpawnVolumeSize = WePackage::Instance()->DataStream.ReadFloat();
		//WePackage::Instance()->DataStream.ReadVector(SpawnLine);
		//SpawnPlaneWidth = WePackage::Instance()->DataStream.ReadFloat();
		//SpawnPlaneHeight = WePackage::Instance()->DataStream.ReadFloat();
		//SpawnType = WePackage::Instance()->DataStream.ReadInt();
		//ParticleMoveType = WePackage::Instance()->DataStream.ReadInt();
		//WePackage::Instance()->DataStream.ReadColor(ColorFrom);
		//WePackage::Instance()->DataStream.ReadColor(ColorTo);
		//AlphaFrom = WePackage::Instance()->DataStream.ReadFloat();
		//AlphaTo = WePackage::Instance()->DataStream.ReadFloat();
		//GrowFrom = WePackage::Instance()->DataStream.ReadFloat();
		//GrowTo = WePackage::Instance()->DataStream.ReadFloat();
		//WePackage::Instance()->DataStream.ReadVector(Gravity);
		//WePackage::Instance()->DataStream.ReadVector(SpawnParticleDirection);
		//StartVelocity = WePackage::Instance()->DataStream.ReadFloat();
		//Spread = WePackage::Instance()->DataStream.ReadFloat();
		//ParticlesLifeTime = WePackage::Instance()->DataStream.ReadFloat();
		//MaterialName = WePackage::Instance()->DataStream.ReadString();
		WePackage::Instance()->EndReading();
		
		WePackage::Instance()->PackageExtension = ".wep";
		return true;
	}else{
		WePackage::Instance()->PackageExtension = ".wep";
		return false;
	}
}	



bool WeParticleSystem::SavePreset(StringParam _Name, bool Replace)
{   
	WePackage::Instance()->PackageExtension = ".ppp";

	//if(Replace) WePackage::Instance()->Remove(_Name);

	if (WePackage::Instance()->BeginWriting(_Name,8, true))
	{
		Properties.Serialize(WePackage::Instance()->DataStream);        				
		WePackage::Instance()->EndWriting();   
		WePackage::Instance()->PackageExtension = ".wep";		
		return true;
	}else{
		WePackage::Instance()->PackageExtension = ".wep";
		return false;
	}
}	