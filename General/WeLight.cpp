/*
    WeLight.cpp

	Autor: Nebojsa Vislavski
	Date: 15.1.2006.
	Version: 1.0

	this is light class it contains information of light properties.


*/

#include <WeLight.h>
#include <WeTimer.h>
#include <WeMaterialManager.h>
#include <WeTextureManager.h>
#include <WeModelManager.h>
#include <WePhysics.h>

WeLight::WeLight()
{
	ModelName = "MapEditor.Geometry.Light";
	IsLight = true;
	Type = WETYPE_LIGHT;
	LightType = LIGHT_POINT;
	PrevLightType = LIGHT_POINT;
	LoopKeyStates = true;
	AnimationTime = 0;
	AnimationDuration = 0;
	NextLight = this;
	PrevLight = this;
	IsDynamic = false;
	AffectDynamicActors = false;
	CastShadowVolume = false;	
	InterpolateStates = false;

	LightVer = 12;
	Projector = false;
	ProjectorMaterial = NULL;
	ProjectorIsOmni = false;
	SizeX = 10.0f;
	SizeY = 10.0f;
	StartFalloff = 0;
	
	Range = 100;
	SpotAngle = D3DX_PI / 2.0f;
	SpotAngleFalloff = D3DX_PI / 2.0f;

	Corona = false;
	Flares = false;
	FlareSizeX = 0.5f;
	FlareSizeY = 0.5f;
	CoronaSizeX = 0.1f;
	CoronaSizeY = 0.1f;
	CoronaTexture = NULL;
	for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
		FlareTexture[i] = NULL;

	CoronaAlpha = 1.0f;
	for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
		FlareAlpha[i] = 1.0f;

	FadeInTime = 0.5;
	FadeOutTime = 0.5;

	CoronaFixedSize = false;
	CoronaRotation = false;
	AlwaysShowFlares = false;

	RenderLocation = WeVector(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&RenderTM);
}


void WeLight::Initialize()
{
	if (InEditor)
	{		
		if (LightType == LIGHT_STATIC_LINEAR)
			ModelName = "MapEditor.Geometry.LinearLight";
		else if (LightType == LIGHT_STATIC_AREA)
			ModelName = "MapEditor.Geometry.AreaLight";
		else if (LightType == LIGHT_STATIC_BEAM)
			ModelName = "MapEditor.Geometry.BeamLight";
		else
			ModelName = "MapEditor.Geometry.Light";


		PhysicModel.Type = PHYSICMODEL_STATIC_SHAPE;
		WeActor::Initialize();
	}
	else
	{
		Renderable = false; // just make it invisible
	}

	if (Projector)
	{		
		ProjectorMaterial = WeMaterialManager::Instance()->Load(ProjectorMaterialName);
	}

	if (Corona)
	{
		CoronaTexture = WeTextureManager::Instance()->Load(CoronaTextureName, true);
	}

	if (Flares)
	{
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareTexture[i] = WeTextureManager::Instance()->Load(FlareTextureNames[i], true);
	}

	LightStateIterator = LightStates.FirstI();
}

void WeLight::AddLightState(WeLightState *ls)
{
	LightStates.PushBack(*ls);
	AnimationDuration += ls->Duration;
}

void WeLight::RemoveLightState(WeLightState *ls)
{
	for (LightStates.First(); !LightStates.End(); LightStates.Next())
	{
		if (&LightStates.GetItem() == ls)
		{
			AnimationDuration -= LightStates.GetItem().Duration;
			LightStates.Delete();
			return;
		}
	}
}

