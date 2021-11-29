/*
       File: WeShaderManager.cpp
	   Author: Tomislav Kukic
	   Date: 12.09.2005.
	   Version: 1.0
*/



#include <WeShader.h>
#include <WeShaderManager.h>
#include <WeShader.h>
#include <WeEngine.h>
#include <WeLoger.h>



WeShaderManager* WeShaderManager::_instance = NULL;


WeShaderManager* WeShaderManager::Instance()
{
	if(_instance) return _instance;
	_instance = new WeShaderManager();
	return _instance;

}



WeShaderManager::WeShaderManager(void)
{
	DefaultShader = NULL;
	bInitialized = false;
	Quality = SHADER_LOW;
}



WeShaderManager::~WeShaderManager(void)
{
	SAFE_RELEASE(EffectPool);
	FreeAll();
}



void WeShaderManager::Free(WeShader* Shader)
{
	for(Shaders.First(); !Shaders.End(); Shaders.Next())
	{
		if(Shaders.GetItem() == Shader)
		{
			delete Shaders.GetItem();
			Shaders.Delete();
			return;
		}
	}
}


void WeShaderManager::FreeAll()
{
	for(Shaders.First(); !Shaders.End(); Shaders.Next())
		delete Shaders.GetItem();

	Shaders.Clear();

	// Delete Default Shader
	//if (DefaultShader) 
	//{
	//	delete DefaultShader;
	//	DefaultShader = NULL;
	//}
}

void WeShaderManager::Initialize(StringParam DefaultShaderName)
{
	if (bInitialized) return;

	if (FAILED(D3DXCreateEffectPool(&EffectPool)))
	{
		return;
	}

	if (!DefaultShader) 
	{
		DefaultShader = new WeShader();
		if (!DefaultShader->Load(DefaultShaderName))
		{
			if (!DefaultShader->Load(DefaultShaderName, false))
			{
				return;
			}
		}
	}	

	WorldMatrix = DefaultShader->GetEffect()->GetParameterByName(0, "WorldMatrix");
	PrevWorldMatrix = DefaultShader->GetEffect()->GetParameterByName(0, "PrevWorldMatrix");
	ViewMatrix = DefaultShader->GetEffect()->GetParameterByName(0, "ViewMatrix");
	ProjectionMatrix =  DefaultShader->GetEffect()->GetParameterByName(0, "Projection");
	ViewProjection = DefaultShader->GetEffect()->GetParameterByName(0, "ViewProjection");
	PrevViewProjection = DefaultShader->GetEffect()->GetParameterByName(0, "PrevViewProjection");
	DeltaTime = DefaultShader->GetEffect()->GetParameterByName(0, "DeltaTime");
	ElapsedTime = DefaultShader->GetEffect()->GetParameterByName(0, "ElapsedTime");
	EyePosition = DefaultShader->GetEffect()->GetParameterByName(0, "EyePosition");
	ResWidth = DefaultShader->GetEffect()->GetParameterByName(0, "ResWidth");
	ResHeight = DefaultShader->GetEffect()->GetParameterByName(0, "ResHeight");
	MaterialColor = DefaultShader->GetEffect()->GetParameterByName(0, "MaterialColor");

	//PointLightCount = DefaultShader->GetEffect()->GetParameterByName(0, "PointLightCount");
	//PointLightPosition = DefaultShader->GetEffect()->GetParameterElement("PointLightPosition", 0);
	//PointLightColor= DefaultShader->GetEffect()->GetParameterElement("PointLightColor", 0);
	//PointLightSpecular = DefaultShader->GetEffect()->GetParameterElement("PointLightSpecular",0);
	//PointLightRange = DefaultShader->GetEffect()->GetParameterElement("PointLightRange", 0);
	LightEnabled = DefaultShader->GetEffect()->GetParameterByName(0, "LightEnabled");


	bInitialized = true;

}


bool WeShaderManager::SetQuality(WeShaderQuality quality)
{
    bool ok = true, b = false;
	Quality = quality;
	for (Shaders.First(); !Shaders.End(); Shaders.Next())
	{
		b = Shaders.GetItem()->SetQuality(Quality);
		if (!b) ok = false;
	}
	return ok;
}


WeShader* WeShaderManager::Load(StringParam name, bool ForceReload, bool FromPackage)
{
	WeShader* newShader = NULL;
	bool isNew = false;
	if (!bInitialized)
	{
		return NULL;
	}

	if(!ForceReload)
	{
		for(Shaders.First(); !Shaders.End(); Shaders.Next())
		{
			if(Shaders.GetItem()->Name == name)
			{
				return Shaders.GetItem();
			}
		}
	}

	if (!newShader)
	{
		newShader = new WeShader();
		isNew = true;
	}

	if (newShader->Load(name, !FromPackage))
	{
		newShader->SetQuality(Quality);
		if (isNew)
		{
			Shaders.PushBack(newShader);
		}
		return newShader;
	}
	else
	{
		if (isNew)
		{
			delete newShader;
		}
	}

	// Load default shader if not loaded
	if (DefaultShader)
	{
		return DefaultShader;
	}

	DefaultShader->Load(WeEngine::Instance()->Desc.DefaultShaderName);
	return DefaultShader;
}


void WeShaderManager::SetWorldMatrix(WeMatrix &World)
{
	if (bInitialized)
	DefaultShader->GetEffect()->SetMatrix(WorldMatrix, &World);
}

