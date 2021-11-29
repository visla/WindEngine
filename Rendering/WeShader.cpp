/*
      File: WeShader.cpp
	  Author: Tomislav Kukic
	  Date: 12.09.2005.
	  Version: 1.0
*/



#include <WeRenderDevice.h>
#include <WeShader.h>
#include <WePackage.h>
#include <WeShaderManager.h>
#include <WeMaterial.h>
#include <WeProfiler.h>
#include <WeTextureManager.h>
#include <WeTexture.h>


WeShader::WeShader(void)
{
	Loaded = false;
	Effect = NULL;
	PassesCount = 0;
	WantLightsCount = 0;
	WantSumLightColor = 0;
	WantDirectLightsCount = 0;	
	AmbientTechnique = 0;
}


WeShader::~WeShader(void)
{
	SAFE_RELEASE(Effect);
}



	
void WeShader::Begin()
{
	if(!Effect) return;
	if (!WeRenderDevice::Instance()->GetUsingShaders()) return;		

	WeProfiler::Instance()->ShaderBegins++;
	WeProfiler::Instance()->ShadersActive.PushBack(Name);					
	Effect->Begin(&PassesCount, 0);
}


void WeShader::End()
{
	if(!Effect) return;
	if (!WeRenderDevice::Instance()->GetUsingShaders()) return;

	Effect->End();	
}


void WeShader::BeginPass(UINT PassNum)
{
	if(!Effect) return;
	if (!WeRenderDevice::Instance()->GetUsingShaders()) return;

	Effect->BeginPass(PassNum);
}


void WeShader::EndPass()
{
	if (!Effect) return;
	if (!WeRenderDevice::Instance()->GetUsingShaders()) return;

	Effect->EndPass();
}

void WeShader::CommitChanges()
{
	if (!Effect) return;
	if (!WeRenderDevice::Instance()->GetUsingShaders()) return;

	Effect->CommitChanges();
}


void WeShader::SetAlphaBlending()
{
}


void WeShader::SetBlendingAdd()
{
}


void WeShader::ClearBlending()
{
}