void WeLight::Tick()
{
	AnimationTime += WeTimer::Instance()->GetDeltaTimeF();
	if (LoopKeyStates)	
	{
		if (AnimationTime >= AnimationDuration) AnimationTime = 0;
	}


	AnimationDuration = 0;
	for (LightStates.First(); !LightStates.End(); LightStates.Next())
	{
		AnimationDuration += LightStates.GetItem().Duration;
	}

	float dl;
	bool taken = false;
	dl = 0;
	for (LightStateIterator = LightStates.FirstI(); LightStateIterator != LightStates.EndI(); ++LightStateIterator)
	{
		if (taken)
		{
			NextDiffuse = LightStateIterator.GetItem().Diffuse;
			NextSpecular = LightStateIterator.GetItem().Specular;
			NextIntensity = LightStateIterator.GetItem().Intensity;
			NextSpecularIntensity = LightStateIterator.GetItem().SpecularIntensity;
			NextCoronaAlpha = LightStateIterator.GetItem().CoronaAlpha;
			for (int i = 0; i < LIGHT_FLARE_TEXTURES; ++i)
				NextFlareAlpha[i] = LightStateIterator.GetItem().FlareAlpha[i];
			taken = false;
			break;
		}

		dl += LightStateIterator.GetItem().Duration;
		if (dl > AnimationTime) 
		{	
			InterpolateTime = (AnimationTime - (dl - LightStateIterator.GetItem().Duration)) / LightStateIterator.GetItem().Duration;
			CurrentDiffuse = LightStateIterator.GetItem().Diffuse;
			CurrentSpecular = LightStateIterator.GetItem().Specular;
			CurrentIntensity = LightStateIterator.GetItem().Intensity;
			CurrentSpecularIntensity = LightStateIterator.GetItem().SpecularIntensity;
			CurrentCoronaAlpha = LightStateIterator.GetItem().CoronaAlpha;
			for (int i = 0; i < LIGHT_FLARE_TEXTURES; ++i)
				CurrentFlareAlpha[i] = LightStateIterator.GetItem().FlareAlpha[i];
			taken = true;
		}
	}

	if (taken)
	{
		for (LightStateIterator = LightStates.FirstI(); LightStateIterator != LightStates.EndI(); ++LightStateIterator)
		{			
			NextSpecularIntensity = LightStateIterator.GetItem().SpecularIntensity;
			NextSpecular = LightStateIterator.GetItem().Specular;
			NextDiffuse = LightStateIterator.GetItem().Diffuse;
			NextIntensity = LightStateIterator.GetItem().Intensity;
			NextCoronaAlpha = LightStateIterator.GetItem().CoronaAlpha;
			for (int i = 0; i < LIGHT_FLARE_TEXTURES; ++i)
				NextFlareAlpha[i] = LightStateIterator.GetItem().FlareAlpha[i];
			break;			
		}				
	}

	BSphere.Position = Location;
	BSphere.Radius = Range;


	if (InEditor)	
	{	
		if ((PrevLightType != LightType) || (PrevSizeX != SizeX) || (PrevSizeY != SizeY))
		{
			if (LightType == LIGHT_STATIC_LINEAR)
			{
				ModelName = "MapEditor.Geometry.LinearLight";
				Scale = WeVector(1.0, 1.0, SizeX);
				BSphere.Radius = SizeX;
			}
			else if (LightType == LIGHT_STATIC_AREA)
			{
				ModelName = "MapEditor.Geometry.AreaLight";
				Scale = WeVector(SizeX, 1.0, SizeY);
				if (SizeX > SizeY) BSphere.Radius = SizeX;
				else BSphere.Radius = SizeY;
			}
			else if (LightType == LIGHT_STATIC_BEAM)
			{
				ModelName = "MapEditor.Geometry.BeamLight";
				Scale = WeVector(SizeX, SizeX, Range);
				if (SizeX > Range) BSphere.Radius = SizeX;
				else BSphere.Radius = Range;
			}
			else
			{
				ModelName = "MapEditor.Geometry.Light";
				Scale = WeVector(1.0, 1.0, 1.0);
				BSphere.Radius = 2.0f;
			}

			SAFE_DELETE(Model);
			Model = WeModelManager::Instance()->Load(ModelName, false);
			if (Model)
				Model->ComputeBoundingBox(this);

			if (LightType == LIGHT_STATIC_BEAM)
			{
				WeVector v, tmpv;
				v = Scale / 2.0f;
				v.x *= 2.0f;
				v.y *= 2.0f;

				SAFE_DELETE(PhysicHandle);			
				WeMatrix tmpM;
				tmpM = TM;
				tmpv.x = TM._31; tmpv.y = TM._32; tmpv.z = TM._33;
				tmpv = Location + tmpv*Range/2.0f;
				tmpM._41 = tmpv.x; tmpM._42 = tmpv.y; tmpM._43 = tmpv.z;

				PhysicHandle = WePhysics::Instance()->CreateBox(this, tmpM, v, 0, 10, false, 0, false);		
				PhysicHandle->SetKinematic(true);			
				PrevLightType = LightType;
				PrevSizeX = SizeX;
				PrevSizeY = SizeY;
			}
			else
			{
				WeVector v;
				v = Scale / 2.0f;
				SAFE_DELETE(PhysicHandle);
				PhysicHandle = WePhysics::Instance()->CreateBox(this, TM, v, 0, 10, false, 0, false);
				PhysicHandle->SetKinematic(true);
				PrevLightType = LightType;
				PrevSizeX = SizeX;
				PrevSizeY = SizeY;
			}
		}
	}

	RenderTM = TM;

	static D3DXVECTOR3 ZeroV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVec3TransformCoord(&RenderLocation, &ZeroV, &RenderTM);

	WeActor::Tick();
}