void WeShaderManager::SetPrevWorldMatrix(WeMatrix &PrevWorld)
{
	if (bInitialized)
	DefaultShader->GetEffect()->SetMatrix(PrevWorldMatrix, &PrevWorld);
}


void WeShaderManager::SetViewMatrix(WeMatrix &View)
{
	if (bInitialized)
	DefaultShader->GetEffect()->SetMatrix(ViewMatrix, &View);
}

void WeShaderManager::SetProjectionMatrix(WeMatrix &Projection)
{
	if (bInitialized)
	DefaultShader->GetEffect()->SetMatrix(ProjectionMatrix, &Projection);
}


void WeShaderManager::SetViewProjection(WeMatrix &viewProj)
{
	if (bInitialized)
	DefaultShader->GetEffect()->SetMatrix(ViewProjection, &viewProj);
}

void WeShaderManager::SetPrevViewProjection(WeMatrix &prevviewProj)
{
	if (bInitialized)
	DefaultShader->GetEffect()->SetMatrix(PrevViewProjection, &prevviewProj);
}


void WeShaderManager::SetEyePosition(WeVector &eyeposition)
{
	if (bInitialized)
		DefaultShader->GetEffect()->SetFloatArray(EyePosition, (float *)&eyeposition, 3);
}

void WeShaderManager::SetMaterialColor(WeColor &MatColor)
{
	if (bInitialized)
		DefaultShader->GetEffect()->SetFloatArray(MaterialColor, (float *)&MatColor, 4);
}




void WeShaderManager::SetDeltaTime(float deltatime)
{
	if (bInitialized)
		DefaultShader->GetEffect()->SetFloat(DeltaTime, deltatime);
}

void WeShaderManager::SetElapsedTime(float elapsed)
{
	if (bInitialized)
		DefaultShader->GetEffect()->SetFloat(ElapsedTime, elapsed);
}

void WeShaderManager::SetResWidth(float Width)
{
	if (bInitialized)
		DefaultShader->GetEffect()->SetFloat(ResWidth, Width);
}

void WeShaderManager::SetResHeight(float Height)
{
	if (bInitialized)
		DefaultShader->GetEffect()->SetFloat(ResHeight, Height);
}


void WeShaderManager::OnLostDevice()
{
	for(Shaders.First(); !Shaders.End(); Shaders.Next())
	{
		if (Shaders.GetItem()->Effect)
			Shaders.GetItem()->Effect->OnLostDevice();
	}

	DefaultShader->Effect->OnLostDevice();
}



void WeShaderManager::OnResetDevice()
{
	for(Shaders.First(); !Shaders.End(); Shaders.Next())
	{
		if (Shaders.GetItem()->Effect)
			Shaders.GetItem()->Effect->OnResetDevice();
	}

	DefaultShader->Effect->OnResetDevice();
}


void WeShaderManager::SetLights(bool Enable)
{
	DefaultShader->Effect->SetBool(LightEnabled, Enable);


	/*int pi = 0;
	WeVector Loc(0,0,0);
	D3DXVECTOR4 tmpV(0,0,0,0);

	if (Lights.Empty())
	{		
		Loc = WeVector(0,0,0);
		tmpV = D3DXVECTOR4(0,0,0,0);
		DefaultShader->Effect->SetFloatArray(PointLightPosition, (float *)&Loc, 3);
		DefaultShader->Effect->SetFloat(PointLightRange, 0);
		DefaultShader->Effect->SetVector(PointLightColor, &tmpV);
		DefaultShader->Effect->SetVector(PointLightSpecular, &tmpV);		
	}

	for (Lights.First(); !Lights.End(); Lights.Next())
	{
		if (Lights.GetItem()->LightType == LIGHT_POINT)
		{
			Loc = Lights.GetItem()->Location + Lights.GetItem()->GetCurrentLightState()->DeltaLocation;
			DefaultShader->Effect->SetFloatArray(PointLightPosition, (float *)&Loc, 	3);

			DefaultShader->Effect->SetFloat(PointLightRange, Lights.GetItem()->GetCurrentLightState()->Range);

			tmpV.x = Lights.GetItem()->GetCurrentLightState()->Diffuse.r;
			tmpV.y = Lights.GetItem()->GetCurrentLightState()->Diffuse.g;
			tmpV.z = Lights.GetItem()->GetCurrentLightState()->Diffuse.b;
			tmpV.w = Lights.GetItem()->GetCurrentLightState()->Diffuse.a;
			DefaultShader->Effect->SetVector(PointLightColor, &tmpV);

			tmpV.x = Lights.GetItem()->GetCurrentLightState()->Specular.r;
			tmpV.y = Lights.GetItem()->GetCurrentLightState()->Specular.g;
			tmpV.z = Lights.GetItem()->GetCurrentLightState()->Specular.b;
			tmpV.w = Lights.GetItem()->GetCurrentLightState()->Specular.a;
			DefaultShader->Effect->SetVector(PointLightSpecular, &tmpV);

			pi++;
		}
	}
	DefaultShader->Effect->SetInt(PointLightCount, pi);*/

}


//WeMatrix& WeShaderManager::GetWorldMatrix()
//{
//	WeMatrix Mat;
//	if (bInitialized) 
//		DefaultShader->GetEffect()->GetMatrix(WorldMatrix, &Mat);
//	return Mat;
//}
//
//WeMatrix& WeShaderManager::GetViewProjection()
//{
//	WeMatrix Mat;
//	if (bInitialized) 
//		DefaultShader->GetEffect()->GetMatrix(ViewProjection, &Mat);
//	return Mat;
//}
