/*
      File: WeMaterial.cpp
	  Author: Tomislav Kukic
	  Date: 12.09.2005.
	  Version: 1.0
*/




#include <WeMaterial.h>
#include <WeTextureManager.h>
#include <WeShaderManager.h>
#include <WeRenderDevice.h>
#include <WeRenderTargetManager.h>
#include <WeTextureEditor.h>
#include <WePackage.h>
#include <WeTimer.h>
#include <WeLoadCounter.h>


WeMaterial::WeMaterial()
{
	Initialize();
}

WeMaterial::~WeMaterial()
{
	for (int i = 0; i < MAX_MATERIAL_TEXTURES; i++)
	{
		Textures[i] = NULL;        
	}
	Shader = NULL;
}


void WeMaterial::Initialize()
{
	Shader				= NULL;
	DoubleSided			= false;
	Type				= WEMATERIALTYPE_NORMAL;
	Name				= "";
	ReflectionUpdateInterval = 1;
	EnvMapSize			= 512;
	EnvMapSampler		= 1;
	ReflectionType		= WEREFLECTIONTYPE_NONE;
	MaterialColor		= WeColor(1.0f,1.0f,1.0f,1.0f);
	TextureAnimation	= WETEXTUREANIMATION_NONE;
	TextureAnimParam1	= 0.0f;
	TextureAnimParam2	= 0.0f;
	TextureAnimParam3	= 0.0f;
	TextureAnimParam4	= 0.0f;
	TextureAnimParam5	= 0.0f;
	Param1				= 0.0f;
	Param2				= 0.0f;
	Param3				= 0.0f;
	Param4				= 0.0f;
	iParam1				= 0;
	iParam2				= 0;
	iParam3				= 0;
	BumpIntensity		= 2.5f;
	SpecularPow			= 4;
	AlphaRef			= 0;
	Version				= 15;
	AcceptLights		= true;
	Glowing				= false;
	Refractive			= false;
	BlendType			= WEMATERIAL_BLEND_STANDARD;

	D3DXMatrixIdentity(&TextureAnimationM);

	for (int i = 0; i < MAX_MATERIAL_TEXTURES; i++) Textures[i] = NULL;
}


void WeMaterial::SetTextures(float ElapsedTime_)
{
	ElapsedTime = ElapsedTime_;

	for (int i = 0; i < MAX_MATERIAL_TEXTURES; i++)
	{
		WeRenderDevice::Instance()->SetTexture(i, Textures[i]);
	}	
	// Set and material color
	if (Shader)
	{
		// calculate texture transformation matrix
		switch(TextureAnimation)
		{
			case WETEXTUREANIMATION_NONE: TexAnimNormal(); break;
			case WETEXTUREANIMATION_SCROLL: TexAnimScroll(); break;
			case WETEXTUREANIMATION_ROTATE: TexAnimRotate(); break;
			case WETEXTUREANIMATION_SHAKE: TexAnimShake(); break;
			case WETEXTUREANIMATION_SLIDESHOW: TexAnimSlideShow(); break;
			case WETEXTUREANIMATION_ZOOM: TexAnimZoom(); break;
			case WETEXTUREANIMATION_SLIDESHOWRANDOM: TexAnimSlideShowRandom(); break;
			case WETEXTUREANIMATION_UVSCALE: TexAnimUVScale(); break;
		}

		Shader->SetTextureAnimation(TextureAnimationM);
	}
	
	
}


void WeMaterial::Load(StringParam ShaderName, StringParam TextureName)
{
	// TODO: Erase this later
	// --
	//Type = WEMATERIALTYPE_ALPHABLENDED;


	Textures[0] = WeTextureManager::Instance()->Load(TextureName);	
	Shader = WeShaderManager::Instance()->Load(ShaderName);
}