bool WeLight::IntersectFrustum(WeFrustum *frustum)
{
	if (World)
	{
		if (!World->ShowActorTypes[-1 - WETYPE_LIGHT])
			return false;
	}

	return frustum->Collide(BSphere);
}


void WeLight::AddKeyState(WeLightState &LightState, float TimeS, bool LoopKeyStates_)
{
	LightState.Duration = TimeS;
	LoopKeyStates = LoopKeyStates_;
	LightStates.PushBack(LightState);
	AnimationDuration += TimeS;
}


WeColor WeLight::GetDiffuseColor()
{
	WeColor col;
	if (InterpolateStates)
	{
		D3DXColorLerp(&col, &CurrentDiffuse, &NextDiffuse, InterpolateTime);
		return col;
	} else
	{
		return CurrentDiffuse;
	}
}

WeColor WeLight::GetSpecularColor()
{
	WeColor col;
	if (InterpolateStates)
	{
		D3DXColorLerp(&col, &CurrentSpecular, &NextSpecular, InterpolateTime);
		return col;
	} else
	{
		return CurrentSpecular;
	}
}


float WeLight::GetCoronaAlpha()
{
	float intensity;
	if (InterpolateStates)
	{
		//char tmp[100];
		//sprintf(tmp, "CI: %3.3f, NI: %3.3f, IT: %3.3f", CurrentIntensity, NextIntensity, InterpolateTime);
		//MessageBox(0, tmp, 0, 0);
		intensity = CurrentCoronaAlpha + (NextCoronaAlpha - CurrentCoronaAlpha)*InterpolateTime;
		return intensity;
	} else
	{
		return CurrentCoronaAlpha;
	}
}

float WeLight::GetFlareAlpha(int index)
{
	float intensity;
	if (InterpolateStates)
	{
		//char tmp[100];
		//sprintf(tmp, "CI: %3.3f, NI: %3.3f, IT: %3.3f", CurrentIntensity, NextIntensity, InterpolateTime);
		//MessageBox(0, tmp, 0, 0);
		intensity = CurrentFlareAlpha[index] + (NextFlareAlpha[index] - CurrentFlareAlpha[index])*InterpolateTime;
		return intensity;
	} else
	{
		return CurrentFlareAlpha[index];
	}
}

float WeLight::GetSpecularIntensity()
{
	float intensity;
	if (InterpolateStates)
	{
		//char tmp[100];
		//sprintf(tmp, "CI: %3.3f, NI: %3.3f, IT: %3.3f", CurrentIntensity, NextIntensity, InterpolateTime);
		//MessageBox(0, tmp, 0, 0);
		intensity = CurrentSpecularIntensity + (NextSpecularIntensity - CurrentSpecularIntensity)*InterpolateTime;
		return intensity;
	} else
	{
		return CurrentSpecularIntensity;
	}

}


