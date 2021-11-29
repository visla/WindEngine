#include <WeFlareRenderer.h>
#include <WeWorld.h>
#include <WeCamera.h>
#include <WeBatchItem.h>
#include <WeLight.h>
#include <WeRenderableNode.h>
#include <WeRenderTree.h>
#include <WeBatchRenderer.h>
#include <WeShaderBatch.h>
#include <WeRenderDevice.h>
#include <WeShaderManager.h>
#include <WeProfiler.h>
#include <WeMaterial.h>
#include <WeMaterialManager.h>
#include <WeMesh.h>
#include <d3dx9math.h>
#include <WePhysics.h>
#include <WeTimer.h>
#include <WeTextureManager.h>
#include <WeCanvas.h>
#include <WeLoger.h>
#include <math.h>


// taken from http://www.gamedev.net/reference/articles/article813.asp
// flare at -(length/5.5) is not used
float FlaresDistance[5] = {0.5f, 0.333f, 0.125f, -0.25f, -0.5f};
float FlaresSize[5] = {0.5f, 0.25f, 1.0f, 0.25f, 0.5f};

WeFlareRenderer *WeFlareRenderer::_instance = NULL;


WeFlareRenderer *WeFlareRenderer::Instance()
{
	if (_instance) return _instance;
	_instance = new WeFlareRenderer;
	return _instance;
}

bool WeFlareRenderer::Initialize()
{
	if (bInitialized) return true;

	bInitialized = true;
	OnResetDevice();

	RemoveAllLights();

	return true;
}


void WeFlareRenderer::ShutDown()
{
	if (!bInitialized) return;

	RemoveAllLights();

	bInitialized = false;
}


void WeFlareRenderer::OnLostDevice()
{
}

void WeFlareRenderer::OnResetDevice()
{
}



WeFlareRenderer::WeFlareRenderer()
{
	bInitialized = false;
}



void WeFlareRenderer::RemoveLight(WeLight *Light)
{
	for (FlareBatches.First(); !FlareBatches.End(); FlareBatches.Next())
	{
		if (FlareBatches.GetItem()->Light == Light)
		{
			delete FlareBatches.GetItem();
			FlareBatches.Delete();
			break;
		}
	}
}


void WeFlareRenderer::RemoveAllLights()
{
	for (FlareBatches.First(); !FlareBatches.End(); )
	{
		delete FlareBatches.GetItem();
		FlareBatches.Delete();
	}
}


void WeFlareRenderer::PrepareQueuedBatches(WeWorld *world, WeCamera *Camera)
{
	if (!WeRenderDevice::Instance()->GetFlares()) return;

	// find all lights in Camera's frustum
	WeLight *Light;
	for(Light = world->Lights->NextLight; Light != world->Lights; Light = Light->NextLight)
	{		
		if (Light->Corona || Light->Flares)
		{
			bool HasBatch = false;
			WeFlareBatch *batch = NULL;

			// find batch for this Light, if it exists
			for (FlareBatches.First(); !FlareBatches.End(); FlareBatches.Next())
			{
				if (FlareBatches.GetItem()->Light == Light)
				{
					batch = FlareBatches.GetItem();
					HasBatch = true;
					break;
				}
			}

			// calculate Light location and if it is in frustum
			
			D3DXVECTOR4 loc;
			D3DXVec3Transform(&loc, &Light->Location, &Camera->ViewProj);

			D3DXVECTOR3 LightLocation;
			float w = abs(loc.w);
			LightLocation.x = loc.x / w;
			LightLocation.y = loc.y / w;
			LightLocation.z = loc.z / w;

			bool LightInFrustum;
			LightInFrustum =
				(abs(LightLocation.x) < 1) &&
				(abs(LightLocation.y) < 1) &&
				(abs(LightLocation.z - 0.5) < 0.5);

			// calculate if Light is occluded
			bool LightOccluded = true;
			if (LightInFrustum || Light->AlwaysShowFlares)
			{
				// check if Light is occluded
				if (Light->InEditor)
					LightOccluded = false;
				else
				{
					bool ok = false;
					WeRaycastHit Hit;

					WeVector Dir, Location;
					Location = Camera->Location;

					do {
						// calculate Location to Light Direction and Distance
						D3DXVec3Subtract(&Dir, &Light->Location, &Location);
						float Distance = D3DXVec3Length(&Dir);
						D3DXVec3Normalize(&Dir, &Dir);
						LightOccluded = WePhysics::Instance()->RaycastClosest(Location, Dir, WE_SHAPE_TYPE_ALL, Hit, 7, Distance);

						if (!LightOccluded)
							ok = true;
						else if (!Hit.ActorHit->PassFlares)
							ok = true;
						else
						{
							WeVector DeltaDir;
							D3DXVec3Scale(&DeltaDir, &Dir, 0.1f);
							D3DXVec3Add(&Location, &Location, &DeltaDir);
						}
					} while (!ok);
				}
			}

			// update OccludedTime
			if (batch)
			{
				if (LightOccluded)
				{
					if (!batch->Occluded)
						batch->OccludedTime = (1.0f - batch->VisibleTime / batch->Light->FadeInTime) * batch->Light->FadeOutTime;
					batch->OccludedTime += WeTimer::Instance()->GetDeltaTimeF();
				}
				else // !LightOccluded
				{
					if (batch->Occluded)
						batch->VisibleTime = (1.0f - batch->OccludedTime / batch->Light->FadeOutTime) * batch->Light->FadeInTime;
					if (batch->VisibleTime < batch->Light->FadeInTime)
					{
						batch->VisibleTime += WeTimer::Instance()->GetDeltaTimeF();
						if (batch->VisibleTime > batch->Light->FadeInTime)
							batch->VisibleTime = batch->Light->FadeInTime;
					}
				}
			}

			// check if something will be visible
			bool visible;
			visible = LightInFrustum && !LightOccluded;
			if (Light->Corona && batch && (batch->OccludedTime < batch->Light->FadeOutTime) && (LightLocation.z > 0))
				visible = true;

			// if Light is visible, update info
			if (visible)
			{
				if (!HasBatch)
				{
					batch = new WeFlareBatch;
					batch->Light = Light;
					batch->OccludedTime = 0;
					batch->VisibleTime = 0;
				}

				batch->LightLocation = LightLocation;
				batch->InFrustum = LightInFrustum;
				batch->Occluded = LightOccluded;

				if (!HasBatch)
					FlareBatches.PushBack(batch);
			} 
			else if (HasBatch)
			{
				delete FlareBatches.GetItem();
				FlareBatches.Delete();
			}
		}
	}
}