bool WeMaterial::Load(StringParam HoleName)
{
	Strings Names;
	String EffectName;
	int		ver;

	char TempName[1000];
	sprintf(TempName, "MATERIAL \"%s\"", HoleName.GetString());
	SetLoadingName((String)TempName);

	if(WePackage::Instance()->BeginReading(HoleName, 2))
	{
		ver = WePackage::Instance()->DataStream.ReadInt();
		if (ver < 10)
		{
			WePackage::Instance()->DataStream.Seek(-(int)sizeof(int), SEEK_CUR);
			Type = WePackage::Instance()->DataStream.ReadInt();
			ReflectionType = WePackage::Instance()->DataStream.ReadInt();

			EffectName = WePackage::Instance()->DataStream.ReadString();		
			
			for(int i = 0; i < 16; i++)
			{
				Names.PushBack(WePackage::Instance()->DataStream.ReadString());
			}

			TextureAnimation = WePackage::Instance()->DataStream.ReadInt();
			TextureAnimParam1 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam2 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam3 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam4 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam5 = WePackage::Instance()->DataStream.ReadFloat();

			Param1 = WePackage::Instance()->DataStream.ReadFloat();
			Param2 = WePackage::Instance()->DataStream.ReadFloat();
			Param3 = WePackage::Instance()->DataStream.ReadFloat();
			Param4 = WePackage::Instance()->DataStream.ReadFloat();

			WePackage::Instance()->DataStream.ReadColor(MaterialColor);
		} // end of default version
		if (ver == 10)
		{
			Type = WePackage::Instance()->DataStream.ReadInt();
			BlendType = (WEMATERIAL_BLEND_TYPE)WePackage::Instance()->DataStream.ReadInt();
			ReflectionType = WePackage::Instance()->DataStream.ReadInt();

			EffectName = WePackage::Instance()->DataStream.ReadString();		
			
			for(int i = 0; i < 16; i++)
			{
				Names.PushBack(WePackage::Instance()->DataStream.ReadString());
			}

			TextureAnimation = WePackage::Instance()->DataStream.ReadInt();
			TextureAnimParam1 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam2 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam3 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam4 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam5 = WePackage::Instance()->DataStream.ReadFloat();

			Param1 = WePackage::Instance()->DataStream.ReadFloat();
			Param2 = WePackage::Instance()->DataStream.ReadFloat();
			Param3 = WePackage::Instance()->DataStream.ReadFloat();
			Param4 = WePackage::Instance()->DataStream.ReadFloat();

			WePackage::Instance()->DataStream.ReadColor(MaterialColor);
		} // end of version 10
		if (ver == 11)
		{
			Type = WePackage::Instance()->DataStream.ReadInt();
			BlendType = (WEMATERIAL_BLEND_TYPE)WePackage::Instance()->DataStream.ReadInt();
			ReflectionType = WePackage::Instance()->DataStream.ReadInt();
			// new in version 11
			AcceptLights = WePackage::Instance()->DataStream.ReadBool();

			EffectName = WePackage::Instance()->DataStream.ReadString();		
			
			for(int i = 0; i < 16; i++)
			{
				Names.PushBack(WePackage::Instance()->DataStream.ReadString());
			}

			TextureAnimation = WePackage::Instance()->DataStream.ReadInt();
			TextureAnimParam1 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam2 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam3 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam4 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam5 = WePackage::Instance()->DataStream.ReadFloat();

			Param1 = WePackage::Instance()->DataStream.ReadFloat();
			Param2 = WePackage::Instance()->DataStream.ReadFloat();
			Param3 = WePackage::Instance()->DataStream.ReadFloat();
			Param4 = WePackage::Instance()->DataStream.ReadFloat();

			WePackage::Instance()->DataStream.ReadColor(MaterialColor);
		} // end of version 11
		if (ver == 12)
		{
			Type = WePackage::Instance()->DataStream.ReadInt();
			BlendType = (WEMATERIAL_BLEND_TYPE)WePackage::Instance()->DataStream.ReadInt();
			ReflectionType = WePackage::Instance()->DataStream.ReadInt();
			// new in version 11
			AcceptLights = WePackage::Instance()->DataStream.ReadBool();

			EffectName = WePackage::Instance()->DataStream.ReadString();		

			for(int i = 0; i < 16; i++)
			{
				Names.PushBack(WePackage::Instance()->DataStream.ReadString());
			}

			TextureAnimation = WePackage::Instance()->DataStream.ReadInt();
			TextureAnimParam1 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam2 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam3 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam4 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam5 = WePackage::Instance()->DataStream.ReadFloat();

			Param1 = WePackage::Instance()->DataStream.ReadFloat();
			Param2 = WePackage::Instance()->DataStream.ReadFloat();
			Param3 = WePackage::Instance()->DataStream.ReadFloat();
			Param4 = WePackage::Instance()->DataStream.ReadFloat();

			WePackage::Instance()->DataStream.ReadColor(MaterialColor);
			BumpIntensity = WePackage::Instance()->DataStream.ReadFloat();
			SpecularPow = WePackage::Instance()->DataStream.ReadFloat();
		} // end of version 12

		if (ver == 13)
		{
			Type = WePackage::Instance()->DataStream.ReadInt();
			BlendType = (WEMATERIAL_BLEND_TYPE)WePackage::Instance()->DataStream.ReadInt();
			ReflectionType = WePackage::Instance()->DataStream.ReadInt();
			// new in version 11
			AcceptLights = WePackage::Instance()->DataStream.ReadBool();

			// new in version 13
			Glowing = WePackage::Instance()->DataStream.ReadBool();
			Refractive = WePackage::Instance()->DataStream.ReadBool();

			EffectName = WePackage::Instance()->DataStream.ReadString();		

			for(int i = 0; i < 16; i++)
			{
				Names.PushBack(WePackage::Instance()->DataStream.ReadString());
			}

			TextureAnimation = WePackage::Instance()->DataStream.ReadInt();
			TextureAnimParam1 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam2 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam3 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam4 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam5 = WePackage::Instance()->DataStream.ReadFloat();

			Param1 = WePackage::Instance()->DataStream.ReadFloat();
			Param2 = WePackage::Instance()->DataStream.ReadFloat();
			Param3 = WePackage::Instance()->DataStream.ReadFloat();
			Param4 = WePackage::Instance()->DataStream.ReadFloat();

			WePackage::Instance()->DataStream.ReadColor(MaterialColor);
			BumpIntensity = WePackage::Instance()->DataStream.ReadFloat();
			SpecularPow = WePackage::Instance()->DataStream.ReadFloat();
		} // end of version 13

		if (ver == 14)
		{
			Type = WePackage::Instance()->DataStream.ReadInt();
			BlendType = (WEMATERIAL_BLEND_TYPE)WePackage::Instance()->DataStream.ReadInt();
			ReflectionType = WePackage::Instance()->DataStream.ReadInt();
			// new in version 11
			AcceptLights = WePackage::Instance()->DataStream.ReadBool();

			// new in version 13
			Glowing = WePackage::Instance()->DataStream.ReadBool();
			Refractive = WePackage::Instance()->DataStream.ReadBool();

			EffectName = WePackage::Instance()->DataStream.ReadString();		

			for(int i = 0; i < 16; i++)
			{
				Names.PushBack(WePackage::Instance()->DataStream.ReadString());
			}

			TextureAnimation = WePackage::Instance()->DataStream.ReadInt();
			TextureAnimParam1 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam2 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam3 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam4 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam5 = WePackage::Instance()->DataStream.ReadFloat();

			Param1 = WePackage::Instance()->DataStream.ReadFloat();
			Param2 = WePackage::Instance()->DataStream.ReadFloat();
			Param3 = WePackage::Instance()->DataStream.ReadFloat();
			Param4 = WePackage::Instance()->DataStream.ReadFloat();

			WePackage::Instance()->DataStream.ReadColor(MaterialColor);
			BumpIntensity = WePackage::Instance()->DataStream.ReadFloat();
			SpecularPow = WePackage::Instance()->DataStream.ReadFloat();
			AlphaRef = WePackage::Instance()->DataStream.ReadDWord();
		} // end of version 14



		if (ver == 15)
		{
			Type = WePackage::Instance()->DataStream.ReadInt();
			BlendType = (WEMATERIAL_BLEND_TYPE)WePackage::Instance()->DataStream.ReadInt();
			ReflectionType = WePackage::Instance()->DataStream.ReadInt();
			// new in version 11
			AcceptLights = WePackage::Instance()->DataStream.ReadBool();

			// new in version 13
			Glowing = WePackage::Instance()->DataStream.ReadBool();
			Refractive = WePackage::Instance()->DataStream.ReadBool();
			DoubleSided = WePackage::Instance()->DataStream.ReadBool();

			EffectName = WePackage::Instance()->DataStream.ReadString();		

			for(int i = 0; i < 16; i++)
			{
				Names.PushBack(WePackage::Instance()->DataStream.ReadString());
			}

			TextureAnimation = WePackage::Instance()->DataStream.ReadInt();
			TextureAnimParam1 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam2 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam3 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam4 = WePackage::Instance()->DataStream.ReadFloat();
			TextureAnimParam5 = WePackage::Instance()->DataStream.ReadFloat();

			Param1 = WePackage::Instance()->DataStream.ReadFloat();
			Param2 = WePackage::Instance()->DataStream.ReadFloat();
			Param3 = WePackage::Instance()->DataStream.ReadFloat();
			Param4 = WePackage::Instance()->DataStream.ReadFloat();

			WePackage::Instance()->DataStream.ReadColor(MaterialColor);
			BumpIntensity = WePackage::Instance()->DataStream.ReadFloat();
			SpecularPow = WePackage::Instance()->DataStream.ReadFloat();
			AlphaRef = WePackage::Instance()->DataStream.ReadDWord();
		} // end of version 15

		WePackage::Instance()->EndReading();

	}else{
		Shader = NULL;
		for(int i=0; i<16; i++) Textures[i] = NULL;
		Name = "";
		ResetLoadingName();
		return false;
	}

	ResetLoadingName();

	Shader = WeShaderManager::Instance()->Load(EffectName,false,true);		
	//Shader = WeShaderManager::Instance()->Load("effects\\EnvStandard.fx", false, false);

	int j = 0;
	for(Names.First(); !Names.End(); Names.Next())
	{
		if ((Names.GetItem() != (String)"") && (Names.GetItem() != (String)" "))
			Textures[j] = WeTextureManager::Instance()->Load(Names.GetItem(), true);
		else
			Textures[j] = NULL;
		j++;
	}

	this->Name = HoleName;
	return true;
}


