/*
   WeTrailRenderer.cpp

   Autor: Nebojsa Vislavski
   Date: 9.11.2005.
   Version: 1.0

   This is particle renderer. Used to render particle systems
  
*/


#include <WeTrailRenderer.h>
#include <WeRenderDevice.h>
#include <WeTrail.h>
#include <WeProfiler.h>
#include <WeBatchItem.h>




D3DVERTEXELEMENT9 TrailD[] =
{
	0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0,
	0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0,
	0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0, 
	D3DDECL_END()
};




WeTrailRenderer *WeTrailRenderer::_instance = NULL;


WeTrailRenderer *WeTrailRenderer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeTrailRenderer;
	return _instance;
}

bool WeTrailRenderer::Initialize()
{
	if (bInitialized) return true;

	bInitialized = true;
	WeRenderDevice::Instance()->Device->CreateVertexDeclaration(TrailD, &TrailDeclaration);

	OnResetDevice();

	return true;
}


void WeTrailRenderer::ShutDown()
{
	if (!bInitialized) return;

	SAFE_RELEASE( TrailDeclaration );
	SAFE_RELEASE( TrailBuffer );
	bInitialized = false;
}


void WeTrailRenderer::OnLostDevice()
{
	OutputDebugString("**** TRAIL RENDERER - OnLost Device\n");
	SAFE_RELEASE( TrailBuffer );
	SAFE_RELEASE( TrailIndex );
}

void WeTrailRenderer::OnResetDevice()
{
	TrailBufferOffset = 0;
	// We need quad array
	if (FAILED(WeRenderDevice::Instance()->Device->CreateVertexBuffer(MAX_TRAIL_BUFFER_SIZE*sizeof(WeTrail::STrailVertex),D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, 0, D3DPOOL_DEFAULT, &TrailBuffer, NULL)))
	{
		TrailBuffer = NULL;
		bInitialized = false;		
	}
	if (FAILED(WeRenderDevice::Instance()->Device->CreateIndexBuffer(MAX_TRAIL_BUFFER_SIZE*sizeof(WORD), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &TrailIndex,NULL)))
	{
		SAFE_RELEASE(TrailBuffer);
		TrailIndex = NULL;
		bInitialized = false;
	}	

}



WeTrailRenderer::WeTrailRenderer()
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


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeTrailRenderer::Render(WeTrail *trail)
{
	int NumToDraw = 0;
	int Size = 0;
	void *v;


	TrailBuffer->Lock(0, 0, (void **)&v, D3DLOCK_DISCARD);
	memcpy(v, trail->VertexData, trail->VertexDataCount*sizeof(WeTrail::STrailVertex));			
	TrailBuffer->Unlock();

	TrailIndex->Lock(0,0, (void **)&v, D3DLOCK_DISCARD);
	memcpy(v, trail->IndexData, trail->GetIndexSize());
	TrailIndex->Unlock();
	
	/*FILE *f;
	f = fopen("trail.txt", "at+");
	fprintf(f, "index size: %d faces: %d\n", trail->GetIndexSize(), trail->GetFacesCount());
	fflush(f);
	fclose(f);*/

	WeRenderDevice::Instance()->Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, trail->VertexDataCount, 0, trail->GetFacesCount());
	//WeRenderDevice::Instance()->Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, trail->SegmentsCount*2);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeTrailRenderer::Render(WeWorld *World, WeTrail *trail)
{
	if (!bInitialized) return;

	WeRenderDevice::Instance()->Device->SetVertexDeclaration(TrailDeclaration);
	WeRenderDevice::Instance()->Device->SetStreamSource(0, TrailBuffer, 0, sizeof(WeTrail::STrailVertex));
	WeRenderDevice::Instance()->Device->SetIndices(TrailIndex);


	Render(trail);	
}