bool WeShader::Load(StringParam name, bool IsFxFile)
{
	SAFE_RELEASE(Effect);

	if (!IsFxFile)
	{
		BYTE* Buffer = NULL;
		DWORD Size;

		if(WePackage::Instance()->BeginReading(name, 3))
		{
			Size = WePackage::Instance()->DataStream.ReadDWord();
			Buffer = new BYTE[Size];
			WePackage::Instance()->DataStream.ReadData((void*)Buffer, (int)Size);
		}
		else
		{ 
			Effect = NULL; 
			return false; 
		}
		WePackage::Instance()->EndReading();


		if (FAILED(D3DXCreateEffect(WeRenderDevice::Instance()->Device, Buffer, Size, NULL, NULL, NULL, WeShaderManager::Instance()->EffectPool, &Effect, NULL)))
		{
			delete[] Buffer;
			Effect = NULL;
			return false;
		}
		delete[] Buffer;

	}
	else
	{
		if (FAILED(D3DXCreateEffectFromFile(WeRenderDevice::Instance()->Device, name, NULL, NULL, NULL, WeShaderManager::Instance()->EffectPool, &Effect, NULL)))
		{
			Effect = NULL;
			return false;
		}	
	}

	Name = name;



	// Get Annotation of how many lights it wants to receive
	WantLightsCount = 0;
	D3DXHANDLE tmpHandle;
	
/*	if (WeShaderManager::Instance()->GetQuality() == SHADER_LOW)		
		Effect->SetTechnique("Low");
	if (WeShaderManager::Instance()->GetQuality() == SHADER_MEDIUM)		
		Effect->SetTechnique("Medium");
	if (WeShaderManager::Instance()->GetQuality() == SHADER_HIGH)		
		Effect->SetTechnique("High");
		*/


	tmpHandle = Effect->GetParameterByName(NULL, "WantLightsCount");
	if (tmpHandle)
	{
		Effect->GetInt(tmpHandle, &WantLightsCount);
	} 		
	tmpHandle = Effect->GetParameterByName(NULL, "WantDirectLightsCount");
	if (tmpHandle)
	{
		Effect->GetInt(tmpHandle, &WantDirectLightsCount);
	} 		


	TextureAnimation = Effect->GetParameterByName(NULL, "TextureAnimation");	
	ResWidth = Effect->GetParameterByName(NULL, "ResWidth");	
	ResHeight = Effect->GetParameterByName(NULL, "ResHeight");	
	BloomScale = Effect->GetParameterByName(NULL, "BloomScale");	
	Brightness = Effect->GetParameterByName(NULL, "Brightness");	
	BrightnessColor = Effect->GetParameterByName(NULL, "BrightnessColor");	
	BumpIntensity = Effect->GetParameterByName(NULL, "BumpIntensity");	
	SpecularPow = Effect->GetParameterByName(NULL, "SpecularPow");	
	Param1 = Effect->GetParameterByName(NULL, "Param1");	
	Param2 = Effect->GetParameterByName(NULL, "Param2");	
	Param3 = Effect->GetParameterByName(NULL, "Param3");	
	Param4 = Effect->GetParameterByName(NULL, "Param4");	
	ProjectorPosition = Effect->GetParameterByName(NULL, "ProjectorPosition");
	ProjectorMatrix =  Effect->GetParameterByName(NULL, "ProjectorMatrix");
	AmbientColor = Effect->GetParameterByName(NULL, "AmbientColor");
	ShadowOmniLightPosition = Effect->GetParameterByName(NULL, "ShadowOmniLightPosition");
	UsingAlpha = Effect->GetParameterByName(NULL, "UsingAlpha");
	SkyColor = Effect->GetParameterByName(NULL, "SkyColor");
	GroundColor = Effect->GetParameterByName(NULL, "GroundColor");
	Scaling = Effect->GetParameterByName(NULL, "Scaling");
	PixelMotionBlurFactor = Effect->GetParameterByName(NULL, "PixelMotionBlurFactor");

	AmbientTechnique = Effect->GetTechniqueByName("Ambient");
	DefaultTechnique = Effect->GetTechniqueByName("Default");
	RefractiveTechnique = Effect->GetTechniqueByName("Refractive");
	

	// TODO: This is just because of older shaders that are already written
	if (!DefaultTechnique) 
	{
		DefaultTechnique = Effect->GetTechniqueByName("Low");
	}
	
	DiffusePointLowTechnique = Effect->GetTechniqueByName("DiffusePointLow");
	DiffusePointHighTechnique = Effect->GetTechniqueByName("DiffusePointHigh");
	BumpPointLowTechnique = Effect->GetTechniqueByName("BumpPointLow");
	BumpPointHighTechnique = Effect->GetTechniqueByName("BumpPointHigh");
	SpecularPointLowTechnique = Effect->GetTechniqueByName("SpecularPointLow");
	SpecularPointHighTechnique = Effect->GetTechniqueByName("SpecularPointHigh");
	BumpSpecularPointHighTechnique = Effect->GetTechniqueByName("BumpSpecularPointHigh");
	BumpSpecularPointLowTechnique = Effect->GetTechniqueByName("BumpSpecularPointLow");

	// referentni shader by Kocman
	//ReferenceShaderTechnique = Effect->GetTechniqueByName("ReferenceFX");

	

	LightPosition	= Effect->GetParameterByName(NULL, "LightPosition");
	LightSpecular	= Effect->GetParameterByName(NULL, "LightSpecular");
	LightColor		= Effect->GetParameterByName(NULL, "LightColor");
	LightRange		= Effect->GetParameterByName(NULL, "LightRange");
	LightIntensity	= Effect->GetParameterByName(NULL, "LightIntensity");
	SpecularIntensity	= Effect->GetParameterByName(NULL, "SpecularIntensity");
	LightDirection	= Effect->GetParameterByName(NULL, "LightDirection");

	LightPositionA	= Effect->GetParameterByName(NULL, "LightPositionA");;
	LightColorA	= Effect->GetParameterByName(NULL, "LightColorA");;
	LightIntensityA	= Effect->GetParameterByName(NULL, "LightIntensityA");;
	LightSpecularIntensityA	= Effect->GetParameterByName(NULL, "LightSpecularIntensityA");;
	LightSpecularColorA	= Effect->GetParameterByName(NULL, "LightSpecularColorA");;
	LightRangeA	= Effect->GetParameterByName(NULL, "LightRangeA");;
	LightCountA	= Effect->GetParameterByName(NULL, "LightCountA");;

	

	// Load shader light params if it want lights
	// Then load it 
	/*
	PointLightCount = Effect->GetParameterByName(NULL, "PointLightCount");
	DirectLightCount = Effect->GetParameterByName(NULL, "DirectLightCount");
	if (WantLightsCount == 1)
	{
		PointLightPosition[0] = Effect->GetParameterByName(NULL, "PointLightPosition");
		PointLightSpecular[0] = Effect->GetParameterByName(NULL, "PointLightSpecular");
		PointLightColor[0]	  =	Effect->GetParameterByName(NULL, "PointLightColor");
		PointLightRange[0]	  = Effect->GetParameterByName(NULL, "PointLightRange");
		PointLightIntensity[0]	 = Effect->GetParameterByName(NULL, "PointLightIntensity");
	} else
	{		
		for (int i = 0; i < WantLightsCount; i++)
		{
			PointLightPosition[i] = Effect->GetParameterElement("PointLightPosition", i);
			PointLightSpecular[i] = Effect->GetParameterElement("PointLightSpecular", i);
			PointLightColor[i]	  =	Effect->GetParameterElement("PointLightColor", i);
			PointLightRange[i]	  = Effect->GetParameterElement("PointLightRange", i);
			PointLightIntensity[i]	 = Effect->GetParameterElement("PointLightIntensity", i);
		}
	}


	// for directional lights
	if (WantDirectLightsCount == 1)
	{
		DirectLightDirection[0] = Effect->GetParameterByName(NULL, "DirectLightDirection");
		DirectLightSpecular[0] = Effect->GetParameterByName(NULL, "DirectLightSpecular");
		DirectLightColor[0]	  =	Effect->GetParameterByName(NULL, "DirectLightColor");
		
	} else
	{		
		for (int i = 0; i < WantDirectLightsCount; i++)
		{
			DirectLightDirection[i] = Effect->GetParameterElement("DirectLightDirection", i);
			DirectLightSpecular[i] = Effect->GetParameterElement("DirectLightSpecular", i);
			DirectLightColor[i]	  =	Effect->GetParameterElement("DirectLightColor", i);			
		}
	}
	*/


	return true;
}