void WeMaterial::Tick()
{
}

// ==================================================================================
// ==================================================================================
// ==================================================================================
// ==================================================================================
// ==================================================================================
// ==================================================================================
// Texture Animation

void WeMaterial::TexAnimNormal()
{
	D3DXMatrixIdentity(&TextureAnimationM);
}


void WeMaterial::TexAnimScroll()
{
   TextureAnimationM._11 = 1.0f;
   TextureAnimationM._21 = 0.0f;
   TextureAnimationM._31 = 0.0f;
   TextureAnimationM._12 = 0.0f;
   TextureAnimationM._22 = 1.0f;
   TextureAnimationM._33 = 1.0f;
   TextureAnimationM._41 = TextureAnimParam1 * ElapsedTime;
   TextureAnimationM._42 = TextureAnimParam2 * ElapsedTime;
}

void WeMaterial::TexAnimShake()
{
   float Indicator; 
   
   Indicator = (float)(rand() % 1000);
   Indicator = TextureAnimParam1 / Indicator;

   D3DXMatrixIdentity(&TextureAnimationM);
   TextureAnimationM._41 = Indicator;
   TextureAnimationM._42 = Indicator;
}

void WeMaterial::TexAnimSlideShow()
{
	float k1, k2;	
	int SlicesNum = 10;
	int xpos, ypos, tmpi;
	float sx,sy;

	// ako je nesto 0 onda da izadje
	if ((TextureAnimParam1 == 0) || (TextureAnimParam2 == 0) || (TextureAnimParam3 == 0)) 
	{
		D3DXMatrixIdentity(&TextureAnimationM);
		return;	
	}
	
	//TextureAnimParam5 += WeTimer::Instance()->GetDeltaTimeF();
	TextureAnimParam5 = ElapsedTime;
	tmpi = (int)(TextureAnimParam5 / (1.0f / TextureAnimParam4));
	iParam1 = tmpi % (int)TextureAnimParam3;

	//if (TextureAnimParam5 > (1.0f / TextureAnimParam4)) 
	//{ 
	//	TextureAnimParam5 = 0.0f;
	//	iParam1++;
 //       
	//	if (iParam1 >= TextureAnimParam3) iParam1 = 0;
	//}
	
	k1 = 1.0f / TextureAnimParam1;
	k2 = 1.0f / TextureAnimParam2;

	xpos = iParam1 % (int)k1;
	ypos = iParam1 / (int)k1;
	sx = (float)xpos * TextureAnimParam1;
	sy = (float)ypos * TextureAnimParam2;

	TextureAnimationM._11 = TextureAnimParam1;
	TextureAnimationM._12 = 0.0f;
	TextureAnimationM._13 = 0.0f;

	TextureAnimationM._21 = 0.0f;
	TextureAnimationM._22 = TextureAnimParam2;
	TextureAnimationM._23 = 0.0f;

	TextureAnimationM._31 = 0.0f;
	TextureAnimationM._32 = 0.0f;
	TextureAnimationM._33 = 1.0f;

	TextureAnimationM._41 = sx;
	TextureAnimationM._42 = sy;

}