float WeLight::GetIntensity()
{
	float intensity;
	if (InterpolateStates)
	{
		//char tmp[100];
		//sprintf(tmp, "CI: %3.3f, NI: %3.3f, IT: %3.3f", CurrentIntensity, NextIntensity, InterpolateTime);
		//MessageBox(0, tmp, 0, 0);
		intensity = CurrentIntensity + (NextIntensity - CurrentIntensity)*InterpolateTime;
		return intensity;
	} else
	{
		return CurrentIntensity;
	}

}

WeLightState* WeLight::GetCurrentLightState()
{
	//WeLightState *Res;
	//float Dt = 0;

	return &LightStateIterator.GetItem();

	//LightStates.First();	
	//for (int i = 0; i < LightStates.GetItemsCount(); i++)
	//{
	//	Dt += LightStates.GetItem().Duration;
	//	if (Dt > AnimationTime)
	//	{
	//		return &LightStates.GetItem();
	//	}		
	//	LightStates.Next();
	//}
 //   // else return last element
	//LightStates.Last();
	//return &LightStates.GetItem();
}


WeLight::~WeLight()
{
	WeFlareRenderer::Instance()->RemoveLight(this);
//	MessageBox(0, "OVDE SAM", 0, 0);
//	LightStates.Clear();
	//MessageBox(0, "OVDE SAM", 0, 0);
}

void WeLight::Clone(WeActor *Destination, bool DeepCopy)
{
	WeActor::Clone(Destination);

	WeLight *tmpL;
	tmpL = (WeLight *)Destination;

	tmpL->IsDynamic = IsDynamic;
	tmpL->LightType = LightType;
	tmpL->LoopKeyStates = LoopKeyStates;
	tmpL->AnimationTime = AnimationTime;
	tmpL->AnimationDuration = AnimationDuration;
	tmpL->AffectDynamicActors = AffectDynamicActors;
	tmpL->StartFalloff = StartFalloff;

	tmpL->Projector = Projector;
	tmpL->ProjectorMaterial = ProjectorMaterial;
	tmpL->ProjectorMaterialName = ProjectorMaterialName;
	tmpL->ProjectorIsOmni = ProjectorIsOmni;

	tmpL->SpotAngle = SpotAngle;
	tmpL->SpotAngleFalloff = SpotAngleFalloff;
	tmpL->Range = Range;
	tmpL->InterpolateStates = InterpolateStates;
	tmpL->BSphere = BSphere;
	tmpL->Flares = Flares;
	tmpL->Corona = Corona;
	tmpL->CoronaSizeX = CoronaSizeX;
	tmpL->CoronaSizeY = CoronaSizeY;
	tmpL->FlareSizeX = FlareSizeX;
	tmpL->FlareSizeY = FlareSizeY;
	tmpL->CoronaTextureName = CoronaTextureName;

	tmpL->SizeX = SizeX;
	tmpL->SizeY = SizeY;	

	for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
		tmpL->FlareTextureNames[i] = FlareTextureNames[i];

	tmpL->CoronaAlpha = CoronaAlpha;
	for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
		tmpL->FlareAlpha[i] = FlareAlpha[i];

	WeLightState ls;
	tmpL->LightStates.Clear();
	for (LightStates.First(); !LightStates.End(); LightStates.Next())
	{
		ls = LightStates.GetItem();
		tmpL->LightStates.PushBack(ls);
	}

	tmpL->FadeInTime = FadeInTime;
	tmpL->FadeOutTime = FadeOutTime;

	tmpL->CoronaFixedSize = CoronaFixedSize;
	tmpL->CoronaRotation = CoronaRotation;
	tmpL->AlwaysShowFlares = AlwaysShowFlares;
}