void WeShader::SetParams(WeMaterial *Mat)
{
	if (ResWidth) Effect->SetFloat(ResWidth, (float)WeRenderDevice::Instance()->GetResWidth());
	if (ResHeight) Effect->SetFloat(ResHeight, (float)WeRenderDevice::Instance()->GetResHeight());
	if (BloomScale) Effect->SetFloat(BloomScale, WeRenderDevice::Instance()->GetBloomScale());
	if (Brightness) Effect->SetFloat(Brightness, WeRenderDevice::Instance()->GetBrightness());
	if (BrightnessColor) Effect->SetFloatArray(BrightnessColor, (float *)&WeRenderDevice::Instance()->GetBrightnessColor(), 4);
	if (AmbientColor) Effect->SetFloatArray(AmbientColor, (float *)&WeRenderDevice::Instance()->GetAmbientColor(), 4);

	if (SkyColor) Effect->SetFloatArray(SkyColor, (float *)&WeRenderDevice::Instance()->GetSkyColor(), 4);		
	if (GroundColor) Effect->SetFloatArray(GroundColor, (float *)&WeRenderDevice::Instance()->GetGroundColor(), 4);		
	if (PixelMotionBlurFactor) Effect->SetFloat(PixelMotionBlurFactor, WeRenderDevice::Instance()->GetPixelMotionBlurFactor());

	if (Mat)
	{
		if (Mat->Type == WEMATERIALTYPE_ALPHABLENDED) 
			Effect->SetBool(UsingAlpha, true);
		else
			Effect->SetBool(UsingAlpha, false);

		if (Param1) Effect->SetFloat(Param1, Mat->Param1);
		if (Param2) Effect->SetFloat(Param2, Mat->Param2);
		if (Param3) Effect->SetFloat(Param3, Mat->Param3);
		if (Param4) Effect->SetFloat(Param4, Mat->Param4);
		if (BumpIntensity) Effect->SetFloat(BumpIntensity, Mat->BumpIntensity);
		if (SpecularPow) Effect->SetFloat(SpecularPow, Mat->SpecularPow);
	}
}

void WeShader::SetParam1(float param1)
{
	if (Param1) Effect->SetFloat(Param1, param1);
}
void WeShader::SetParam2(float param2)
{
	if (Param2) Effect->SetFloat(Param1, param2);
}
void WeShader::SetParam3(float param3)
{
	if (Param3) Effect->SetFloat(Param3, param3);
}
void WeShader::SetParam4(float param4)
{
	if (Param4) Effect->SetFloat(Param4, param4);
}

void WeShader::SetScaling(WeVector &Scale)
{
	if (!Effect) return;

	if (Scaling)
		Effect->SetFloatArray(Scaling, (float *)&Scale, 3);

}