void WeMaterial::TexAnimSlideShowRandom()
{
	float k1, k2;	
	int SlicesNum = 10;
	int xpos, ypos;
	float sx,sy;

	// ako je nesto 0 onda da izadje
	if ((TextureAnimParam1 == 0) || (TextureAnimParam2 == 0) || (TextureAnimParam3 == 0)) 
	{
		D3DXMatrixIdentity(&TextureAnimationM);
		return;	
	}

	TextureAnimParam5 += WeTimer::Instance()->GetDeltaTimeF();
	if (TextureAnimParam5 > 1.0f / TextureAnimParam4)
	{
		iParam1 = rand() % (int)TextureAnimParam3;	
		TextureAnimParam5 = 0;
	}	

	k1 = 1.0f / TextureAnimParam1;
	k2 = 1.0f / TextureAnimParam2;

	xpos = iParam1 % (int)k1;
	ypos = iParam1 / (int)k1;
	sx = (float)xpos * TextureAnimParam1;
	sy = (float)ypos * TextureAnimParam2;

	TextureAnimationM._11 = TextureAnimParam1;
	TextureAnimationM._12 = 0.0f;
	TextureAnimationM._13 = 0.0f;

	TextureAnimationM._21 = 0.0f;
	TextureAnimationM._22 = TextureAnimParam2;
	TextureAnimationM._23 = 0.0f;

	TextureAnimationM._31 = 0.0f;
	TextureAnimationM._32 = 0.0f;
	TextureAnimationM._33 = 1.0f;

	TextureAnimationM._41 = sx;
	TextureAnimationM._42 = sy;

}