void WeFlareRenderer::RenderQueuedBatches(WeWorld *World, WeCamera *Camera)
{
	if (!WeRenderDevice::Instance()->GetFlares()) return;

	for (FlareBatches.First(); !FlareBatches.End(); FlareBatches.Next())
	{
		WeFlareBatch *batch = FlareBatches.GetItem();

		float W = (float)WeRenderDevice::Instance()->GetResWidth();
		float H = (float)WeRenderDevice::Instance()->GetResHeight();

		// Render Corona
		if (batch->Light->Corona && batch->Light->GetCoronaAlpha() && (!batch->Occluded || (batch->OccludedTime < batch->Light->FadeOutTime)))
		{
			if (!batch->Light->CoronaTexture)
				batch->Light->CoronaTexture = WeTextureManager::Instance()->Load(batch->Light->CoronaTextureName, true);

			if (batch->Light->CoronaTexture)
			{
				int w = (int)(batch->Light->CoronaSizeX * W);
				int h = (int)(batch->Light->CoronaSizeY * H);

				if (batch->Light->CoronaFixedSize)
				{
					D3DXVECTOR4 loc;
					D3DXVec3Transform(&loc, &batch->Light->Location, &Camera->View);

					if (loc.z > 0.001)
					{
						float k = 100.0f / loc.z;
						w = (int)(w * k);
						h = (int)(h * k);
					}
				}

				int Lx = (int)((1.0 + batch->LightLocation.x) * 0.5 * W);
				int Ly = (int)((1.0 - batch->LightLocation.y) * 0.5 * H);

				float Alpha = batch->Light->GetCoronaAlpha();

				if (batch->Occluded)
					Alpha *= (1.0f  - batch->OccludedTime / batch->Light->FadeOutTime);
				else if (batch->VisibleTime < batch->Light->FadeInTime)
					Alpha *= batch->VisibleTime / batch->Light->FadeInTime;

				WeVector Up1;
				D3DXVECTOR4 v;
				Up1 = Camera->Location;
				Up1.y = Up1.y + 1.0f;
				D3DXVec3Transform(&v, &Up1, &Camera->View);

				float angle = 0.0f;
				if (!batch->Light->CoronaRotation)
					angle = atan2(v.x, v.y);

				WeCanvas::Instance()->DrawRectangleRotated(
					(float)(Lx - w / 2.0f), (float)(Ly - h / 2.0f), (float)w, (float)h, angle,
					D3DCOLOR_ARGB((int)(Alpha * 255),255,255,255), batch->Light->CoronaTexture,
					bmAdditive);
			}
		}

		// Render Flares
		if (batch->Light->Flares && !batch->Occluded)
		{
			for (int i = 0; i < LIGHT_FLARE_TEXTURES; i++)
			{
				if (batch->Light->GetFlareAlpha(i))
				{
					if (!batch->Light->FlareTexture[i])
						batch->Light->FlareTexture[i] = WeTextureManager::Instance()->Load(batch->Light->FlareTextureNames[i], true);

					if (batch->Light->FlareTexture[i])
					{
						int w = (int)(batch->Light->FlareSizeX * FlaresSize[i] * W);
						int h = (int)(batch->Light->FlareSizeY * FlaresSize[i] * H);

						int Lx = (int)((1.0 + batch->LightLocation.x * FlaresDistance[i]) * 0.5 * W);
						int Ly = (int)((1.0 - batch->LightLocation.y * FlaresDistance[i]) * 0.5 * H);

						float Alpha = batch->Light->GetFlareAlpha(i);

						WeCanvas::Instance()->DrawRectangle(
							Lx - w / 2, Ly - h / 2, w, h,
							D3DCOLOR_ARGB((int)(Alpha * 255),255,255,255), batch->Light->FlareTexture[i],
							bmAdditive);
					}
				}
			}
		}
	}
}