void WeShader::SetLight(WeLight *Light)
{
	if (!Light) return;
	
	Effect->SetFloatArray(LightPosition, (float *)&Light->Location, 3);
	Effect->SetFloatArray(LightDirection, (float *)&Light->Direction, 3);
	Effect->SetFloat(LightRange, Light->Range);	

	D3DXVECTOR4 tmpV(0,0,0,0);

	tmpV.x = Light->GetDiffuseColor().r;
	tmpV.y = Light->GetDiffuseColor().g;
	tmpV.z = Light->GetDiffuseColor().b;
	tmpV.w = Light->GetDiffuseColor().a;
	Effect->SetVector(LightColor, &tmpV);

	// TODO: Maybe uncomment this later
	tmpV.x = Light->GetSpecularColor().r;
	tmpV.y = Light->GetSpecularColor().g;
	tmpV.z = Light->GetSpecularColor().b;
	tmpV.w = Light->GetSpecularColor().a;
	Effect->SetVector(LightSpecular, &tmpV);	
	Effect->SetFloat(LightIntensity, Light->GetIntensity());		

	if (Light->GetSpecularIntensity() > 0.0)
	{
		if (Light->IsDynamic)
		{
		/*	FILE *f = fopen("light.txt", "at+");
			fprintf(f, "%3.3f\n", Light->GetSpecularIntensity());
			fflush(f);
			fclose(f);*/
		}
	}

	Effect->SetFloat(SpecularIntensity, Light->GetSpecularIntensity());
}


void WeShader::SetLights(CSmartList <WeLight *> &Lights)
{
	//if (Lights->GetItemsCount() = 0) return;



//	for(int i=0; i<Lights->GetItemsCount(); i++)
//	{
		int CountX = Lights.GetItemsCount();
		

		D3DXVECTOR4 Position[5];
		D3DXVECTOR4 Color[5];
		float Intensity[5];
		float SpecularIntensity[5];
		D3DXVECTOR4 SpecularColor[5];
		float Range[5];
		int i=0;
		for (Lights.First(); !Lights.End(); Lights.Next())
		{
			Position[i] = D3DXVECTOR4( //0.0f,
										//0.0f,
										//0.0f,
										Lights.GetItem()->Location,
										1.0f);
			Color[i] = D3DXVECTOR4(	Lights.GetItem()->GetDiffuseColor().r,
									Lights.GetItem()->GetDiffuseColor().g,
									Lights.GetItem()->GetDiffuseColor().b,
									1.0f);
			Intensity[i] = Lights.GetItem()->GetIntensity();
			SpecularIntensity[i] = Lights.GetItem()->GetSpecularIntensity();
			SpecularColor[i] = D3DXVECTOR4(	Lights.GetItem()->GetSpecularColor().r,
											Lights.GetItem()->GetSpecularColor().g,
											Lights.GetItem()->GetSpecularColor().b,
											1.0f);
			Range[i] = Lights[i]->Range;
				i++;
		}

		Effect->SetVectorArray(LightPositionA, Position, CountX);
		Effect->SetVectorArray(LightColorA, Color, CountX);
		Effect->SetFloatArray(LightIntensityA, Intensity, CountX);
		Effect->SetFloatArray(LightSpecularIntensityA, SpecularIntensity, CountX);
		Effect->SetVectorArray(LightSpecularColorA, SpecularColor, CountX);
		Effect->SetFloatArray(LightRangeA, Range, CountX);
		Effect->SetFloat(LightCountA, (float)CountX);
}

void WeShader::SetTextureAnimation(WeMatrix &TexAnimM)
{
	if (!Effect) return;


	if (TextureAnimation)
	{
		Effect->SetMatrix(TextureAnimation, &TexAnimM);
	} else
	{
		WeRenderDevice::Instance()->Device->SetTransform(D3DTS_TEXTURE0, &TexAnimM);
	}
}

void WeShader::SetProjectorMatrix(WeMatrix &m)
{
	if (ProjectorMatrix)
		Effect->SetMatrix(ProjectorMatrix, &m);
}

void WeShader::SetProjectorPosition(WeVector &v)
{
	if (ProjectorPosition)
	{
		Effect->SetFloatArray(ProjectorPosition, (float*)&v, 3);
	}
}




void WeShader::SetShadowOmniLightPosition(WeVector &v)
{
	if (!Effect) return;	
	if (!ShadowOmniLightPosition) return;

	Effect->SetFloatArray(ShadowOmniLightPosition, (float *)&v, 3);
}

bool WeShader::SetAmbientTechnique()
{
	if (!Effect) return false;

	if (AmbientTechnique) 
	{
		Effect->SetTechnique(AmbientTechnique);					
		return true;
	}	
	else return SetDefaultTechnique();
	
}