void WeMaterial::TexAnimWaveX()
{
}

void WeMaterial::TexAnimWaveY()
{
}


void WeMaterial::TexAnimRotate()
{
	float Angle = D3DX_PI * 2 * TextureAnimParam1 * ElapsedTime;

	WeMatrix tmpTrans;
	WeMatrix tmpTrans2;
	WeMatrix tmpRot;

	
	D3DXMatrixTranslation(&tmpTrans, -TextureAnimParam2, -TextureAnimParam3, 0.0f);
	D3DXMatrixRotationZ(&tmpRot, Angle);
	D3DXMatrixTranslation(&tmpTrans2, TextureAnimParam2, TextureAnimParam3, 0.0f);

	D3DXMatrixMultiply(&TextureAnimationM, &tmpTrans, &tmpRot);
	D3DXMatrixMultiply(&TextureAnimationM, &TextureAnimationM, &tmpTrans2);
}



void WeMaterial::TexAnimZoom()
{
    float tmp;
	
	//tmp = cosf(WeTimer::Instance()->GetElapsedTimeF()*TextureAnimParam3 / 2*D3DX_PI);
	tmp = cosf(ElapsedTime/2*D3DX_PI);

	float from;
	from = abs(TextureAnimParam2 - TextureAnimParam1) / 2.0f;	
	tmp = tmp * (abs(TextureAnimParam1 - TextureAnimParam2)) + from;


	TextureAnimationM._11 = tmp;
	TextureAnimationM._12 = 0.0f;
	TextureAnimationM._13 = 0.0f;
	
	TextureAnimationM._21 = 0.0f;
	TextureAnimationM._22 = tmp;
	TextureAnimationM._23 = 0.0f;

	TextureAnimationM._31 = 0.0f;
	TextureAnimationM._32 = 0.0f;
	TextureAnimationM._33 = tmp;

	TextureAnimationM._41 = 0.0f;
	TextureAnimationM._42 = 0.0f;
	TextureAnimationM._43 = 0.0f;
}


void WeMaterial::TexAnimUVScale()
{

	D3DXMatrixIdentity( &TextureAnimationM);
	TextureAnimationM._11 = TextureAnimParam1;
	TextureAnimationM._22 = TextureAnimParam2;
	TextureAnimationM._33 = TextureAnimParam3;
}