/*
   WeParticleRenderer.cpp

   Autor: Nebojsa Vislavski
   Date: 9.11.2005.
   Version: 1.0

   This is particle renderer. Used to render particle systems
  
*/


#include <WeParticleRenderer.h>
#include <WeRenderDevice.h>
#include <WeParticleSystem.h>
#include <WeProfiler.h>



const int ParticleBufferLength = 10000;
const int ParticleIndexLength = 10000;  



D3DVERTEXELEMENT9 ParticleD[] =
{
	0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
	0, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0,
	0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0,
	0, 44, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0,
	0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1,
	0, 56, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2,
	0, 68, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3,
	D3DDECL_END()
};




WeParticleRenderer *WeParticleRenderer::_instance = NULL;


WeParticleRenderer *WeParticleRenderer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeParticleRenderer;
	return _instance;
}

bool WeParticleRenderer::Initialize()
{
	if (bInitialized) return true;

	bInitialized = true;
	WeRenderDevice::Instance()->Device->CreateVertexDeclaration(ParticleD, &ParticleDeclaration);

	OnResetDevice();

	return true;
}


void WeParticleRenderer::ShutDown()
{
	if (!bInitialized) return;

	SAFE_RELEASE( ParticleDeclaration );
	SAFE_RELEASE( ParticlesBuffer );
	bInitialized = false;
}


void WeParticleRenderer::OnLostDevice()
{
	SAFE_RELEASE( ParticlesBuffer );
	SAFE_RELEASE( ParticlesIndex );
	OutputDebugString("**** ParticleRender- ONLOST\n");
}