void WeLight::Serialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;
	WeActor::Serialize(FS);

	FS.WriteInt(LightVer);
	FS.WriteBool(IsDynamic);
	FS.WriteInt(LightType);
	FS.WriteBool(LoopKeyStates);
	FS.WriteBool(AffectDynamicActors);

	FS.WriteFloat(Range);
	FS.WriteFloat(SpotAngle);
	FS.WriteFloat(SpotAngleFalloff);
	FS.WriteBool(InterpolateStates);

	FS.WriteBool(Projector);
	FS.WriteBool(ProjectorIsOmni);	
	FS.WriteString(ProjectorMaterialName);

	FS.WriteBool(Corona);
	FS.WriteBool(Flares);
	FS.WriteFloat(CoronaSizeX);
	FS.WriteFloat(CoronaSizeY);
	FS.WriteFloat(FlareSizeX);
	FS.WriteFloat(FlareSizeY);
	FS.WriteString(CoronaTextureName);
	for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
		FS.WriteString(FlareTextureNames[i]);


	//FS.WriteFloat(CoronaAlpha);
	//for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
	//	FS.WriteFloat(FlareAlpha[i]);

	FS.WriteFloat(FadeInTime);
	FS.WriteFloat(FadeOutTime);

	FS.WriteInt(LightStates.GetItemsCount());
	for (LightStates.First(); !LightStates.End(); LightStates.Next())
	{
		LightStates.GetItem().Serialize(FS);
	}

	FS.WriteBool(CoronaFixedSize);
	FS.WriteBool(CoronaRotation);
	FS.WriteBool(AlwaysShowFlares);

	FS.WriteFloat(SizeX);
	FS.WriteFloat(SizeY);
	FS.WriteFloat(StartFalloff);
}



