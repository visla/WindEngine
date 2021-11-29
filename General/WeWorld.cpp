/*
       File: WeWorld.cpp
	   Author: Tomislav Kukic
	   Date: 13.09.2005.
	   Version: 1.0
*/




#include <WeWorld.h>
#include <WeBatchRenderer.h>
#include <WeShadowVolumeRenderer.h> 
#include <WeProjectorRenderer.h>
#include <WeVelocityRenderer.h>
#include <WeGlowRenderer.h>
#include <WeActor.h>
#include <WeRenderTree.h>
#include <WeProfiler.h>
#include <WeParticleRenderer.h>
#include <WeCamera.h>
#include <WeShaderManager.h>
#include <WeReflectionRenderer.h>
#include <WeLight.h>
#include <WePathNode.h>
#include <WeNetworkActor.h>
#include <WeParticleSystem.h>
//#include <direct.h>
#include <WeSkyBox.h>
#include <WeSkyBoxRenderer.h>
#include <WeCloth.h>
#include <WeZone.h>
#include <WePortal.h>
#include <WeStaticModel.h>
#include <WeFrustum.h>
#include <WeTextStream.h>
#include <WeRenderDevice.h>
#include <WeFlareRenderer.h>
#include <WeCanvas.h>
#include <WePostProcess.h>
#include <WeTimer.h>
#include <WeLoadCounter.h>
#include <WeProfiler.h>
#include <WeEngine.h>



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeWorld::WeWorld(void)
{
	IsEditor = false;
	isLoading	=	false;
	isUnloading =	false;
	isLoaded	=	false;
	isHeaderLoaded = false;
	bHasTicked	=	false;
	SkyBox		=	new WeSkyBox;
	Version		=	WORLD_VERSION;
	OnLoadProgress = NULL;
	OnLoadCanceled = NULL;
	OnBakingProgress = NULL;
	OnMapLoaded = NULL;
	WorldFrame = 0;
	ZonedRendering = false;

	// Create list sentinel 
	Actors = new WeActor;
	Lights = new WeLight;
	PathNodes = new WePathNode;
	FirstPathNode = NULL;
	RenderTree = new WeRenderTree;
	WeBox Box(WeVector(-1000.0,-1000.0,-1000.0), 50000.0f);

	RenderTree->Initialize(this, 15, Box);

	for (int RenderPhase = 0; RenderPhase < RENDERPHASE_COUNT; ++RenderPhase)
		EnableRenderPhase(RenderPhase, true);

#ifdef _DEBUG
	for (int i = 0; i < 10; ++i)
		ShowActorTypes[i] = true;
#endif
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeWorld::~WeWorld(void)
{
	SAFE_DELETE(RenderTree);

	WeActor *I, *tmp;
	for (I = Actors->NextActor; I != Actors;)
	{
		tmp = I->NextActor;
		I->World = NULL;
		SAFE_DELETE(I);
		I = tmp;
	}
	SAFE_DELETE(Actors);
	SAFE_DELETE(Lights);
	SAFE_DELETE(SkyBox);
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeActor* WeWorld::AddActor(WeActor* Actor)
{
	//Actors.PushBack(Actor);
	if (!Actor) return Actor;

	// just add him to the list
	Actor->World = this;
	Actor->PrevActor = Actors->PrevActor;
	Actor->NextActor = Actors;	
	Actors->PrevActor = Actor;
	Actor->PrevActor->NextActor = Actor;

	// if actor is light then add him to his own list
	if (Actor->IsLight)
	{
		((WeLight *)Actor)->PrevLight = Lights->PrevLight;
		((WeLight *)Actor)->NextLight = Lights;
		Lights->PrevLight = ((WeLight *)Actor);
		((WeLight *)Actor)->PrevLight->NextLight = ((WeLight *)Actor);		
	}

	if (Actor->Type == WETYPE_PATHNODE)
	{
		((WePathNode *)Actor)->PrevPathNode = PathNodes->PrevPathNode;
		((WePathNode *)Actor)->NextPathNode = PathNodes;
		PathNodes->PrevPathNode = ((WePathNode *)Actor);
		((WePathNode *)Actor)->PrevPathNode->NextPathNode = ((WePathNode *)Actor);
	}

	if (Actor->Type == WETYPE_ZONE)
	{
		Zones.PushBack((WeZone*)Actor);
	}
	if (Actor->Type == WETYPE_PORTAL)
	{
		Portals.PushBack((WePortal*)Actor);
	}

	// if actor is prefab than add it to render tree
	if (Actor->IsPrefab) RenderTree->PushPrefab(Actor);

	WeProfiler::Instance()->ObjectsInWorld++;

	return Actor;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  RemoveActor
//         Input Parametars :  Actor
//         Output Parametars:  bool
//         Comments         :  Remove Actor from world but dont destroy it
//
//////////////////////////////////////////////////////////////////////////
bool WeWorld::RemoveActor(WeActor* Actor)
{
	if (!Actor) return false;
	if (Actor->World != this) return false; // cant remove actor that is not in this world

	Actor->PrevActor->NextActor = Actor->NextActor;
	Actor->NextActor->PrevActor = Actor->PrevActor;
	Actor->PrevActor = Actor;
	Actor->NextActor = Actor;

	// Dont clear world mark from actor. Maybe he still want to know in what world he was
//	Actor->World = NULL;
	// Remove it from light list if this is light
	if (Actor->IsLight)
	{
		((WeLight *)Actor)->PrevLight->NextLight = ((WeLight *)Actor)->NextLight;
		((WeLight *)Actor)->NextLight->PrevLight = ((WeLight *)Actor)->PrevLight;
		((WeLight *)Actor)->PrevLight = ((WeLight *)Actor);
		((WeLight *)Actor)->NextLight = ((WeLight *)Actor);
	}

	if (Actor->Type == WETYPE_PATHNODE)
	{
		((WePathNode *)Actor)->PrevPathNode->NextPathNode = ((WePathNode *)Actor)->NextPathNode;
		((WePathNode *)Actor)->NextPathNode->PrevPathNode = ((WePathNode *)Actor)->PrevPathNode;
		((WePathNode *)Actor)->PrevPathNode = ((WePathNode *)Actor);
		((WePathNode *)Actor)->NextPathNode = ((WePathNode *)Actor);
	}
	if (Actor->Type == WETYPE_ZONE)
	{
		for (Zones.First(); !Zones.End(); Zones.Next())
		{
			if (Zones.GetItem() == Actor)
			{
				Zones.Delete();
			}
			break;
		}

	}

	if (Actor->Type == WETYPE_PORTAL)
	{
		for (Portals.First(); !Portals.End(); Portals.Next())
		{
			if (Portals.GetItem() == Actor)
			{
				Portals.Delete();
				break;
			}
		}
	}

	if (Actor->IsPrefab) RenderTree->PopPrefab(Actor);
	WeProfiler::Instance()->ObjectsInWorld--;
	
	return true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeWorld::Render(WeCamera* Cam, bool RenderPostProcess)
{
	if (ZonedRendering && !ZonedRenderingPrepared)
	{
		PrepareZonedRendering();
	}


	WeShaderManager::Instance()->SetViewMatrix(Cam->View);
	WeShaderManager::Instance()->SetProjectionMatrix(Cam->Projection);
	WeShaderManager::Instance()->SetViewProjection(Cam->ViewProj);
	WeShaderManager::Instance()->SetPrevViewProjection(Cam->PrevViewProj);
	WeShaderManager::Instance()->SetEyePosition(Cam->Location);

	WeRenderDevice::Instance()->SetTextureFiltering();

	// Render Actual World
	// find what we see. This will fill queues for WeRelfectionRenderer, WeSkyBoxRenderer
	WeBatchRenderer::Instance()->PrepareQueuedBatches(this, Cam);
	// prepare reflections Env Maps
	//WeReflectionRenderer::Instance()->Render(this, Cam);
	// render to velocity texture if pixel motion blur is turned on 	


	if (RenderPostProcess)
		WeVelocityRenderer::Instance()->RenderQueuedBatches(this, Cam);
	// Render SkyBox
	// render actual world
	// 1. Render ambient world (using ambient techinques)


	if (WeShaderManager::Instance()->GetQuality() == SHADER_LOW)
	{	
		if (WeRenderDevice::Instance()->GetShadows())
		{
			// try to turn on the fog if we have it
			// TODO: Currenntly this part is not tested
			/*WeRenderDevice::Instance()->TurnFogOn();
			SkyBox->OnRender(Cam);
			WeBatchRenderer::Instance()->RenderAmbientBatches(this);	
			WeBatchRenderer::Instance()->RenderAmbientAlphaBatches(this);	
			// turn fog off
			WeRenderDevice::Instance()->TurnFogOff();

			//// 2. Render extruded geometry using shadow renderer(write to stencil double sided)	
			WeShadowVolumeRenderer::Instance()->PrepareQueuedBatches(this, Cam);
			WeShadowVolumeRenderer::Instance()->RenderQueuedBatches(this);

			////// 3. Render normal world with stenciling if we have written something to stencil
			WeRenderDevice::Instance()->SetRenderState(D3DRS_STENCILENABLE, TRUE);	
			WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	
			WeRenderDevice::Instance()->SetRenderState(D3DRS_STENCILREF, 1);
			WeRenderDevice::Instance()->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATER);
			WeRenderDevice::Instance()->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);	
			WeBatchRenderer::Instance()->DrawQueuedBatches(this);	
			WeRenderDevice::Instance()->SetRenderState(D3DRS_STENCILENABLE, FALSE);	

			WeProjectorRenderer::Instance()->PrepareQueuedBatches(this, Cam);
			WeProjectorRenderer::Instance()->RenderQueuedBatches(this);

			// Privremeno iskljuc
			WeBatchRenderer::Instance()->DrawAlphaBatches(this);	
			WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	*/
		}
		else
		{
			if (RenderPostProcess)
				WeRenderDevice::Instance()->TurnFogOn();

			if (RenderPostProcess && RenderPhaseEnabled(RENDERPHASE_SKYBOX))
				SkyBox->OnRender(Cam);

			if (RenderPhaseEnabled(RENDERPHASE_AMBIENT))
				WeBatchRenderer::Instance()->RenderAmbientBatches(this);

			if (RenderPhaseEnabled(RENDERPHASE_AMBIENTALPHA))
				WeBatchRenderer::Instance()->RenderAmbientAlphaBatches(this);

			if (RenderPostProcess)
				WeRenderDevice::Instance()->TurnFogOff();

			WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			if (RenderPhaseEnabled(RENDERPHASE_LIGHTING))
				WeBatchRenderer::Instance()->DrawQueuedBatches(this);	

			//// Projectors
			if (RenderPhaseEnabled(RENDERPHASE_PROJECTORS))
			{
				WeProjectorRenderer::Instance()->PrepareQueuedBatches(this, Cam);
				WeProjectorRenderer::Instance()->RenderQueuedBatches(this);
			}

			if (RenderPhaseEnabled(RENDERPHASE_ALPHA))
				WeBatchRenderer::Instance()->DrawAlphaBatches(this);

			WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	
		}
	} // END SHADER_LOW



	if (WeShaderManager::Instance()->GetQuality() == SHADER_HIGH)
	{
		if (WeRenderDevice::Instance()->GetShadows())		
		{

		} else
		{
			// render standard skybox
//			if (RenderPostProcess && RenderPhaseEnabled(RENDERPHASE_SKYBOX))
//				SkyBox->OnRender(Cam);

			// high version with no shadows at all
			WeBatchRenderer::Instance()->RenderZBuffer(this);
			WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	

			WeRenderDevice::Instance()->TurnFogOn();
			WeBatchRenderer::Instance()->RenderBatchesHigh(this);
			if (RenderPhaseEnabled(RENDERPHASE_PROJECTORS))
			{
				WeProjectorRenderer::Instance()->PrepareQueuedBatches(this, Cam);
				WeProjectorRenderer::Instance()->RenderQueuedBatches(this);
			}
			WeBatchRenderer::Instance()->RenderAlphaBatchesHigh(this);			
			WeRenderDevice::Instance()->TurnFogOff();
			WeRenderDevice::Instance()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	
		}
	} // END SHADER_HIGH

	

	// Render glowing
	if (RenderPostProcess && RenderPhaseEnabled(RENDERPHASE_GLOW))
		WeGlowRenderer::Instance()->RenderQueuedBatches(this, Cam);	

	// Refractions
	if (RenderPostProcess && RenderPhaseEnabled(RENDERPHASE_REFRACTIONS))
		WeBatchRenderer::Instance()->DrawRefractiveBatches(this);

	// Foreground objects
	if (RenderPhaseEnabled(RENDERPHASE_FOREGROUND))
		WeBatchRenderer::Instance()->DrawForegroundBatches(this);

	// Flares
	if (RenderPhaseEnabled(RENDERPHASE_FLARES))
	{
		WeFlareRenderer::Instance()->PrepareQueuedBatches(this, Cam);
		WeFlareRenderer::Instance()->RenderQueuedBatches(this, Cam);
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
void WeWorld::PreRender(WeCamera* Cam)
{
	WeActor *Iterator, *tmp;
	for(Iterator = Actors->NextActor; Iterator != Actors; )
	{
		tmp = Iterator->NextActor;
		Iterator->PreRender(Cam);
		Iterator = tmp;
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
void WeWorld::PostRender(WeCamera* Cam)
{
	WeActor *Iterator, *tmp;
	for(Iterator = Actors->NextActor; Iterator != Actors; )
	{
		tmp = Iterator->NextActor;
		Iterator->PostRender(Cam);
		Iterator = tmp;
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
bool WeWorld::Load(StringParam name)
{	
	// DEPRECATED NOT USED


	WeFileStream FS;
	FILE *f;
	f = fopen(name, "rb");
	if (!f) 
	{
		return false;
	}
	fclose(f);

	if (!FS.Open(name))
	{
		return false;
	}

	long	Loaded, FileSize;
	int		Type;
	float	Percent;
	bool	StopLoading = false;


	isLoading = true;

	FileSize = FS.GetSize();
	Loaded = FS.Tell();

	//Version = FS.ReadInt();
	OnLoadMapHeader(FS);
	SkyBox->Deserialize(FS);
	SkyBox->Initialize();

	while (!FS.Eof())
	{
		// now load other objects
		Type = FS.ReadInt();
		if (FS.Eof()) break;

		switch (Type)
		{
		case WETYPE_ACTOR: 
			WeActor *tmpA;
			tmpA = new WeActor();
			tmpA->InEditor = IsEditor;
			tmpA->Deserialize(FS);	
			tmpA->Initialize();
			tmpA->SetTransformation(tmpA->TM);
			AddActor(tmpA);
			break;
		case WETYPE_LIGHT:
			WeLight *tmpL;
			tmpL = new WeLight;
			tmpL->InEditor = IsEditor;
			tmpL->Deserialize(FS);
			tmpL->Initialize();
			tmpL->SetTransformation(tmpL->TM);
			AddActor(tmpL);
			break;
		case WETYPE_NETWORKACTOR:
			WeNetworkActor *tmpNA;
			tmpNA = new WeNetworkActor;
			tmpNA->InEditor = IsEditor;
			tmpNA->Deserialize(FS);
			tmpNA->Initialize();
			tmpNA->SetTransformation(tmpNA->TM);
			AddActor(tmpNA);
			break;
		/*case WETYPE_PREFAB: 
			WePrefab *tmpP;
			tmpP = new WePrefab;
			tmpP->InEditor = IsEditor;
			tmpP->Deserialize(FS);
			tmpP->Initialize();
			tmpP->SetTransformation(tmpP->TM);
			AddActor(tmpP);
			break;*/
		case WETYPE_PARTICLESYSTEM:
			WeParticleSystem *tmpPS;
			tmpPS = new WeParticleSystem;
			tmpPS->InEditor = IsEditor;
			tmpPS->Deserialize(FS);
			tmpPS->Initialize();
			tmpPS->SetTransformation(tmpPS->TM);
			AddActor(tmpPS);			
			break;
		case WETYPE_PATHNODE:
			WePathNode *tmpPN;
			tmpPN = new WePathNode;
			tmpPN->InEditor = IsEditor;
			tmpPN->Deserialize(FS);
			tmpPN->Initialize();
			tmpPN->SetTransformation(tmpPN->TM);
			AddActor(tmpPN);
			break;
		default:
			OnLoadMapItem(Type, FS);	
		}

		Loaded = FS.Tell();		
		Percent = ((float)Loaded / (float)FileSize) * 100.0f;
		if (OnLoadProgress) OnLoadProgress(Percent, StopLoading);
		if (StopLoading)
		{
			if (OnLoadCanceled) OnLoadCanceled();
			FS.Close();
			isLoaded = false;
			isLoading = false;
			return false;			
		}
	}

	isLoaded = true;
	isLoading = false;

	DoSortPathNodes();

	if (OnMapLoaded) OnMapLoaded();
    	
	 // we have loaded all elements of World than call build to better organize them
	//BuildWorld();
	return true;
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeWorld::LoadInTick(StringParam name, bool LoadToEditor, bool StopLoading)
{
	static WeFileStream FS;

	if (isLoaded) return true;

	if (!isLoading)
	{
		if (!FS.Exists(name)) return false;
		if (!FS.Open(name))	return false;
	}

	long	Loaded, FileSize;
	int		Type;
	bool	SortPathNodes = false;

	if (StopLoading)
	{
		FS.Close();
		isLoading = false;
		isLoaded = false;
		return true;
	}

	isLoading = true;
	FileSize = FS.GetSize();
	Loaded = FS.Tell();

	if (!isHeaderLoaded)
	{
		Version = FS.ReadInt();
		OnLoadMapHeader(FS);
		isHeaderLoaded = true;
		SkyBox->Deserialize(FS);
		SkyBox->Initialize();
	}

	if (!FS.Eof())
	{
		// now load other objects
		Type = FS.ReadInt();
		if (FS.Eof()) 
		{
			FS.Close();
			PercentLoaded = 100.0f;
			isLoaded = true;
			isLoading = false;
			SortPathNodes = true;
		}
		else
		{
			//char TempName[100];
			//sprintf(TempName, "ACTOR %d", Type);
			//SetLoadingName((String)TempName);
			// this is temp and later should be removed... Really later...
			bool WasPrefab = false; 
			if (Type == WETYPE_PREFAB) 
			{
				Type = WETYPE_ACTOR;
				WasPrefab = true;
			}

			switch (Type)
			{
			case WETYPE_ACTOR: 
				WeActor *tmpA;
				tmpA = new WeActor();
				
				tmpA->InEditor = IsEditor;
				tmpA->Deserialize(FS);	
				if (WasPrefab)	
				{
					tmpA->Type = WETYPE_ACTOR;
					tmpA->IsPrefab = true;
					tmpA->PhysicModel.Type = PHYSICMODEL_STATIC_SHAPE;
				}

				tmpA->Initialize();
				tmpA->SetTransformation(tmpA->TM);
				AddActor(tmpA);
				// Load materials for preset it uses
				if (tmpA->Model)
					tmpA->Model->LoadMaterials(tmpA);
				break;

			case WETYPE_CLOTH:				
				WeCloth *tmpC;
				tmpC = new WeCloth();
				tmpC->InEditor = IsEditor;
				tmpC->Deserialize(FS);	
				tmpC->Initialize();				
				AddActor(tmpC);
				break;

			case WETYPE_LIGHT:
				WeLight *tmpL;
				tmpL = new WeLight;
				tmpL->InEditor = IsEditor;
				tmpL->Deserialize(FS);
				tmpL->Initialize();
				tmpL->SetTransformation(tmpL->TM);
				AddActor(tmpL);
				break;

			case WETYPE_NETWORKACTOR:
				WeNetworkActor *tmpNA;
				tmpNA = new WeNetworkActor;
				tmpNA->InEditor = IsEditor;
				tmpNA->Deserialize(FS);
				tmpNA->Initialize();
				tmpNA->SetTransformation(tmpNA->TM);
				AddActor(tmpNA);
				break;

			/*case WETYPE_PREFAB: 
				WePrefab *tmpP;
				tmpP = new WePrefab;
				tmpP->InEditor = IsEditor;
				tmpP->Deserialize(FS);				
				tmpP->Initialize();
				tmpP->SetTransformation(tmpP->TM);
				AddActor(tmpP);
				// Load materials for preset it uses
				if (tmpP->Model) tmpP->Model->LoadMaterials(tmpP);
				break;*/

			case WETYPE_PARTICLESYSTEM:
				WeParticleSystem *tmpPS;
				tmpPS = new WeParticleSystem;
				tmpPS->InEditor = IsEditor;
				tmpPS->Deserialize(FS);
				tmpPS->Initialize();
				tmpPS->SetTransformation(tmpPS->TM);
				AddActor(tmpPS);			
				break;

			case WETYPE_PATHNODE:
				WePathNode *tmpPN;
				tmpPN = new WePathNode;
				tmpPN->InEditor = IsEditor;
				tmpPN->Deserialize(FS);
				tmpPN->Initialize();
				tmpPN->SetTransformation(tmpPN->TM);
				AddActor(tmpPN);
				break;

			case WETYPE_ZONE:
				WeZone *tmpZ;
				tmpZ = new WeZone;
				tmpZ->InEditor = IsEditor;
				tmpZ->Deserialize(FS);
				tmpZ->Initialize();
				tmpZ->SetTransformation(tmpZ->TM);
				AddActor(tmpZ);
				break;

			case WETYPE_PORTAL:
				WePortal *tmpPo;
				tmpPo = new WePortal;
				tmpPo->InEditor = IsEditor;
				tmpPo->Deserialize(FS);
				tmpPo->Initialize();
				tmpPo->SetTransformation(tmpPo->TM);
				AddActor(tmpPo);
				break;

			default:
				WeActor *tmp;
				tmp = OnLoadMapItem(Type, FS);	
				if (tmp)
				{
					tmp->InEditor = IsEditor;
					tmp->Deserialize(FS);
					tmp->Initialize();
					tmp->SetTransformation(tmp->TM);					
					AddActor(tmp);
				}
					
			}

			//ResetLoadingName();
			Loaded = FS.Tell();		
			PercentLoaded = ((float)Loaded / (float)FileSize) * 100.0f;
			// TODO: Make code if StopLoading is set to true
			if (OnLoadProgress) OnLoadProgress(PercentLoaded / 100.0f, StopLoading);
		}
	}

	// Sort PathNodes
	if (SortPathNodes)
		DoSortPathNodes();

	return true;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeWorld::Unload()
{
	isUnloading = true;
	isLoaded	= false;
	isLoading   = false;
	isHeaderLoaded = false;	

	SAFE_DELETE(RenderTree);

	WeActor *I, *tmp;
	for (I = Actors->NextActor; I != Actors;)
	{
		tmp = I->NextActor;
		I->World = NULL;
		SAFE_DELETE(I);
		I = tmp;
	}
	SAFE_DELETE(Actors);
	SAFE_DELETE(Lights);
	SAFE_DELETE(PathNodes);
	//SAFE_DELETE(SkyBox);

	Actors = new WeActor;
	Lights = new WeLight;
	PathNodes = new WePathNode;

	isLoading	=	false;
	isUnloading =	false;
	isLoaded	=	false;
	isHeaderLoaded = false;
	bHasTicked	=	false;
	SkyBox		=	new WeSkyBox;
	Version		=	WORLD_VERSION;
	OnLoadProgress	= NULL;
	OnLoadCanceled	= NULL;
	OnMapLoaded		= NULL;

	// Create list sentinel 
	RenderTree = new WeRenderTree;
	WeBox Box(WeVector(0,0,0), 50000.0f);

	RenderTree->Initialize(this, 15, Box);
	return true;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Save
//         Input Parametars :  name - name of map
//         Output Parametars:  N/A
//         Comments         :  This saves map to file. IF exists it will be overwritten
//
//////////////////////////////////////////////////////////////////////////
bool WeWorld::Save(StringParam name)
{
	WeFileStream FS;
	WeActor *Iterator;

	if (!FS.OpenNew(name)) return false;

	// Save header and then sarialize all actors

	FS.WriteInt(Version);
	OnSaveMapHeader(FS);
	SkyBox->Serialize(FS);
	for (Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		// Write type so we could deserialize actual object when we want
		if (Iterator->Serializable)
		{
			if (!((Iterator->Type == WETYPE_ACTOR) && (Iterator->ModelName == (String)"MapEditor.Geometry.ParticlePlane")))
			{
				FS.WriteInt(Iterator->Type);			
				Iterator->Serialize(FS);
			}
		}
	}	
	FS.Close();
	return true;
}


bool WeWorld::ExportToOBJ(StringParam name)
{
	WeTextStream TS;
	WeActor *Iterator;
	CSmartList <WeMesh*> ExportedMeshes;

	if (!TS.OpenNew(name)) return false;

	TS.WriteString("################################################\n");
	TS.WriteString("#\n");
	TS.WriteString("# UltraTubes map\n");
	TS.WriteString("#\n");
	TS.WriteString("################################################\n");

	ExportedMeshes.Clear();

	TS.WriteString("\n");
	TS.WriteString("# Vertex List1\n");
	TS.WriteString("\n");

	for (Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if (Iterator->IsPrefab && Iterator->Model && !Iterator->Model->IsModel && 
			Iterator->Model->Mesh /*&& Iterator->Model->Mesh->FacesCount*/)
		{
			Iterator->Model->Mesh->ExportVerticesToOBJ(TS, Iterator->TM);
			ExportedMeshes.PushBack(Iterator->Model->Mesh);
		}
	}

	TS.WriteString("\n");
	TS.WriteString("# Face List\n");
	TS.WriteString("\n");

	int GroupIndex = 1;
	int FirstVertex = 1;

	for (ExportedMeshes.First(); !ExportedMeshes.End(); ExportedMeshes.Next())
	{
		ExportedMeshes.GetItem()->ExportFacesToOBJ(TS, GroupIndex, FirstVertex);
		GroupIndex++;
		FirstVertex += ExportedMeshes.GetItem()->VerticesCount;
	}

	TS.Close();
	return true;
}


bool WeWorld::SaveMapHeader(StringParam name)
{
	WeFileStream FS;

	if (!FS.Exists(name)) return false;
	if (!FS.Open(name)) return false;

	FS.WriteInt(Version);
	OnSaveMapHeader(FS);
	FS.Close();
	return true;
}


bool WeWorld::LoadMapHeader(StringParam name)
{
	WeFileStream FS;

	if (!FS.Exists(name)) return false;
	if (!FS.Open(name)) return false;

	Version = FS.ReadInt();
	OnLoadMapHeader(FS);
	FS.Close();
	return true;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeWorld::HasTicked()
{	
	return bHasTicked;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeWorld::Tick()
{
	WeActor *Iterator, *tmp;

	SkyBox->Tick();

	for(Iterator = Actors->NextActor; Iterator != Actors; )
	{
		tmp = Iterator->NextActor;		
		Iterator->Tick(); // maybe want to destroy himself here
		Iterator = tmp;
	}


	WorldFrame++;
	bHasTicked = true;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  BuildWorld
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Builds world
//
//////////////////////////////////////////////////////////////////////////
void WeWorld::BuildWorld()
{
	RenderTree->BuildRenderTree();
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeWorld::NewWorld()
{
	isLoaded = false;
	isLoading = false;
	isHeaderLoaded = false;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeWorld::ClearAll()
{	
	Actors->NextActor = Actors;
	Actors->PrevActor = Actors;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  FindNearsetLights 
//         Input Parametars :  BSphere
//         Output Parametars:  N/A
//         Comments         :  This finds dynamic light that should affect object with this bsphere
//
//////////////////////////////////////////////////////////////////////////
void WeWorld::FindNearestLights(WeSphere &ObjectsSphere, CSmartList <WeLight *> &lightarray, int NumLights, int NumDirectLights, bool SphereIsDynamicActor)
{
	// This will find only one light
	lightarray.Clear();	
    
	if (NumLights == 0 && NumDirectLights == 0) return;	
	//if (NumLights > 1) NumLights = 1;

	WeLight *Iterator, *MinLight = NULL;
	//WeLight *I2, *LI;
	WeSphere tmpS;
	WeVector tmpV;
	float Range, MinRange = 1000000;
	float tmp;
	int pl, dl;
	int LightsFound = 0;
	pl = 0; 
	dl = 0;

    // Then take first NumLights and add them. 
	for(Iterator = Lights->NextLight; Iterator != Lights; Iterator = Iterator->NextLight)
	{
		// if this light is dynamic or affects dynamic 
		if (Iterator->IsDynamic || (Iterator->AffectDynamicActors && SphereIsDynamicActor))
		{
			if (Iterator->LightType == LIGHT_POINT)
			{
				tmpV = ObjectsSphere.Position - Iterator->Location;
				Range = D3DXVec3LengthSq(&tmpV);			
				tmp = (ObjectsSphere.Radius + Iterator->Range)*(ObjectsSphere.Radius + Iterator->Range);
				if (Range <= tmp)
				{
					// This light is visible and should be used
					lightarray.PushBack(Iterator);
					LightsFound++;
				}
			}			
			// TODO: make this 10 to be define ie. constant value
			if (LightsFound > WeEngine::Instance()->Desc.MaxDynamicLights) break;
		}		
	}	
}



void WeWorld::FindSumColorLights(WeVector &Location, WeColor &outSumColor)
{
    // Then take first NumLights and add them. 
	/*WeLight *Iterator;
	float R, Att;
	WeVector tmpV;
	WeColor tmpC;

	outSumColor.r = outSumColor.g = outSumColor.b = outSumColor.a = 0;

	for(Iterator = Lights->NextLight; Iterator != Lights; Iterator = Iterator->NextLight)
	{
		if (Iterator->IsDynamic || Iterator->AffectDynamicActors)
		{
			tmpV = Location - (Iterator->Location);
			R = D3DXVec3LengthSq(&tmpV);			
			
			if (R > Iterator->GetCurrentLightState()->Range*Iterator->GetCurrentLightState()->Range) continue;
			R = D3DXVec3Length(&tmpV);			

			if (R <= Iterator->GetCurrentLightState()->StartFalloffRange) Att = 1.0f;
			else 
			{
				float tmpR;
				float FalloffR;
				FalloffR = Iterator->GetCurrentLightState()->Range - Iterator->GetCurrentLightState()->StartFalloffRange;
				tmpR = R - Iterator->GetCurrentLightState()->StartFalloffRange;
				if (tmpR == 0) Att = 1.0f;
				else Att = 1.0f - tmpR/FalloffR;
			}

			tmpC = Iterator->GetCurrentLightState()->Diffuse;
			tmpC = tmpC * Att;
			outSumColor += tmpC;
		}
	}*/
	//printf("r: %3.3f g: %3.3f b: %3.3f a: %3.3f", outSumColor.r, outSumColor.g, outSumColor.b, outSumColor.a);
}


void WeWorld::CalculateStaticLights(bool SimpleGenerate)
{
	WeActor *Iterator;
	WeLight *LI;
	CSmartList <WeLight*> LS;
	

	Editor_HideDoodads();

	MaxFacesToLight = 0;
	FacesLit = 0;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if (Iterator->IsPrefab) 
		{
			// find lights for this object
			if (Iterator->Model)
			{
				if (!Iterator->Model->IsModel)
				{
					if (Iterator->Model->Mesh)				
						MaxFacesToLight += Iterator->Model->Mesh->GetNumFaces();
				}
			}
		}
	}

	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if (Iterator->IsPrefab) 
		{
			// find lights for this object
			LS.Clear();
			for(LI = Lights->NextLight; LI != Lights; LI = LI->NextLight)
			{
				if (Iterator->BSphere.Intersect(&LI->BSphere))
				{
					LS.PushBack(LI);
				}
			}	
			Iterator->LightMap.Generate(Iterator, this, &LS, SimpleGenerate);
			if (Iterator->LightMap.Stopped) break;			
		}
	}

	Editor_ShowDoodads();
}

void WeWorld::CalculateStaticLights(CSmartList <WeLight *> &Lights, bool SimpleGenerate)
{
	WeActor *Iterator;
	WeLight *LI;
	CSmartList <WeLight*> LS;

	Editor_HideDoodads();

	MaxFacesToLight = 0;
	FacesLit = 0;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if (Iterator->IsPrefab) 
		{
			// find lights for this object
			LS.Clear();
			for (Lights.First(); !Lights.End(); Lights.Next())			
			{
				LI = Lights.GetItem();
				if (Iterator->BSphere.Intersect(&LI->BSphere))
				{
					LS.PushBack(LI);
				}
			}	

			if (!LS.Empty())
			{
				if (Iterator->Model)
				{
					if (!Iterator->Model->IsModel)
					{
						if (Iterator->Model->Mesh)				
							MaxFacesToLight += Iterator->Model->Mesh->GetNumFaces();
					}
				}
			}
		}
	}

	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if (Iterator->IsPrefab) 
		{
			// find lights for this object			
			LS.Clear();
			for (Lights.First(); !Lights.End(); Lights.Next())			
			{
				LI = Lights.GetItem();
				if (Iterator->BSphere.Intersect(&LI->BSphere))
				{
					LS.PushBack(LI);
				}
			}	

			if (!LS.Empty())
			{
				Iterator->LightMap.Generate(Iterator, this, &LS, SimpleGenerate);
				if (Iterator->LightMap.Stopped) break;			
			}
		}
	}

	Editor_ShowDoodads();
}



void WeWorld::CalculateStaticLights(CSmartList <WeActor *> &Actors, bool SimpleGenerate)
{
	WeActor *Iterator;
	WeLight *LI;
	CSmartList <WeLight*> LS;

	Editor_HideDoodads();

	MaxFacesToLight = 0;
	FacesLit = 0;
	for(Actors.First(); !Actors.End(); Actors.Next())		
	{
		Iterator = Actors.GetItem();
		if (Iterator->IsPrefab) 
		{
			if (Iterator->Model)
			{
				if (!Iterator->Model->IsModel)
				{
					if (Iterator->Model->Mesh)				
						MaxFacesToLight += Iterator->Model->Mesh->GetNumFaces();
				}
			}
		}
	}

	for(Actors.First(); !Actors.End(); Actors.Next())		
	{
		Iterator = Actors.GetItem();
		if (Iterator->IsPrefab) 
		{
			// find lights for this object			
			LS.Clear();
			for(LI = Lights->NextLight; LI != Lights; LI = LI->NextLight)
			{
				if (Iterator->BSphere.Intersect(&LI->BSphere))
				{
					LS.PushBack(LI);
				}
			}	

			if (!LS.Empty())
			{
				Iterator->LightMap.Generate(Iterator, this, &LS, SimpleGenerate);
				if (Iterator->LightMap.Stopped) break;			
			}
		}
	}

	Editor_ShowDoodads();
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeActor *WeWorld::FindActor(StringParam Name)
{
	WeActor *Iterator;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if (Iterator->Name == Name) return Iterator;
	}
	return NULL;
}


WePathNode* WeWorld::FindClosestDirectionPathNode(WeVector &Location)
{
	WePathNode *Iterator, *Min;
	WeVector tmpV, XAxis, YAxis, ZAxis;
	float d;
	float mind;

	// to some greater value
	mind = 1000000.0f;
	Min = NULL;

	for(Iterator = PathNodes->NextPathNode; Iterator != PathNodes; Iterator = Iterator->NextPathNode)
	{
		if (Iterator->IsActive)
		{
			Iterator->GetLocalAxis(XAxis, YAxis, ZAxis);
			D3DXVec3Subtract(&tmpV, &Iterator->Location, &Location);
			d = D3DXVec3Dot(&tmpV, &ZAxis);
			// we are not interested in nodes that look in opposite direction from our actual path or are normal to our vector
			if (d <= 0.001f) continue; 

			d = D3DXVec3LengthSq(&tmpV);
			if (d < mind) 
			{
				Min = Iterator;
				mind = d;
			}
		}
	}

	return Min;
}


WePathNode* WeWorld::FindClosestInvDirectionPathNode(WeVector &Location)
{
	WePathNode *Iterator, *Min;
	WeVector tmpV, XAxis, YAxis, ZAxis;
	float d;
	float mind;

	// to some greater value
	mind = 1000000.0f;
	Min = NULL;

	for(Iterator = PathNodes->NextPathNode; Iterator != PathNodes; Iterator = Iterator->NextPathNode)
	{
		if (Iterator->IsActive)
		{
			Iterator->GetLocalAxis(XAxis, YAxis, ZAxis);
			D3DXVec3Subtract(&tmpV, &Location, &Iterator->Location);
			d = D3DXVec3Dot(&tmpV, &ZAxis);
			// we are not interested in nodes that look in opposite direction from our actual path or are normal to our vector
			if (d <= 0.001f) continue; 

			d = D3DXVec3LengthSq(&tmpV);
			if (d < mind) 
			{
				Min = Iterator;
				mind = d;
			}
		}
	}

	return Min;
}


WePathNode* WeWorld::FindNextDirectionPathNode(WeVector &Location)
{
	WePathNode *Next;
	Next = FindClosestDirectionPathNode(Location);

	if (!Next)
		Next = FirstPathNode;

	if (Next && (Next->NodeType == NODETYPE_TELEPORTING) || (Next->NodeType == NODETYPE_TELEPORTED))
		Next = Next->NextNode;

	return Next;
}



void WeWorld::GetRefPathNodes(WeVector &Location, WePathNode *&Node1, WePathNode *&Node2, float &D1, float &D2, WeVector &RefPos)
{
	Node1 = FindClosestInvDirectionPathNode(Location);

	if (!Node1)
		Node1 = FindClosestDirectionPathNode(Location);

	if (!Node1)
		Node1 = FirstPathNode;

	Node2 = Node1->NextNode;

	WeVector W;

	D3DXVec3Subtract(&W, &Location, &Node1->Location);
	D1 = D3DXVec3Dot(&Node1->RefDir, &W);

	D3DXVec3Subtract(&W, &Location, &Node2->Location);
	D2 = D3DXVec3Dot(&Node2->RefDir, &W);

	D3DXVec3Scale(&W, &Node1->RefDir, D1);
	D3DXVec3Add(&RefPos, &Node1->Location, &W);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeWorld::Editor_ShowAll()
{
	WeActor *Iterator;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		Iterator->Editor_Show();
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
void WeWorld::Editor_HideAll()
{
	WeActor *Iterator;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		Iterator->Editor_Hide();
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
void WeWorld::Editor_ShowPathNodes()
{
	WeActor* Iterator;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if(Iterator->Type == WETYPE_PATHNODE) 
		{
			Iterator->Editor_Show();
		}
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
void WeWorld::Editor_HidePathNodes()
{
	WeActor* Iterator;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if(Iterator->Type == WETYPE_PATHNODE) 
		{
			Iterator->Editor_Hide();
		}
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
void WeWorld::Editor_HideZones()
{
	WeActor* Iterator;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if(Iterator->Type == WETYPE_ZONE) 
		{
			Iterator->Editor_Hide();
		}
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
void WeWorld::Editor_ShowZones()
{
	WeActor* Iterator;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if(Iterator->Type == WETYPE_ZONE) 
		{
			Iterator->Editor_Show();
		}
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
void WeWorld::Editor_HidePortals()
{
	WeActor* Iterator;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if(Iterator->Type == WETYPE_PORTAL) 
		{
			Iterator->Editor_Hide();
		}
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
void WeWorld::Editor_ShowPortals()
{
	WeActor* Iterator;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if(Iterator->Type == WETYPE_PORTAL) 
		{
			Iterator->Editor_Show();
		}
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
void WeWorld::Editor_ShowDoodads()
{
	WeActor* Iterator;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if(Iterator->Type == WETYPE_PORTAL || 
		   Iterator->Type == WETYPE_ZONE || 
		   Iterator->Type == WETYPE_PATHNODE || 
		   Iterator->Type == WETYPE_PARTICLESYSTEM || 
		   Iterator->Type == WETYPE_LIGHT) 
		{
			Iterator->Editor_Show();
		}
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
void WeWorld::Editor_HideDoodads()
{
	WeActor* Iterator;
	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		if(Iterator->Type == WETYPE_PORTAL || 
			Iterator->Type == WETYPE_ZONE || 
			Iterator->Type == WETYPE_PATHNODE || 
			Iterator->Type == WETYPE_PARTICLESYSTEM || 
			Iterator->Type == WETYPE_LIGHT) 
		{
			Iterator->Editor_Hide();
		}
	}
}


void WeWorld::ClearAllLights()
{
	WeActor *I, *tmp;
	for (I = Actors->NextActor; I != Actors; )
	{
		if (I->IsLight)
		{
			tmp = I->NextActor;
			RemoveActor(I);			
			I->World = NULL;
			SAFE_DELETE(I);
			I = tmp;
		} else I = I->NextActor;
	}	
	SAFE_DELETE(Lights);	
	Lights = new WeLight;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeWorld::ConnectZones()
{
	for (Portals.First(); !Portals.End(); Portals.Next())
	{
		for (Zones.First(); !Zones.End(); Zones.Next())
		{
			if (Portals.GetItem()->Zone1Name == Zones.GetItem()->Name)
			{
				Portals.GetItem()->Zone1 = Zones.GetItem();
				Zones.GetItem()->Portals.PushBackUnique(Portals.GetItem());
			}
			if (Portals.GetItem()->Zone2Name == Zones.GetItem()->Name)
			{
				Portals.GetItem()->Zone2 = Zones.GetItem();
				Zones.GetItem()->Portals.PushBackUnique(Portals.GetItem());
			}
		}
	}
}



WePathNode* WeWorld::GetPathNode(int SequenceNumber)
{
	for (WePathNode* Iterator = PathNodes->NextPathNode; Iterator != PathNodes; Iterator = Iterator->NextPathNode)
	{
		if (Iterator->SequenceNumber == SequenceNumber)
			return Iterator;
	}

	return NULL;
}



WePathNode* WeWorld::GetNextPathNode(int CurrentNodeNumber, int aNodeType, int LastNodeNumber)
{
	WePathNode *MinPathNode = NULL;

	for (WePathNode* Iterator = PathNodes->NextPathNode; Iterator != PathNodes; Iterator = Iterator->NextPathNode)
	{
		if (Iterator->SequenceNumber > CurrentNodeNumber)
		{
			if ((aNodeType < 0) || (Iterator->NodeType == aNodeType))
			{
				if (!MinPathNode || (Iterator->SequenceNumber < MinPathNode->SequenceNumber))
					MinPathNode = Iterator;
			}
		}
	}

	if (MinPathNode && (LastNodeNumber > 0) && (MinPathNode->SequenceNumber > LastNodeNumber))
		MinPathNode = NULL;

	return MinPathNode;
}


int WeWorld::ProcessPathNodes(WePathNode *FirstNode, int LastNodeNumber, WePathNode **RefLastNode, int TubeID)
// ako je LastNodeNumber == 0, ide se do kraja
{
	int Len = 1;
	WePathNode *PrevNode = FirstNode;
	*RefLastNode = NULL;

	while (true)
	{
		WePathNode *Node;
		Node = GetNextPathNode(PrevNode->SequenceNumber, -1, LastNodeNumber);
		if (!Node)
			break;

		Node->TubeID = TubeID;

		if (Node->NodeType == NODETYPE_NORMAL)
		{
			PrevNode->NextNode = Node;
			Node->PrevNode = PrevNode;
			++TotalNormalPathNodes;
			++TotalPathNodes;
			++Len;

			PrevNode = Node;
		}

		else if (Node->NodeType == NODETYPE_SPLIT)
		{
			WePathNode *Node2, *NodeStart1, *NodeStart2, *NodeEnd1, *NodeEnd2;
			Node2 = GetNextPathNode(Node->SequenceNumber, NODETYPE_CONNECT, LastNodeNumber);
			if (!Node2)
				break;

			if ((Node->LeftNum > Node2->LeftNum) || (Node->RightNum > Node->RightNum))
				break;

			NodeStart1 = GetPathNode(Node->LeftNum);
			NodeStart2 = GetPathNode(Node->RightNum);

			if (!NodeStart1 || !NodeStart2)
				break;

			int Len1, Len2;
			Len1 = ProcessPathNodes(NodeStart1, Node2->LeftNum, &NodeEnd1, 1);
			Len2 = ProcessPathNodes(NodeStart2, Node2->RightNum, &NodeEnd2, 2);

			if (!NodeEnd1 || !NodeEnd2)
				break;

			PrevNode->NextNode = Node;
			Node->PrevNode = PrevNode;
			++TotalPathNodes;
			++Len;

			if (Len1 <= Len2)
			{
				Node->NextNode = NodeStart1;
				NodeStart1->PrevNode = Node;
				Node->NextAltNode = NodeStart2;
				NodeStart2->PrevNode = Node;

				NodeEnd1->NextNode = Node2;
				Node2->PrevNode = NodeEnd1;
				NodeEnd2->NextNode = Node2;
				Node2->PrevAltNode = NodeEnd2;

				Node->AltScale = (float)Len1 / (float)Len2;
				Len += Len1;
			}
			else
			{
				Node->NextNode = NodeStart2;
				NodeStart2->PrevNode = Node;
				Node->NextAltNode = NodeStart1;
				NodeStart1->PrevNode = Node;

				NodeEnd2->NextNode = Node2;
				Node2->PrevNode = NodeEnd2;
				NodeEnd1->NextNode = Node2;
				Node2->PrevAltNode = NodeEnd1;

				Node->AltScale = (float)Len2 / (float)Len1;
				Len += Len2;
			}

			++TotalPathNodes;
			++Len;

			PrevNode = Node2;
		}

		else if (Node->NodeType == NODETYPE_TELEPORTING)
		{
			WePathNode *Node2;
			Node2 = GetNextPathNode(PrevNode->SequenceNumber, NODETYPE_TELEPORTED, LastNodeNumber);
			if (!Node2)
				break;

			PrevNode->NextNode = Node;
			Node->PrevNode = PrevNode;
			++TotalPathNodes;
			++Len;

			Node->NextNode = Node2;
			Node2->PrevNode = Node;
			++TotalPathNodes;
			++Len;

			PrevNode = Node2;
		}
	}

	*RefLastNode = PrevNode;

	return Len;
}



WePathNode* WeWorld::NumerizePathNodes(WePathNode *FirstNode, float *Index, float Step)
{
	WePathNode *Node = FirstNode;

	do {
		if (Node->NodeType != NODETYPE_SPLIT)
		{
			Node->IsActive = true;
			Node->PathIndex = *Index;
			Node->SortedIndex = (int)(Node->PathIndex * TotalPathNodes + 0.5f);
			*Index = *Index + Step;
			Node = Node->NextNode;
		}
		else
		{
			Node->IsActive = true;
			Node->PathIndex = *Index;
			Node->SortedIndex = (int)(Node->PathIndex * TotalPathNodes + 0.5f);
			*Index = *Index + Step;

			float IndexAlt;
			IndexAlt = *Index;
			NumerizePathNodes(Node->NextAltNode, &IndexAlt, Step * Node->AltScale);

			Node = NumerizePathNodes(Node->NextNode, Index, Step);

			Node->IsActive = true;
			Node->PathIndex = *Index;
			Node->SortedIndex = (int)(Node->PathIndex * TotalPathNodes + 0.5f);
			*Index = *Index + Step;

			Node = Node->NextNode;
		}
	} while ((Node != FirstPathNode) && (Node->NodeType != NODETYPE_CONNECT));

	return Node;
}



void WeWorld::DoSortPathNodes()
{
	for (WePathNode* Iterator = PathNodes->NextPathNode; Iterator != PathNodes; Iterator = Iterator->NextPathNode)
		Iterator->IsActive = false;

	TotalPathNodes = 0;
	TotalNormalPathNodes = 0;
	TotalPathLen = 0;

	FirstPathNode = GetNextPathNode(-1, NODETYPE_NORMAL, 0);
	if (FirstPathNode)
	{
		++TotalPathNodes;
		++TotalNormalPathNodes;

		WePathNode *Last;
		TotalPathLen = ProcessPathNodes(FirstPathNode, 0, &Last, 0);
		Last->NextNode = FirstPathNode;
		FirstPathNode->PrevNode = Last;
	}
	else
	{
		WePathNode* ZeroPathNode;
		ZeroPathNode = new WePathNode();
		AddActor(ZeroPathNode);

		ZeroPathNode->NextNode = ZeroPathNode;
		ZeroPathNode->PrevNode = ZeroPathNode;
		FirstPathNode = ZeroPathNode;

		++TotalPathNodes;
		++TotalNormalPathNodes;
		++TotalPathLen;
	}

	float Index = 0.0f;
	NumerizePathNodes(FirstPathNode, &Index, 1.0f / (float)TotalPathLen);

	for (WePathNode* Iterator = PathNodes->NextPathNode; Iterator != PathNodes; )
	{
		if (Iterator->IsActive)
		{
			D3DXVec3Subtract(&Iterator->RefDir, &Iterator->NextNode->Location, &Iterator->Location);
			if (D3DXVec3LengthSq(&Iterator->RefDir) > 0.001f)
				D3DXVec3Normalize(&Iterator->RefDir, &Iterator->RefDir);
			//WeLoger::DebugLog("NODE %d: %.3f %d/%d\n", Iterator->SequenceNumber, Iterator->PathIndex, Iterator->SortedIndex, TotalPathNodes);
			Iterator = Iterator->NextPathNode;
		}
		else
		{
			WePathNode *NextPathNode;
			NextPathNode = Iterator->NextPathNode;
			//RemoveActor(Iterator);
			//delete Iterator;
			Iterator = NextPathNode;
		}
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
void WeWorld::PrepareZonedRendering()
{
	// 0.1 napravi rendertree u zonama	
	ConnectZones();

	for (Zones.First(); !Zones.End(); Zones.Next())
	{
		Zones.GetItem()->MakeRenderTree();
	}

	// 1. pobacaj prefabe u zone. Pa u octree-eve.
	WeActor *Iterator;	

	for(Iterator = Actors->NextActor; Iterator != Actors; Iterator = Iterator->NextActor)
	{
		for (Zones.First(); !Zones.End(); Zones.Next())
		{
			Zones.GetItem()->AddActor(Iterator);
		}
	}	    		
	ZonedRenderingPrepared = true;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
bool WeWorld::GatherCameraZones(WeCamera *Cam, CSmartList <WeZone*> &camZones)
{
	camZones.Clear();

	bool CamInLastZone = (Cam->LastZone ? Cam->LastZone->WorldBox.Intersect(&Cam->Location) : false);

	for (Zones.First(); !Zones.End(); Zones.Next())
	{
		WeZone *zone = Zones.GetItem();
		if (zone->WorldBox.Intersect(&Cam->Location))
		{
			if (!CamInLastZone)
			{
				Cam->LastZone = zone;
				CamInLastZone = true;
			}

			if (zone->UsedFrame != WorldFrame)
				GatherCameraZones(Cam, zone, &Cam->Frustum, camZones);
		}
	}

	if (!Cam->LastZone) return false;
    	
	return true;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void  WeWorld::GatherCameraZones(WeCamera *Cam, WeZone *Parent, WeFrustum *frustum, CSmartList <WeZone*> &camZones)
{
    //1. find all zone portals.
	WeFrustum NewFrustum;
	camZones.PushBack(Parent);
	Parent->Frustums.PushBack(*frustum);	
	Parent->UsedFrame = WorldFrame;
	WeZone *NextZone;

	for (Parent->Portals.First(); !Parent->Portals.End(); Parent->Portals.Next())
	{		
		if (Parent->Portals.GetItem()->Zone1 == Parent)
			NextZone = Parent->Portals.GetItem()->Zone2;            
		else 
			NextZone = Parent->Portals.GetItem()->Zone1;            



		//1. If zone where portal leads is already used then skip this portal
		if ((NextZone->UsedFrame == WorldFrame) ||(NextZone->WorldBox.Intersect(&Cam->Location))) continue;

		//2. make frustum with each portal		
		if (Parent->Portals.GetItem()->ClipPortal(frustum, &NewFrustum))
		{
			//3. where frustum interescts portal go to another portal zone and set this zone new frustum
			GatherCameraZones(Cam, NextZone, &NewFrustum, camZones);
		}	
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
void WeWorld::SetMaxRenderDistanceToAll(float MaxRenderDistance)
{
	WeActor *Iterator, *tmp;

	for(Iterator = Actors->NextActor; Iterator != Actors; )
	{
		tmp = Iterator->NextActor;		
		Iterator->MaxRenderDistance = MaxRenderDistance;
		Iterator = tmp;
	}
}



bool WeWorld::RenderPhaseEnabled(int RenderPhase)
{
	return bRenderPhaseEnabled[RenderPhase];
}



void WeWorld::EnableRenderPhase(int RenderPhase, bool enabled)
{
	bRenderPhaseEnabled[RenderPhase] = enabled;
}