void WeParticleRenderer::OnResetDevice()
{
	ParticleBufferOffset = 0; 
	// We need quad array
	if (FAILED(WeRenderDevice::Instance()->Device->CreateVertexBuffer(ParticleBufferLength*sizeof(WeParticle),D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &ParticlesBuffer, NULL)))
	{
		ParticlesBuffer = NULL;
		bInitialized = false;		
	}


	if (FAILED(WeRenderDevice::Instance()->Device->CreateIndexBuffer(ParticleIndexLength*sizeof(WeParticleIndex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &ParticlesIndex,NULL)))
	{
		SAFE_RELEASE(ParticlesBuffer);
		ParticlesIndex = NULL;
		bInitialized = false;
	}

	CurrentShader = NULL;
}



WeParticleRenderer::WeParticleRenderer()
{
	bInitialized = false;
	WeRenderDevice::Instance()->Renderers.PushBack(this);
}





/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeParticleRenderer::PreRender()
{
	// Render Particle systems

	WeRenderDevice::Instance()->SetRenderState(D3DRS_LIGHTING, FALSE);
	//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//WeRenderDevice::Instance()->Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

	//WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    //WeRenderDevice::Instance()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	//WeRenderDevice::Instance()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

    

}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeParticleRenderer::PostRender()
{
	//WeRenderDevice::Instance()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	WeRenderDevice::Instance()->SetRenderState(D3DRS_LIGHTING, TRUE);
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeParticleRenderer::Render(WeParticleSystem *ps)
{
	if (!WeRenderDevice::Instance()->GetRenderParticles()) return;


	int NumToDraw = 0;
	int Size = 0;
	void *v;


	NumToDraw = ps->LastIndex;

	ParticleBufferOffset = 0;
	while (NumToDraw > 0)
	{
		if (NumToDraw > ParticleBufferLength)
		{
			Size = ParticleBufferLength;				
		} else
		{
			Size = NumToDraw;
		}

		ParticlesBuffer->Lock(0, 0, (void **)&v, D3DLOCK_DISCARD);
		memcpy(v, &(ps->ParticleData[ParticleBufferOffset]), Size*sizeof(WeParticle));			
		ParticlesBuffer->Unlock();

		ParticlesIndex->Lock(0,0, (void **)&v, D3DLOCK_DISCARD);
		memcpy(v, &(ps->ParticleIndex[ParticleBufferOffset]), Size*sizeof(WeParticleIndex));
		ParticlesIndex->Unlock();

		WeRenderDevice::Instance()->Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0 /*-ParticleBufferOffset*4*/, 0, Size*4, 0, Size*2);
		WeProfiler::Instance()->ParticlesCount += Size;
		WeProfiler::Instance()->PolygonsRendered += Size*2;


		ParticleBufferOffset += Size;
		NumToDraw -= ParticleBufferLength;
	}
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeParticleRenderer::Render(WeWorld *World, WeParticleSystem *ps)
{
	if (!bInitialized) return;

	WeRenderDevice::Instance()->Device->SetVertexDeclaration(ParticleDeclaration);
	WeRenderDevice::Instance()->Device->SetStreamSource(0, ParticlesBuffer, 0, sizeof(WeParticleVertex));
	WeRenderDevice::Instance()->Device->SetIndices(ParticlesIndex);
	
	DWORD srcBlend, destBlend;
	WeRenderDevice::Instance()->Device->GetRenderState(D3DRS_SRCBLEND, &srcBlend);
	WeRenderDevice::Instance()->Device->GetRenderState(D3DRS_DESTBLEND, &destBlend);

	if (ps->Properties.BlendType == WEPARTICLE_BLEND_STANDARD)
	{
		WeRenderDevice::Instance()->Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		WeRenderDevice::Instance()->Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	} 
	if (ps->Properties.BlendType == WEPARTICLE_BLEND_ADDITIVE)
	{
		WeRenderDevice::Instance()->Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		WeRenderDevice::Instance()->Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	} 


	ps->Properties.Material->SetTextures(ps->ObjectAge);
	Render(ps);

	WeRenderDevice::Instance()->Device->SetRenderState(D3DRS_SRCBLEND, srcBlend);
	WeRenderDevice::Instance()->Device->SetRenderState(D3DRS_DESTBLEND, destBlend);
}


//void WeParticleRenderer::Render(WeWorld *World)
//{
//	if (!bInitialized) return;
//
//	WeParticleSystem *ps;
//	WeMatrix WI;
//
//
//	D3DXMatrixIdentity(&WI);
//
//	WeRenderDevice::Instance()->Device->SetVertexDeclaration(ParticleDeclaration);
//	WeRenderDevice::Instance()->Device->SetStreamSource(0, ParticlesBuffer, 0, sizeof(WeParticleVertex));
//	WeRenderDevice::Instance()->Device->SetIndices(ParticlesIndex);
//
//
//	PreRender();
//	WeRenderDevice::Instance()->BeginScene();	
//
//
//	for (ShaderBatches.First(); !ShaderBatches.End(); ShaderBatches.Next())
//	{
//		// now set effect.
//		if (ShaderBatches.GetItem()->Shader)
//		{
//			ShaderBatches.GetItem()->Shader->Begin();            
//
//			for (int i = 0; i < ShaderBatches.GetItem()->Shader->GetPassesCount(); i++)
//			{
//				ShaderBatches.GetItem()->Shader->BeginPass(i);
//				// now render particlesystems
//				for (ShaderBatches.GetItem()->ParticleSystems.First(); !ShaderBatches.GetItem()->ParticleSystems.End(); ShaderBatches.GetItem()->ParticleSystems.Next())
//				{
//					WeRenderDevice::Instance()->SetTexture(0, ShaderBatches.GetItem()->ParticleSystems.GetItem()->Material->Texture);
//
//					ps = ShaderBatches.GetItem()->ParticleSystems.GetItem();
//					Render(ps);
//				}
//				ShaderBatches.GetItem()->Shader->EndPass();                
//			}
//			ShaderBatches.GetItem()->Shader->End();            
//		} else
//		{
//			// no shader
//			for (ShaderBatches.GetItem()->ParticleSystems.First(); !ShaderBatches.GetItem()->ParticleSystems.End(); ShaderBatches.GetItem()->ParticleSystems.Next())
//			{
//				WeRenderDevice::Instance()->SetTexture(0, ShaderBatches.GetItem()->ParticleSystems.GetItem()->Material->Texture);
//
//				ps = ShaderBatches.GetItem()->ParticleSystems.GetItem();
//				Render(ps);
//			}
//		}
//
//	}
//
///*
//
//
//	for (I = ParticleSystems.begin(); I != ParticleSystems.end(); I++)
//	{
//		if ((*I)->ParticleCount == 0) continue;
//		
//		if ((*I)->Material) 
//		{
//			WeRenderDevice::Instance()->SetTexture(0, (*I)->Material->Texture);
//			if ((*I)->Material->Shader)
//			{
//				(*I)->Material->Shader->Begin();
//				(*I)->Material->Shader->BeginPass(0);				
//			}
//			
//		} else
//		{
//			WeRenderDevice::Instance()->SetTexture(0, (*I)->ParticleTexture);
//		}
//       
//		NumToDraw = (*I)->LastIndex;
//
//		ParticleBufferOffset = 0;
//		bool tmp = false;
//		tmp = false;
////		printf("NumTo Draw:%d\n", NumToDraw);
//        while (NumToDraw > 0)
//		{
//			if (NumToDraw > ParticleBufferLength)
//			{
//				Size = ParticleBufferLength;				
//			} else
//			{
//				Size = NumToDraw;
//			}
//
//			ParticlesBuffer->Lock(0, 0, (void **)&v, D3DLOCK_DISCARD);
//			memcpy(v, &((*I)->ParticleData[ParticleBufferOffset]), Size*sizeof(WeParticle));			
//			ParticlesBuffer->Unlock();
//
//			ParticlesIndex->Lock(0,0, (void **)&v, D3DLOCK_DISCARD);
//			memcpy(v, &((*I)->ParticleIndex[ParticleBufferOffset]), Size*sizeof(WeParticleIndex));
//			ParticlesIndex->Unlock();
//
//			WeRenderDevice::Instance()->Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, -ParticleBufferOffset*4, 0, Size*4, 0, Size*2);
//			WeProfiler::Instance()->ParticlesCount += Size;
//
//			//printf("POff:%d\n", ParticleBufferOffset);
//			//printf("Live:%3.3f\n", (*I)->ParticleData[(*I)->LastIndex-1].Particle[0].PPosition.z);
//			//ParticleBufferOffset += Size;
//			NumToDraw -= ParticleBufferLength;
//		}
//
//
//		if ((*I)->Material) 
//		{
//			if ((*I)->Material->Shader)
//			{
//				(*I)->Material->Shader->EndPass();
//				(*I)->Material->Shader->End();
//			}
//		}
//			
//	}
//*/
//
//	WeRenderDevice::Instance()->EndScene();	
//
//	PostRender();
//}


void WeParticleRenderer::AddParticleSystem(WeParticleSystem *particlesys)
{
	// find first one with the same shader

	for (ShaderBatches.First(); !ShaderBatches.End(); ShaderBatches.Next())
	{
		if (ShaderBatches.GetItem()->Shader == particlesys->Properties.Material->Shader)
		{
			ShaderBatches.GetItem()->ParticleSystems.PushBack(particlesys);
			return;
		}        
	}	
	//// else we need to put new shader batch and then particlesystem
	WeParticleRenderer::ShaderBatch *tmpSB = new WeParticleRenderer::ShaderBatch;

	if (particlesys->Properties.Material)
		tmpSB->Shader = particlesys->Properties.Material->Shader;
	else 
		tmpSB->Shader = NULL;

	tmpSB->ParticleSystems.PushBack(particlesys);
 //   
	ShaderBatches.PushBack(tmpSB);		
}


WeParticleRenderer::ShaderBatch::~ShaderBatch()
{
	ParticleSystems.Clear();
}

void WeParticleRenderer::ClearAll()
{
	for (ShaderBatches.First(); !ShaderBatches.End(); )
	{
		ShaderBatches.Delete();
	}	
}