void WeLight::Deserialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	WeActor::Deserialize(FS);

	int v;
	v = FS.ReadInt();

	if (v > 100 || v < 2) 
	{
		FS.Seek(-4, SEEK_CUR);
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();
		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;

			FS.ReadVector(LS.DeltaLocation);
			FS.ReadVector(LS.Direction);
			LS.Diffuse.r = FS.ReadFloat();
			LS.Diffuse.g = FS.ReadFloat();
			LS.Diffuse.b = FS.ReadFloat();
			LS.Diffuse.a = FS.ReadFloat();

			LS.Specular.r = FS.ReadFloat();
			LS.Specular.g = FS.ReadFloat();
			LS.Specular.b = FS.ReadFloat();
			LS.Specular.a = FS.ReadFloat();

			
			LS.Range = FS.ReadFloat();
			Range = LS.Range;
			LS.StartFalloffRange = FS.ReadFloat();
			LS.Duration = FS.ReadFloat();
			LightStates.PushBack(LS);
		}
	} // end of non-versioned lights
	else if (v == 2)
	{
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();
		Projector = FS.ReadBool();
		ProjectorMaterialName = FS.ReadString();

		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;

			FS.ReadVector(LS.DeltaLocation);
			FS.ReadVector(LS.Direction);
			LS.Diffuse.r = FS.ReadFloat();
			LS.Diffuse.g = FS.ReadFloat();
			LS.Diffuse.b = FS.ReadFloat();
			LS.Diffuse.a = FS.ReadFloat();

			LS.Specular.r = FS.ReadFloat();
			LS.Specular.g = FS.ReadFloat();
			LS.Specular.b = FS.ReadFloat();
			LS.Specular.a = FS.ReadFloat();

			LS.Range = FS.ReadFloat();
			Range = LS.Range;
			LS.StartFalloffRange = FS.ReadFloat();
			LS.Duration = FS.ReadFloat();
			LightStates.PushBack(LS);
		}
	} // end of version 2
	else if (v == 3)
	{
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();

		Range = FS.ReadFloat();
		SpotAngle = FS.ReadFloat();
		SpotAngleFalloff = FS.ReadFloat();
		InterpolateStates = FS.ReadBool();

		Projector = FS.ReadBool();
		ProjectorIsOmni = FS.ReadBool();
		ProjectorMaterialName = FS.ReadString();

		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;

			FS.ReadColor(LS.Diffuse);
			FS.ReadColor(LS.Specular);
			LS.Duration = FS.ReadFloat();
			LS.Intensity = FS.ReadFloat();
			LightStates.PushBack(LS);
		}
	} // end of version 3
	else if (v == 4)
	{
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();

		Range = FS.ReadFloat();
		SpotAngle = FS.ReadFloat();
		SpotAngleFalloff = FS.ReadFloat();
		InterpolateStates = FS.ReadBool();

		Projector = FS.ReadBool();
		ProjectorIsOmni = FS.ReadBool();
		ProjectorMaterialName = FS.ReadString();

		Corona = FS.ReadBool();
		Flares = FS.ReadBool();
		CoronaSizeX = FS.ReadFloat();
		CoronaSizeY = FS.ReadFloat();
		FlareSizeX = FS.ReadFloat();
		FlareSizeY = FS.ReadFloat();
		CoronaTextureName = FS.ReadString();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareTextureNames[i] = FS.ReadString();

		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;

			FS.ReadColor(LS.Diffuse);
			FS.ReadColor(LS.Specular);
			LS.Specular.r = 1.0f;
			LS.Specular.g = 1.0f;
			LS.Specular.b = 1.0f;
			LS.Specular.a = 1.0f;

			LS.Duration = FS.ReadFloat();
			LS.Intensity = FS.ReadFloat();
			LightStates.PushBack(LS);
		}
	} // end of version 4

	else if (v == 5)
	{
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();

		Range = FS.ReadFloat();
		SpotAngle = FS.ReadFloat();
		SpotAngleFalloff = FS.ReadFloat();
		InterpolateStates = FS.ReadBool();

		Projector = FS.ReadBool();
		ProjectorIsOmni = FS.ReadBool();
		ProjectorMaterialName = FS.ReadString();

		Corona = FS.ReadBool();
		Flares = FS.ReadBool();
		CoronaSizeX = FS.ReadFloat();
		CoronaSizeY = FS.ReadFloat();
		FlareSizeX = FS.ReadFloat();
		FlareSizeY = FS.ReadFloat();
		CoronaTextureName = FS.ReadString();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareTextureNames[i] = FS.ReadString();

		// new in version 5
		CoronaAlpha = FS.ReadFloat();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareAlpha[i] = FS.ReadFloat();
		// end of new in version 5

		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;

			FS.ReadColor(LS.Diffuse);
			FS.ReadColor(LS.Specular);
			LS.Specular.r = 1.0f;
			LS.Specular.g = 1.0f;
			LS.Specular.b = 1.0f;
			LS.Specular.a = 1.0f;

			LS.Duration = FS.ReadFloat();
			LS.Intensity = FS.ReadFloat();
			LightStates.PushBack(LS);
		}
	} // end of version 5

	else if (v == 6)
	{
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();

		Range = FS.ReadFloat();
		SpotAngle = FS.ReadFloat();
		SpotAngleFalloff = FS.ReadFloat();
		InterpolateStates = FS.ReadBool();

		Projector = FS.ReadBool();
		ProjectorIsOmni = FS.ReadBool();
		ProjectorMaterialName = FS.ReadString();

		Corona = FS.ReadBool();
		Flares = FS.ReadBool();
		CoronaSizeX = FS.ReadFloat();
		CoronaSizeY = FS.ReadFloat();
		FlareSizeX = FS.ReadFloat();
		FlareSizeY = FS.ReadFloat();
		CoronaTextureName = FS.ReadString();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareTextureNames[i] = FS.ReadString();

		// new in version 5
		CoronaAlpha = FS.ReadFloat();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareAlpha[i] = FS.ReadFloat();
		// end of new in version 5

		// new in version 6
		FadeInTime = FS.ReadFloat();
		FadeOutTime = FS.ReadFloat();
		// end of new in version 6

		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;
			LS.Deserialize(FS);
			AnimationDuration += LS.Duration;
			LightStates.PushBack(LS);
		}
	} // end of version 6

	else if (v == 7)
	{
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();

		Range = FS.ReadFloat();
		SpotAngle = FS.ReadFloat();
		SpotAngleFalloff = FS.ReadFloat();
		InterpolateStates = FS.ReadBool();

		Projector = FS.ReadBool();
		ProjectorIsOmni = FS.ReadBool();
		ProjectorMaterialName = FS.ReadString();

		Corona = FS.ReadBool();
		Flares = FS.ReadBool();
		CoronaSizeX = FS.ReadFloat();
		CoronaSizeY = FS.ReadFloat();
		FlareSizeX = FS.ReadFloat();
		FlareSizeY = FS.ReadFloat();
		CoronaTextureName = FS.ReadString();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareTextureNames[i] = FS.ReadString();

		// new in version 5
		CoronaAlpha = FS.ReadFloat();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareAlpha[i] = FS.ReadFloat();
		// end of new in version 5

		// new in version 6
		FadeInTime = FS.ReadFloat();
		FadeOutTime = FS.ReadFloat();
		// end of new in version 6

		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;
			LS.Deserialize(FS);
			AnimationDuration += LS.Duration;
			LightStates.PushBack(LS);
		}

		CoronaFixedSize = FS.ReadBool();
	} // end of version 7
	else if (v == 8)
	{
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();

		Range = FS.ReadFloat();
		SpotAngle = FS.ReadFloat();
		SpotAngleFalloff = FS.ReadFloat();
		InterpolateStates = FS.ReadBool();

		Projector = FS.ReadBool();
		ProjectorIsOmni = FS.ReadBool();
		ProjectorMaterialName = FS.ReadString();

		Corona = FS.ReadBool();
		Flares = FS.ReadBool();
		CoronaSizeX = FS.ReadFloat();
		CoronaSizeY = FS.ReadFloat();
		FlareSizeX = FS.ReadFloat();
		FlareSizeY = FS.ReadFloat();
		CoronaTextureName = FS.ReadString();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareTextureNames[i] = FS.ReadString();

		// new in version 5
		CoronaAlpha = FS.ReadFloat();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareAlpha[i] = FS.ReadFloat();
		// end of new in version 5

		// new in version 6
		FadeInTime = FS.ReadFloat();
		FadeOutTime = FS.ReadFloat();
		// end of new in version 6

		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;
			LS.Deserialize(FS);
			AnimationDuration += LS.Duration;
			LightStates.PushBack(LS);
		}

		CoronaFixedSize = FS.ReadBool();
		SizeX = FS.ReadFloat();
		SizeY = FS.ReadFloat();
	} // end of version 9
	else if (v == 9)
	{
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();

		Range = FS.ReadFloat();
		SpotAngle = FS.ReadFloat();
		SpotAngleFalloff = FS.ReadFloat();
		InterpolateStates = FS.ReadBool();

		Projector = FS.ReadBool();
		ProjectorIsOmni = FS.ReadBool();
		ProjectorMaterialName = FS.ReadString();

		Corona = FS.ReadBool();
		Flares = FS.ReadBool();
		CoronaSizeX = FS.ReadFloat();
		CoronaSizeY = FS.ReadFloat();
		FlareSizeX = FS.ReadFloat();
		FlareSizeY = FS.ReadFloat();
		CoronaTextureName = FS.ReadString();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareTextureNames[i] = FS.ReadString();

		// new in version 5
		CoronaAlpha = FS.ReadFloat();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareAlpha[i] = FS.ReadFloat();
		// end of new in version 5

		// new in version 6
		FadeInTime = FS.ReadFloat();
		FadeOutTime = FS.ReadFloat();
		// end of new in version 6

		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;
			LS.Deserialize(FS);
			AnimationDuration += LS.Duration;
			LightStates.PushBack(LS);
		}

		CoronaFixedSize = FS.ReadBool();
		SizeX = FS.ReadFloat();
		SizeY = FS.ReadFloat();
		StartFalloff = FS.ReadFloat();

	} // end of version 9
	else if (v == 10)
	{
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();

		Range = FS.ReadFloat();
		SpotAngle = FS.ReadFloat();
		SpotAngleFalloff = FS.ReadFloat();
		InterpolateStates = FS.ReadBool();

		Projector = FS.ReadBool();
		ProjectorIsOmni = FS.ReadBool();
		ProjectorMaterialName = FS.ReadString();

		Corona = FS.ReadBool();
		Flares = FS.ReadBool();
		CoronaSizeX = FS.ReadFloat();
		CoronaSizeY = FS.ReadFloat();
		FlareSizeX = FS.ReadFloat();
		FlareSizeY = FS.ReadFloat();
		CoronaTextureName = FS.ReadString();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareTextureNames[i] = FS.ReadString();

		// new in version 5 - removed in version 10
		//CoronaAlpha = FS.ReadFloat();
		//for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
		//	FlareAlpha[i] = FS.ReadFloat();
		// end of new in version 5

		// new in version 6
		FadeInTime = FS.ReadFloat();
		FadeOutTime = FS.ReadFloat();
		// end of new in version 6

		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;
			LS.Deserialize(FS);
			AnimationDuration += LS.Duration;
			LightStates.PushBack(LS);
		}

		CoronaFixedSize = FS.ReadBool();
		SizeX = FS.ReadFloat();
		SizeY = FS.ReadFloat();
		StartFalloff = FS.ReadFloat();

	} // end of version 10
	else if (v == 11)
	{
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();

		Range = FS.ReadFloat();
		SpotAngle = FS.ReadFloat();
		SpotAngleFalloff = FS.ReadFloat();
		InterpolateStates = FS.ReadBool();

		Projector = FS.ReadBool();
		ProjectorIsOmni = FS.ReadBool();
		ProjectorMaterialName = FS.ReadString();

		Corona = FS.ReadBool();
		Flares = FS.ReadBool();
		CoronaSizeX = FS.ReadFloat();
		CoronaSizeY = FS.ReadFloat();
		FlareSizeX = FS.ReadFloat();
		FlareSizeY = FS.ReadFloat();
		CoronaTextureName = FS.ReadString();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareTextureNames[i] = FS.ReadString();

		// new in version 5 - removed in version 10
		//CoronaAlpha = FS.ReadFloat();
		//for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
		//	FlareAlpha[i] = FS.ReadFloat();
		// end of new in version 5

		// new in version 6
		FadeInTime = FS.ReadFloat();
		FadeOutTime = FS.ReadFloat();
		// end of new in version 6

		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;
			LS.Deserialize(FS);
			AnimationDuration += LS.Duration;
			LightStates.PushBack(LS);
		}

		CoronaFixedSize = FS.ReadBool();
		CoronaRotation = FS.ReadBool();

		SizeX = FS.ReadFloat();
		SizeY = FS.ReadFloat();
		StartFalloff = FS.ReadFloat();

	} // end of version 11
	else if (v == 12)
	{
		IsDynamic = FS.ReadBool();
		LightType = (WeLightType)FS.ReadInt();
		LoopKeyStates = FS.ReadBool();
		AffectDynamicActors = FS.ReadBool();

		Range = FS.ReadFloat();
		SpotAngle = FS.ReadFloat();
		SpotAngleFalloff = FS.ReadFloat();
		InterpolateStates = FS.ReadBool();

		Projector = FS.ReadBool();
		ProjectorIsOmni = FS.ReadBool();
		ProjectorMaterialName = FS.ReadString();

		Corona = FS.ReadBool();
		Flares = FS.ReadBool();
		CoronaSizeX = FS.ReadFloat();
		CoronaSizeY = FS.ReadFloat();
		FlareSizeX = FS.ReadFloat();
		FlareSizeY = FS.ReadFloat();
		CoronaTextureName = FS.ReadString();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			FlareTextureNames[i] = FS.ReadString();

		// new in version 5 - removed in version 10
		//CoronaAlpha = FS.ReadFloat();
		//for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
		//	FlareAlpha[i] = FS.ReadFloat();
		// end of new in version 5

		// new in version 6
		FadeInTime = FS.ReadFloat();
		FadeOutTime = FS.ReadFloat();
		// end of new in version 6

		int Size = FS.ReadInt();
		for (int i = 0; i < Size; i++)
		{
			WeLightState LS;
			LS.Deserialize(FS);
			AnimationDuration += LS.Duration;
			LightStates.PushBack(LS);
		}

		CoronaFixedSize = FS.ReadBool();
		CoronaRotation = FS.ReadBool();
		AlwaysShowFlares = FS.ReadBool();

		SizeX = FS.ReadFloat();
		SizeY = FS.ReadFloat();
		StartFalloff = FS.ReadFloat();

	} // end of version 12
}


String WeLight::GetInfo()
{
	if (IsDynamic)
		return "<DYNAMIC>";
	else
		return "";
}