bool WeShader::SetRefractiveTechnique()
{
	if (!Effect) return false;

	if (RefractiveTechnique) 
	{
		Effect->SetTechnique(RefractiveTechnique);					
		return true;
	}	
	else return SetDefaultTechnique();
}


bool WeShader::SetDefaultTechnique()
{
	if (DefaultTechnique)
	{
		Effect->SetTechnique(DefaultTechnique);
		return true;
	} else return false;
}

bool WeShader::SetTechnique()
{
	if (!Effect) return false;

	if (WeShaderManager::Instance()->GetQuality() == SHADER_LOW)
	{
		if (WeRenderDevice::Instance()->GetSpecular() && WeRenderDevice::Instance()->GetBump())
		{
			if (BumpSpecularPointLowTechnique) 
			{
				Effect->SetTechnique(BumpSpecularPointLowTechnique);			
				return true;
			} else 	return SetDefaultTechnique();

		}

		if (WeRenderDevice::Instance()->GetSpecular())
		{
			if (SpecularPointLowTechnique) 
			{
				Effect->SetTechnique(SpecularPointLowTechnique);			
				return true;
			} else 	return SetDefaultTechnique();
		} 

		if (WeRenderDevice::Instance()->GetBump())
		{
			if (BumpPointLowTechnique) 
			{
				Effect->SetTechnique(BumpPointLowTechnique);
				return true;
			}
			else return SetDefaultTechnique();			
		}

		if (DiffusePointLowTechnique) 
		{
			Effect->SetTechnique(DiffusePointLowTechnique);
			return true;
		} else return SetDefaultTechnique();
	}


	if (WeShaderManager::Instance()->GetQuality() == SHADER_HIGH)
	{
	//	Effect->SetTechnique(ReferenceShaderTechnique);
	//	return true;

		
		if (WeRenderDevice::Instance()->GetSpecular() && WeRenderDevice::Instance()->GetBump())
		{
			if (BumpSpecularPointHighTechnique) 
			{
				Effect->SetTechnique(BumpSpecularPointHighTechnique);			
				return true;
			} else 	return SetDefaultTechnique();
		}


		if (WeRenderDevice::Instance()->GetSpecular())
		{
			if (SpecularPointHighTechnique) 
			{
				Effect->SetTechnique(SpecularPointHighTechnique);			
				return true;
			} else return SetDefaultTechnique();
		}

		if (WeRenderDevice::Instance()->GetBump())
		{
			if (BumpPointHighTechnique) 
			{
				Effect->SetTechnique(BumpPointHighTechnique);
				return true;
			}
			else return SetDefaultTechnique();
		}

		if (DiffusePointHighTechnique) 
		{
			Effect->SetTechnique(DiffusePointHighTechnique);
			return true;
		} else return SetDefaultTechnique();
	
	}
	return false;
}




bool WeShader::SetQuality(WeShaderQuality Quality_)
{
	//if (!Effect) return false;
	//Quality = Quality_;
	// // if shader quality will be medium or high and it is not supported just use low quality.
	////if (Quality == SHADER_MEDIUM && !WeRenderDevice::Instance()->SupportsShader2a()) Quality = SHADER_LOW;
	////if (Quality == SHADER_HIGH && !WeRenderDevice::Instance()->SupportsShader2a()) 
	////{
	////	Quality = SHADER_MEDIUM;
	////	if (Quality == SHADER_MEDIUM && !WeRenderDevice::Instance()->SupportsShader2a()) Quality = SHADER_LOW;
	////}
	//

	//if (Quality == SHADER_LOW)		
	//{
	//	//MessageBox(0, "Low", 0, 0);
	//	Effect->SetTechnique("Low");
	//	return true;
	//}
	////if (Quality == SHADER_MEDIUM)		
	////{
	////	//MessageBox(0, "Med", 0, 0);		
	////	if (Effect->ValidateTechnique("Medium") == D3D_OK) Effect->SetTechnique("Medium");
	////	else Effect->SetTechnique("Low");
	////}
	//if (Quality == SHADER_HIGH)		
	//{
	//	//MessageBox(0, "High", 0, 0);
	//	//Effect->SetTechnique("High");
	//	if (Effect->ValidateTechnique("High") == D3D_OK) 
	//	{
	//		Effect->SetTechnique("High");
	//		return true;
	//	}
	//	//else if (Effect->ValidateTechnique("Medium") == D3D_OK) Effect->SetTechnique("Medium");
	//	else 
	//	{
	//		Effect->SetTechnique("Low");
	//		return false;
	//	}
	//}

	return false;
}