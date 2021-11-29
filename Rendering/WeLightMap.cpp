/*
     WeLightMap.cpp

	 Autor: Nebojsa Vislavski
	 Date: 30.1.2006.
	 Version: 1.0

  
*/

#include <WeLightMap.h>
#include <WeRenderDevice.h>
#include <WeWorld.h>
#include <WeActor.h>
#include <WeStaticModel.h>
#include <WeMesh.h>
#include <WeLight.h>
#include <WeTextureManager.h>
#include <WeLightMapper.h>
#include <WePhysics.h>
#include <WeMaterial.h>
#include <WeProfiler.h>

LPDIRECT3DVERTEXBUFFER9 WeLightMap::DefaultColorVB = NULL;

WeLightMap::WeLightMap()
{
	Owner = NULL;
	ColorVB = NULL;
	Version = -1;
	NumVertices = 0;
	LightMapTexture = NULL;
	Initialized = false;
	LUVData = NULL;
}


WeLightMap::~WeLightMap()
{
	WeTextureManager::Instance()->Free(LightMapTexture);
	LightMapTexture = NULL;
}
	
void WeLightMap::Use()
{
	WeRenderDevice::Instance()->SetTexture(5, LightMapTexture);
	//if (!ColorVB) return;	
	//WeRenderDevice::Instance()->Device->SetStreamSource(1, ColorVB, 0, sizeof(D3DCOLOR));
}



void Fill(int x1, int y1, int x2, int y2, int x3, int y3, DWORD *pbits, D3DCOLOR Color, int LightMapSize)
{
	// sort by y
	int t;
	if (y1 > y2)
	{
		t = y1; y1 = y2; y2 = t;
		t = x1; x1 = x2; x2 = t;
	}
	if (y2 > y3)
	{
		t = y2; y2 = y3; y3 = t;
		t = x2; x2 = x3; x3 = t;
	}
	if (y1 > y2)
	{
		t = y1; y1 = y2; y2 = t;
		t = x1; x1 = x2; x2 = t;
	}

	float sx1, sx2, sy;
	int m;
	float dx, dy, n1, n2, n3;	
	
	// if special case
	if (y1 == y2)
	{
		sx1 = x1;
		sx2 = x2;
		sy = y1;

		if (sx1 > sx2) 
		{
			t = sx1; sx1 = sx2; sx2 = t;
		}


		for (int i = sx1; i <= sx2; i++)
		{
			if (i < LightMapSize && y1 < LightMapSize) 
				pbits[(int)i+(int)y1*(int)LightMapSize] = Color;
		}	

		if (y2 == y3)
		{
			if (x2 > x3) 
			{
				t = x2; x2 = x3; x3 = t;				
			}

			for (int i = x2; i <= x3; i++)
			{
				if (i < LightMapSize && y1 < LightMapSize) 
					pbits[(int)i+(int)y1*(int)LightMapSize] = Color;
			}	
		} else
		{
			dx = x3 - x1;
			dy = y3 - y1;
			n1 = dx/dy;

			dx = x3 - x2;
			dy = y3 - y2;
			n2 = dx/dy;

			sx1 = x1 + n1;
			sx2 = x2 + n2;

			for (int j = y1+1; j <= y3; j++)
			{
				if (sx1 > sx2) 
				{
					t = sx1; sx1 = sx2; sx2 = t;
					t = n1; n1 = n2; n2 = t;
				}

				for (int i = sx1; i <= sx2; i++)
				{
					if (i < LightMapSize && j < LightMapSize) 
						pbits[i+j*(int)LightMapSize] = Color;					
				}
				sx1 += n1;
				sx2 += n2;
			}
		}		
	} else if (y2 == y3)
	{
		sx1 = x1;
		sx2 = x1;
		
		dx = x3 - x1;
		dy = y3 - y1;
		n1 = dx / dy;

		dx = x2 - x1;
		dy = y2 - y1;
		n2 = dx / dy;

	
		for (int j = y1; j <= y2; j++)
		{
			if (sx1 > sx2) 
			{
				t = sx1; sx1 = sx2; sx2 = t;
				t = n1; n1 = n2; n2 = t;
			}
			for (int i = sx1; i <= sx2; i++)			
			{
				if (i < LightMapSize && j < LightMapSize) 
					pbits[i+j*(int)LightMapSize] = Color;					
			}
			sx1 += n1;
			sx2 += n2;
		}
	} else
	{
		sx1 = x1;
		sx2 = x1;


		dx = x2 - x1;
		dy = y2 - y1;
		n1 = dx / dy;

		dx = x3 - x1;
		dy = y3 - y1;
		n2 = dx / dy;

	
		for (int j = y1; j <= y2; j++)
		{
			if (sx1 > sx2) 
			{
				t = sx1; sx1 = sx2; sx2 = t;
				t = n1; n1 = n2; n2 = t;
			}

			for (int i = sx1; i <= sx2; i++)			
			{
				if (i < LightMapSize && j < LightMapSize) 
					pbits[i+j*(int)LightMapSize] = Color;					
			}
			sx1 += n1;
			sx2 += n2;
		}

		

		dx = x3 - sx1;
		dy = y3 - y2;
		n1 = dx / dy;

		dx = x3 - sx2;
		dy = y3 - y2;
		n2 = dx / dy;	


		for (int j = y2+1; j <= y3; j++)
		{
			if (sx1 > sx2) 
			{
				t = sx1; sx1 = sx2; sx2 = t;
				t = n1; n1 = n2; n2 = t;
			}

			for (int i = sx1; i <= sx2; i++)			
			{
				if (i < LightMapSize && j < LightMapSize) 
					pbits[i+j*(int)LightMapSize] = Color;					
			}

			sx1 += n1;
			sx2 += n2;
		}		
	}
	
}

void Line(int x, int y, int x2, int y2, WORD *pbits, int LightMapSize)
{
	int t;
	float px, py, dx, dy, n;


	if (x > x2) 
	{
		t = x2; x2 = x; x = t;
		t = y2; y2 = y; y = t;
	}			

	dx = x2 - x;
	dy = y2 - y;

	if (abs(dx) >= abs(dy))
	{

		if (dx != 0) n = dy/dx;
		else n = 1;

		px = x;
		py = y;
		for (int j = x; j < x2; j++)
		{
			px = j;
			py += n;
			if (px < LightMapSize && py < LightMapSize) 
			{
				pbits[(int)px+(int)py*(int)LightMapSize] = 0xffff;
			}
		}				
	} else
	{
		if (y > y2) 
		{			
			t = x2; x2 = x; x = t;
			t = y2; y2 = y; y = t;
		}

		dx = x2 - x;
		dy = y2 - y;

		if (dy != 0) n = dx/dy;
		else n = 1;

		px = x;
		py = y;

		for (int j = y; j < y2; j++)
		{
			px += n;
			py = j;
			if (px < LightMapSize && py < LightMapSize) 
			{
				pbits[(int)px+(int)py*(int)LightMapSize] = 0xffff;
			}
		}				
	}				
}


float NormalDist(float x1, float y1, float x2, float y2)
{
	float dx,dy;
	dx = x2 - x1;
	dy = y2 - y1;
	return sqrtf(dx*dx + dy*dy);
}


void WeLightMap::ProcessLight(WeVector V, WeVector N, WeVector T, CSmartList <WeLight*> *Lights, WeColor &outCol, WeVector NFromNormalMap)
{
	float D, R;
	WeVector L, xv , yv, zv, tmpV1, tmpV2, tmpV3, tmpN;
	WeColor C;
	WeMatrix W2T;
	WeRaycastHit Hit;

	D3DXVec3Normalize(&N, &N);
	D3DXVec3Normalize(&T, &T);

	D3DXVec3TransformCoord(&V, &V, &Owner->TM);
	D3DXVec3Cross(&tmpV1, &T, &N);
	D3DXVec3TransformNormal(&tmpV1, &tmpV1, &Owner->TM);
	D3DXVec3TransformNormal(&N, &N, &Owner->TM);
	D3DXVec3TransformNormal(&T, &T, &Owner->TM);


	D3DXMatrixIdentity(&W2T);
	W2T._11 = T.x;
	W2T._21 = T.y;
	W2T._31 = T.z;
	W2T._12 = tmpV1.x;
	W2T._22 = tmpV1.y;
	W2T._32 = tmpV1.z;
	W2T._13 = N.x;
	W2T._23 = N.y;
	W2T._33 = N.z;
	


	outCol = WeColor(0.0f, 0.0f, 0.0f, 0.0f); // WeRenderDevice::Instance()->GetAmbientColor();
	tmpN = N*0.01f;
	
	//D3DXVec3Lerp(&N, &WeVector(0,0,1), &NFromNormalMap, 3);
	//N = NFromNormalMap;
	for (Lights->First(); !Lights->End(); Lights->Next())
	{

		// =======================================================================
		// Calculations for point lights
		if (!Lights->GetItem()->IsDynamic && Lights->GetItem()->LightType == WeLightType::LIGHT_POINT)
		{
			// if light is not visible then ignore it 
			tmpV1 = V + tmpN;
			L = Lights->GetItem()->Location - V;
			R = D3DXVec3Length(&L);					
			D3DXVec3Normalize(&L, &L);

			// for shadowing
			// ==================
			bool Done = false;
			bool InShadow = false;
			while (true)
			{						
				if (WePhysics::Instance()->RaycastClosest(tmpV1, L, WeShapesType::WE_SHAPE_TYPE_STATIC, Hit, 7, R))
				{
					if (!Hit.ActorHit->LightOccluder)
					{
						WeVector tmpvv;
						tmpvv = Hit.WorldImpact - tmpV1;
						R = R - (D3DXVec3Length(&tmpvv) + 0.02f);
						tmpV1 = Hit.WorldImpact + L*0.02f;												
					} else
					{
						if (Hit.Distance < R-0.01f)
						{
							InShadow = true;
							break;
						}								
					}
				} else
				{
					break;
				}
			}
			//InShadow = false;
			if (InShadow) continue;
			// ============



			if (R > Lights->GetItem()->Range)
			{
				D = 0;
			} else
			{
				float r, rm;
				r = R - Lights->GetItem()->StartFalloff;
				if (r < 0) r = 0;
				rm = Lights->GetItem()->Range - Lights->GetItem()->StartFalloff;
				
				if (rm > 0)	R = 1/pow((r/rm)+1.0f, 4);  
				else R = 1.0f;			

			
				D = D3DXVec3Dot(&L, &N) * R;			
				if (D > 1.0) D = 1.0f;
				if (D < 0.0) D < 0.0f;
			}
			
			C = Lights->GetItem()->GetCurrentLightState()->Diffuse;
			outCol += C*D * Lights->GetItem()->GetCurrentLightState()->Intensity;
		}


		// =======================================================================
		// Calculations for spot lights
		if (!Lights->GetItem()->IsDynamic && Lights->GetItem()->LightType == WeLightType::LIGHT_SPOT)
		{
			// if light is not visible then ignore it 
			tmpV1 = V + tmpN;
			L = Lights->GetItem()->Location - V;
			R = D3DXVec3Length(&L);						
			D3DXVec3Normalize(&L, &L);


			if (WePhysics::Instance()->RaycastClosest(tmpV1, L, WeShapesType::WE_SHAPE_TYPE_STATIC, Hit, 7, R))
			{
				if (Hit.Distance < R-0.001f)
				{
					continue;
				}								
			}

			
			if (R > Lights->GetItem()->Range)
			{
				D = 0;
			} else
			{
				float r, rm;
				r = R - Lights->GetItem()->StartFalloff;
				if (r < 0) r = 0;
				rm = Lights->GetItem()->Range - Lights->GetItem()->StartFalloff;

				if (rm > 0)	R = 1/pow((r/rm)+1.0f, 4);  
				else R = 1.0f;			

				WeVector x,y,z;
				Lights->GetItem()->GetLocalAxis(x,y,z);
				z = -z;
				D3DXVec3Normalize(&z, &z);

				float k;
				k = D3DXVec3Dot(&z, &L);
				if (k > cosf(Lights->GetItem()->SpotAngle)) k = 1.0f;
				else
				{
					if (k < cosf(Lights->GetItem()->SpotAngleFalloff)) k = 0;
					else
					{
						float t1, t2;
						t1 = cosf(Lights->GetItem()->SpotAngle);
						t2 = cosf(Lights->GetItem()->SpotAngleFalloff);						
						k = k - t2;
						t1 = t1 - t2;
						
						if (k < 0) k = 0;
						if (t1 > 0)	
						{
							k = (-k/t1)+1.0f;  
							k = 1-k;
						}
						else k = 1.0f;						
					}
				}
			
				D = D3DXVec3Dot(&L, &N) * R * k;			
				if (D > 1.0) D = 1.0f;
				if (D < 0.0) D < 0.0f;
			}

			C = Lights->GetItem()->GetCurrentLightState()->Diffuse;
			outCol += C*D * Lights->GetItem()->GetCurrentLightState()->Intensity;
		}


		// =======================================================================
		// Calculations for beam static lights
		if (!Lights->GetItem()->IsDynamic && Lights->GetItem()->LightType == WeLightType::LIGHT_STATIC_BEAM)
		{
			// if light is not visible then ignore it 
			Lights->GetItem()->GetLocalAxis(xv,yv,zv);
			tmpV1 = V + tmpN;
			L = Lights->GetItem()->Location - V;			
			R = D3DXVec3Length(&L);	
			L = zv;
			D3DXVec3Normalize(&L, &L);
			D3DXVec3Normalize(&zv, &zv);
			L = -L;

			// check if point is in cylinder.
			float d;
			zv = Lights->GetItem()->Location + zv*Lights->GetItem()->Range;
			zv = zv - Lights->GetItem()->Location;

			WeVector bv1, bv2;
			bv1 = Lights->GetItem()->Location - V;
			D3DXVec3Cross(&bv1, &zv, &bv1);			
			d = D3DXVec3Length(&bv1) / D3DXVec3Length(&zv);
			
			if (d > Lights->GetItem()->SizeX) continue;

			if (WePhysics::Instance()->RaycastClosest(tmpV1, L, WeShapesType::WE_SHAPE_TYPE_STATIC, Hit, 7, R))
			{
				if (Hit.Distance < R-0.001f)
				{
					continue;
				}								
			}

			

			if (R > Lights->GetItem()->Range)
			{
				D = 0;
			} else
			{
				float r, rm;
				r = R - Lights->GetItem()->StartFalloff;
				if (r < 0) r = 0;
				rm = Lights->GetItem()->Range - Lights->GetItem()->StartFalloff;

				if (rm > 0)	R = 1/pow((r/rm)+1.0f, 4);  
				else R = 1.0f;			


				D = D3DXVec3Dot(&L, &N); // * R;			
				if (D > 1.0) D = 1.0f;
				if (D < 0.0) D < 0.0f;
			}

			C = Lights->GetItem()->GetCurrentLightState()->Diffuse;
			outCol += C*D * Lights->GetItem()->GetCurrentLightState()->Intensity;
		}

		// =======================================================================
		// calculations for linear light
		if (!Lights->GetItem()->IsDynamic && Lights->GetItem()->LightType == WeLightType::LIGHT_STATIC_LINEAR)
		{
			// if light is not visible then ignore it 
			WeLight* Light;
			Light = Lights->GetItem();
			Lights->GetItem()->GetLocalAxis(xv,yv,zv);
			
			WeVector s1, s2;
			D3DXVec3Normalize(&zv, &zv);
			s1 = Light->Location - zv*Light->SizeX;
			s2 = Light->Location + zv*Light->SizeX;
			
			float t;
			float incr;
			float koef;
			koef = Light->SizeX;

			incr = 1.0f / koef;
			t = 0;
			while (t <= 1.0)
			{
				tmpV1 = V + tmpN;			
				tmpV2 = s1*(1.0f - t) + s2*t;
				L = tmpV2 - V;
				R = D3DXVec3Length(&L);						
				D3DXVec3Normalize(&L, &L);

				if (WePhysics::Instance()->RaycastClosest(tmpV1, L, WeShapesType::WE_SHAPE_TYPE_STATIC, Hit, 7, R))
				{
					if (Hit.Distance < R-0.001f)
					{
						t += incr;
						continue;
					}								
				} 

				if (R > Lights->GetItem()->Range)
				{
					D = 0;
				} else
				{
					float r, rm;
					r = R - Lights->GetItem()->StartFalloff;
					if (r < 0) r = 0;
					rm = Lights->GetItem()->Range - Lights->GetItem()->StartFalloff;

					if (rm > 0)	R = 1/pow((r/rm)+1.0f, 4);  
					else R = 1.0f;			

				
					D = D3DXVec3Dot(&L, &N) * R;			
					D /= koef;
					if (D > 1.0) D = 1.0f;
					if (D < 0.0) D < 0.0f;

				}

				C = Lights->GetItem()->GetCurrentLightState()->Diffuse;
				outCol += C*D * Lights->GetItem()->GetCurrentLightState()->Intensity;;				

				t += incr;
			}			
		}


		// =======================================================================
		// calculations for area light
		if (!Lights->GetItem()->IsDynamic && Lights->GetItem()->LightType == WeLightType::LIGHT_STATIC_AREA)
		{
			// if light is not visible then ignore it 
			WeLight* Light;
			Light = Lights->GetItem();
			Lights->GetItem()->GetLocalAxis(xv,yv,zv);

			WeVector s1, s2, s3, s4;
			D3DXVec3Normalize(&zv, &zv);
			D3DXVec3Normalize(&xv, &xv);

			s1 = Light->Location - zv*Light->SizeX - xv*Light->SizeY;
			s2 = Light->Location - zv*Light->SizeX + xv*Light->SizeY;
			s3 = Light->Location + zv*Light->SizeX + xv*Light->SizeY;
			s4 = Light->Location + zv*Light->SizeX - xv*Light->SizeY;		

			float a, b;
			float incr;
			float koef;
			koef = Light->SizeX;

			tmpV1 = V + tmpN;			

			incr = 1.0f / sqrtf(koef);

			for (float i = 0.0f; i <= 1.0f; i+= incr)
			{
				for (float j = 0.0f; j <= 1.0f; j+= incr)
				{
					tmpV2 = s1 + j*(s2-s1) + i*(s4-s1);
					L = tmpV2 - V;
					R = D3DXVec3Length(&L);						
					D3DXVec3Normalize(&L, &L);

					if (WePhysics::Instance()->RaycastClosest(tmpV1, L, WeShapesType::WE_SHAPE_TYPE_STATIC, Hit, 7, R))
					{
						if (Hit.Distance < R-0.001f)
						{
							continue;
						}								
					} 

					if (R > Lights->GetItem()->Range)
					{
						D = 0;
					} else
					{
						float r, rm;
						r = R - Lights->GetItem()->StartFalloff;
						if (r < 0) r = 0;
						rm = Lights->GetItem()->Range - Lights->GetItem()->StartFalloff;
						
						if (rm > 0)	R = 1/pow((r/rm)+1.0f, 4);  
						else R = 1.0f;			


						D = D3DXVec3Dot(&L, &N) * R;			
						D /= sqrtf(koef);
						if (D > 1.0) D = 1.0f;
						if (D < 0.0) D < 0.0f;
					}

					C = Lights->GetItem()->GetCurrentLightState()->Diffuse;
					outCol += C*D * Lights->GetItem()->GetCurrentLightState()->Intensity;;				
				}
			}
		}
	}
}

WeColor DecodeColor(WORD Col)
{
	WeColor c;
	WORD t,t2;
	t = Col;

	t2 = t & 0x1f;
	c.b = (float)t2 / 31.0f;
	t = t >> 5;

	t2 = t & 0x3f;
	c.g = (float)t2 / 63.0f;
	t = t >> 6;

	t2 = t & 0x1f;
	c.r = (float)t2 / 31.0f;


	return c;
}

void WeLightMap::BlurLightMap(WORD *pbits, bool BlurBlack)
{
	float x1, x2;
	float y1, y2;
	int Size = 1;
	int Cnt;
	WeColor Col;
	WORD t, t2;
	WeColor c;

	for (int i = 0; i < LightMapSize; i++)
	{
		for (int j = 0; j < LightMapSize; j++)
		{
			y1 = i-Size;
			if (y1 < 0) y1 = 0;
			y2 = i+Size;
			if (y2 > LightMapSize-1) y2 = LightMapSize-1;

			x1 = j-Size;
			if (x1 < 0) x1 = 0;
			x2 = j+Size;
			if (x2 > LightMapSize-1) x2 = LightMapSize-1;

			t = pbits[j+i*(int)LightMapSize];
			c = DecodeColor(t);

			Col = c;
			Cnt = 1;
			if ((c.r == 0 && c.g == 0 && c.b == 0) || BlurBlack)
			{				
				for (int l = x1; l <= x2; l++)
				{
					for (int k = y1; k <= y2; k++)
					{
						t = pbits[l+k*(int)LightMapSize];
						c = DecodeColor(t);

						if (c.r > 0 || c.g > 0 || c.b > 0 || BlurBlack)
						{							
							Col += c;														
							Cnt++;
						}						
					}
				}
			}

			Col /= Cnt;

			WORD r,g,b;
			WORD w;
			if (Col.r > 1.0) Col.r = 1.0f;
			if (Col.g > 1.0) Col.g = 1.0f;
			if (Col.b > 1.0) Col.b = 1.0f;

			r = (int)(31.0f * Col.r);
			g = (int)(63.0f * Col.g);
			b = (int)(31.0f * Col.b);

			w = b | (g << 5) | (r << 11);													
			pbits[j+i*(int)LightMapSize] = w;
		}
	}

}

void SaturateEx(WeColor &Col)
{
	
	if (Col.r < 0.0f) Col.r = 0.0f;
	if (Col.g < 0.0f) Col.g = 0.0f;
	if (Col.b < 0.0f) Col.b = 0.0f;

	if (Col.r > 5.0) Col.r = 5.0f;
	if (Col.g > 5.0) Col.g = 5.0f;
	if (Col.b > 5.0) Col.b = 5.0f;

	//Col /= 2.0f;
    if (Col.r > 0.1f)
		Col.r = (log(5.0f) - log(Col.r) - 4.0f) / -4.0f;
	if (Col.g > 0.1f)
		Col.g = (log(5.0f) - log(Col.g) - 4.0f) / -4.0f;
	if (Col.b > 0.1f)
		Col.b = (log(5.0f) - log(Col.b) - 4.0f) / -4.0f;

}


FILE *f = NULL;
void OpenLogging()
{
	f = fopen("bake.log", "at+");	
}

void CloseLogging()
{

	fflush(f);
	fclose(f);
}


void Log(String &s)
{
   	fprintf(f, "%s\n", s.GetString());
}

void WeLightMap::Generate(WeActor *Actor, WeWorld *World, CSmartList <WeLight *> *Lights, bool SimpleGenerate)
{
	if (!Actor) return;
	if (!Actor->Model) return;
	if (Actor->Model->IsModel) return; // we don`t generate light maps for animated models
	if (!Actor->Model->Mesh) return;
	if (!Lights) return;

	WeMesh *Mesh;
	int NumVertices;

	Mesh = Actor->Model->Mesh;
	
	if (SimpleGenerate)
		LightMapSize = 32;
	else
		LightMapSize = Mesh->LightMapSize;



	Owner = Actor;
	// 1. make texture lightmap.
	WeTextureManager::Instance()->Free(LightMapTexture);
	LightMapTexture = WeTextureManager::Instance()->CreateBlank(LightMapSize, LightMapSize, D3DFMT_R5G6B5, 0, D3DPOOL_MANAGED, true);
	

	// 2. make uv coords for object
	NumVertices = Mesh->GetNumVertices();

	WeNormalMappedMeshVertex *Vertices;
	DWORD *Faces;
	DWORD *Attributes;
	WORD *pbits;
	DWORD *npbits;
	int pitch, size;
	int npitch, nsize;

	float Progress;

	//Lumels = new SLumel[LightMapSize*LightMapSize];
	//ZeroMemory(Lumels, sizeof(SLumel)*LightMapSize*LightMapSize);

	char tmpString[100];
	String LogString;
	OpenLogging();
	sprintf(tmpString, "WeLightMap: <Generate>: Actor model Name: %s, MeshName: %s", Actor->Model->Name.GetString(), Actor->Model->Mesh->Name.GetString());
	LogString = tmpString;
	Log(LogString);
	

	if (Mesh->GetUseNormalMap())
	{	
		Mesh->LockVertexBuffer((void **)&Vertices, D3DLOCK_READONLY);
		Mesh->LockIndexBuffer((void **)&Faces, D3DLOCK_READONLY);
		Mesh->LockAttributeBuffer(&Attributes, D3DLOCK_READONLY);		

		LightMapTexture->Lock((void **)&pbits, size, pitch);		


		ZeroMemory(pbits, size);	// TODO: mozda size*pitch

		sprintf(tmpString, "WeLightMap: <Generate>: VB, IB, AB, LightMapTexture Locked, Lighmap size: %d, %d, pitch: %d", size, pitch);
		LogString = tmpString;
		Log(LogString);



		double step, step2;
		double IncrX, IncrY, Incr, Incr2;
		double au, bu, cu, av, bv, cv;
		DWORD Face[3];
		WeVector finV, finN, finT, V1, V2, N1, N2, T1, T2;
		double finNTexU, finNTexV, NU1, NV1, NU2, NV2;
		double fintexU, fintexV, u1, v1, u2, v2;
		float tx,ty;
		int Attr;
		WeTexture *Tex = NULL, *PrevTex = NULL;
		WeMaterial *Mat;
		bool Locked = false;
		Stopped = false;
				

		sprintf(tmpString, "WeLightMap: <Generate>: Before for (0..%d)", Mesh->GetNumFaces()*3);
		LogString = tmpString;
		Log(LogString);

		for (int i = 0; i < Mesh->GetNumFaces()*3; i+=3)
		{
		sprintf(tmpString, "       FOR ENTERED");
		LogString = tmpString;
		Log(LogString);


			Owner->World->FacesLit++;
			Progress = Owner->World->FacesLit / Owner->World->MaxFacesToLight;

			if (Owner->World->OnBakingProgress) 
				Owner->World->OnBakingProgress(Owner->World->FacesLit, Progress, Stopped);

			if (Stopped) 
			{				
				break;
			}

			//Face[0] = Faces[i];
			//Face[1] = Faces[i+1];
			//Face[2] = Faces[i+2];

			//au = Vertices[Face[0]].lu*LightMapSize;
			//av = Vertices[Face[0]].lv*LightMapSize;
			//bu = Vertices[Face[1]].lu*LightMapSize;
			//bv = Vertices[Face[1]].lv*LightMapSize;
			//cu = Vertices[Face[2]].lu*LightMapSize;
			//cv = Vertices[Face[2]].lv*LightMapSize;

			//Line(au, av, bu, bv, pbits, LightMapSize);
			//Line(bu, bv, cu, cv, pbits, LightMapSize);
			//Line(au, av, cu, cv, pbits, LightMapSize);



			// TODO: This is for normal map calculations
			// lock and find normal map
			//Attr = Attributes[i/3];
			//Mesh->Materials[Owner->MaterialPreset].First();
			//if (Mesh->Materials[Owner->MaterialPreset].GetItemsCount() < Attr)
			//{
			//	MessageBox(0, "ERROR", 0, 0);
			//	return;
			//} else
			//{
			//	for (int j = 0; j < Attr; j++)
			//	{
			//		Mesh->Materials[Owner->MaterialPreset].Next();
			//	}
			//	Mat = Mesh->Materials[Owner->MaterialPreset].GetItem();
			//	
			//	if (Mat)
			//	{
			//		Tex = Mat->Textures[WESAMPLER_NORMALMAP];
			//		if (Tex)
			//		{					
			//			Tex->Lock((void **)&npbits, nsize, npitch);
			//			//char tmp[100];
			//			//sprintf(tmp, "Width %d, %d", Tex->Width, npitch);
			//			//MessageBox(0, tmp, 0, 0);

			//			Locked = true;

			//			if (PrevTex != Tex)
			//			{
			//				if (PrevTex) PrevTex->UnLock();
			//				PrevTex = Tex;
			//			}
			//		}
			//	}
			//}			

			
			// line 1-2
			Face[0] = Faces[i];
			Face[1] = Faces[i+1];
			Face[2] = Faces[i+2];

			au = Vertices[Face[0]].lu;
			av = Vertices[Face[0]].lv;
			bu = Vertices[Face[1]].lu;
			bv = Vertices[Face[1]].lv;
			cu = Vertices[Face[2]].lu;
			cv = Vertices[Face[2]].lv;


			IncrX = 1.0 / (NormalDist(au,av,bu,bv)*LightMapSize);
			IncrY = 1.0 / (NormalDist(cu,cv,bu,bv)*LightMapSize);
			if (IncrX < IncrY) Incr = IncrX;
			else Incr = IncrY;

			step = 0;
			sprintf(tmpString, "	Begin face calculations: incr, incrY: %d, %d, LightMapSize: %d", IncrX, IncrY, LightMapSize);
			LogString = tmpString;
			Log(LogString);




			// added && step >= 0.0f 
			while (step <= 1.0 && step >= 0.0f)
			{
				// interpolate pos, normal, lu, lv coords to create scanline
				// then interpolate across that line
				V1 = Vertices[Face[0]].Position*(1.0-step) + Vertices[Face[1]].Position*step;
				V2 = Vertices[Face[2]].Position*(1.0-step) + Vertices[Face[1]].Position*step;
				N1 = Vertices[Face[0]].Normal*(1.0-step) + Vertices[Face[1]].Normal*step;
				N2 = Vertices[Face[2]].Normal*(1.0-step) + Vertices[Face[1]].Normal*step;
				T1 = Vertices[Face[0]].Tangent*(1.0-step) + Vertices[Face[1]].Tangent*step;
				T2 = Vertices[Face[2]].Tangent*(1.0-step) + Vertices[Face[1]].Tangent*step;
				NU1 = Vertices[Face[0]].u*(1.0-step) + Vertices[Face[1]].u*step;
				NU2 = Vertices[Face[2]].u*(1.0-step) + Vertices[Face[1]].u*step;
				NV1 = Vertices[Face[0]].v*(1.0-step) + Vertices[Face[1]].v*step;
				NV2 = Vertices[Face[2]].v*(1.0-step) + Vertices[Face[1]].v*step;
				

				u1 = au*(1.0-step) + bu*step;
				v1 = av*(1.0-step) + bv*step;
				u2 = cu*(1.0-step) + bu*step;
				v2 = cv*(1.0-step) + bv*step;
				
				Incr2 = 1.0 / (NormalDist(u1,v1,u2,v2)*LightMapSize);
				
				step2 = 0.0;

				WeColor Col;
				WeVector NFromNormalMap;

				sprintf(tmpString, "	Before While(step2 <= 1.0): incrX, incrY: %d, %d, LightMapSize: %d", IncrX, IncrY, LightMapSize);
				LogString = tmpString;
				Log(LogString);
				


				// && step2 >= 0.0f added
				while (step2 <= 1.0 && step2 >= 0.0f)
				{
					finV = V1*(1.0-step2) + V2*step2;
					finN = N1*(1.0-step2) + N2*step2;
					finT = T1*(1.0-step2) + T2*step2;
					fintexU = u1*(1.0-step2) + u2*step2;
					fintexV = v1*(1.0-step2) + v2*step2;
					finNTexU = NU1*(1.0-step2) + NU2*step2;
					finNTexV = NV1*(1.0-step2) + NV2*step2;
					
					// get normal from normal map
					//if (Tex)
					//{
					//	WeColor Normal;
					//	int x,y;
					//	
					//	finNTexU = finNTexU - floor(finNTexU);
					//	finNTexV = finNTexV - floor(finNTexV);

					//	x = finNTexU*Tex->Width;
					//	y = finNTexV*Tex->Height;
					//	
					//	if (x > Tex->Width-1) x = Tex->Width-1;
					//	if (y > Tex->Height-1) y = Tex->Height-1;

					//	Normal = npbits[x+y*Tex->Width];
					//	NFromNormalMap.x = 2.0f*Normal.r - 1.0f;
					//	NFromNormalMap.y = 2.0f*Normal.g - 1.0f;
					//	NFromNormalMap.z = 2.0f*Normal.b - 1.0f;						
					//} else
					//{
					//	NFromNormalMap = WeVector(0,0,1);
					//}
					NFromNormalMap = WeVector(0,0,1);

					// use this normal from normal map		
					sprintf(tmpString, "		Before processing pixel");
					LogString = tmpString;
					Log(LogString);

					ProcessLight(finV, finN, finT, Lights, Col, NFromNormalMap);

					sprintf(tmpString, "		After processing pixel");
					LogString = tmpString;
					Log(LogString);

					tx = (fintexU + 0.5/LightMapSize) *LightMapSize;
					ty = (fintexV + 0.5/LightMapSize) *LightMapSize;

					if (tx < 0) tx = 0;
					if (tx > LightMapSize) tx = LightMapSize;
					if (ty < 0) ty = 0;
					if (ty > LightMapSize) ty = LightMapSize;

					// encode it in 565 format
					WORD r,g,b;
					WORD w;
					

					SaturateEx(Col);


					r = (int)(31.0f * Col.r);
					g = (int)(63.0f * Col.g);
					b = (int)(31.0f * Col.b);

					w = b | (g << 5) | (r << 11);										
					pbits[(int)tx + (int)ty*(int)LightMapSize] = w;
					//pbits[(int)tx + (int)ty*(int)LightMapSize] = 0xffff;
					step2 += Incr2;
				}
				step += Incr;
			}
			sprintf(tmpString, "       FOR END");
			LogString = tmpString;
			Log(LogString);


		}
		// TODO: Remove if you want to experiment with normal mapping lightmap
		//if (PrevTex) PrevTex->UnLock();
		

		sprintf(tmpString, "BLURING");
		LogString = tmpString;
		Log(LogString);

		BlurLightMap(pbits, false);
		BlurLightMap(pbits, true);

		sprintf(tmpString, "UNLOCKING");
		LogString = tmpString;
		Log(LogString);

		//FillMemory(pbits, size, 0xff);
		LightMapTexture->UnLock();			
		Mesh->UnlockIndexBuffer();		
		Mesh->UnlockVertexBuffer();
	}

	// 3. bake into lightmap texture
	sprintf(tmpString, "UNLOCKING and ending <Generate>");
	LogString = tmpString;
	Log(LogString);


	CloseLogging();
	//delete[] Lumels;
}






void WeLightMap::GenerateDefault(WeActor *Actor)
{
	//if (!Actor) return;
	//if (!Actor->Model) return;
	//if (!Actor->Model->Mesh) return;

	//SAFE_RELEASE(ColorVB);

	//NumVertices = Actor->Model->Mesh->GetNumVertices();

	//if (FAILED(WeRenderDevice::Instance()->Device->CreateVertexBuffer(NumVertices*sizeof(D3DCOLOR), 0, 0, D3DPOOL_MANAGED, &ColorVB, NULL)))
	//{
	//	MessageBox(0, "Error Generate default", 0, 0);
	//	ColorVB = NULL;
	//	return;
	//} 


	//D3DCOLOR *data;
	//ColorVB->Lock(0, 0, (void **)&data, 0);
	//for (int i = 0; i < NumVertices; i++)
	//{
	//	data[i] = D3DCOLOR_XRGB(255,255,255);
	//}
	//ColorVB->Unlock();
}




void WeLightMap::Serialize(WeFileStream &FS)
{
	static int lnum = 0;

	if (!FS.IsOpened) return;
	WeMesh *Mesh;	
	FS.WriteInt(Version);

	// write texture data
	if (!LightMapTexture || !Owner)
	{
		FS.WriteInt(0);  // zero sized lightmap or no owner with u,v`s
	} else
	{
		//Mesh = Owner->Model->Mesh;
		//char tmp[100];
		//sprintf(tmp, "%s%d.dds", Mesh->Name.GetString(), lnum);
		//lnum++;
		//D3DXSaveTextureToFile(tmp, D3DXIFF_DDS, (LPDIRECT3DTEXTURE9)LightMapTexture->GetTexture(), NULL);
		void *data;
		int size, pitch;

		LightMapTexture->Lock(&data, size, pitch);
		FS.WriteInt(size);
		FS.WriteInt(LightMapTexture->Width);
		FS.WriteInt(LightMapTexture->Height);
		FS.WriteInt(LightMapTexture->Format);
		FS.WriteData(data, size);
		LightMapTexture->UnLock();

	/*	Mesh = Owner->Model->Mesh;		
		if (Mesh->GetUseNormalMap())
		{
			WeNormalMappedMeshVertex *Data;
			Mesh->LockVertexBuffer((void **)&Data, D3DLOCK_READONLY);
			FS.WriteInt(Mesh->GetNumVertices());
			for (int i = 0; i < Mesh->GetNumVertices(); i++)
			{
				FS.WriteFloat(Data[i].lu);
				FS.WriteFloat(Data[i].lv);
			}
			Mesh->UnlockVertexBuffer();
		} else
		{
			WeActualMeshVertex *Data;
			Mesh->LockVertexBuffer((void **)&Data, D3DLOCK_READONLY);
			FS.WriteInt(Mesh->GetNumVertices());
			for (int i = 0; i < Mesh->GetNumVertices(); i++)
			{
				FS.WriteFloat(Data[i].lu);
				FS.WriteFloat(Data[i].lv);
			}
			Mesh->UnlockVertexBuffer();
		}		*/
	}

	//if (!ColorVB)
	//{
	//	FS.WriteInt(0);
	//	return;
	//}

	//D3DCOLOR *data;
	//ColorVB->Lock(0, 0, (void **)&data, D3DLOCK_READONLY);

	//FS.WriteInt(NumVertices);
	//for (int i = 0; i < NumVertices; i++)
	//{
	//	FS.WriteDWord(data[i]);
	//}
	//ColorVB->Unlock();
}


void WeLightMap::Initialize()
{ 
	if (!LUVData) return;
	if (!Owner) return;

	WeMesh *Mesh;
	int NumVertices;

	Mesh = Owner->Model->Mesh;
	NumVertices = Mesh->GetNumVertices();

	if (Mesh->GetUseNormalMap())
	{
		WeNormalMappedMeshVertex *Vertices, *orig;

		orig = (WeNormalMappedMeshVertex*)malloc(sizeof(WeNormalMappedMeshVertex)*NumVertices);
		Mesh->LockVertexBuffer((void **)&Vertices, D3DLOCK_READONLY);
		memcpy(orig, Vertices, sizeof(WeNormalMappedMeshVertex)*NumVertices);
		Mesh->UnlockVertexBuffer();

		Mesh->LockVertexBuffer((void **)&Vertices, D3DLOCK_DISCARD);
		for (int i = 0; i < NumVertices; ++i)
		{
			Vertices[i] = orig[i];
			Vertices[i] = orig[i];
			Vertices[i].lu = LUVData[i].lu;
			Vertices[i].lv = LUVData[i].lv;
		}
		Mesh->UnlockVertexBuffer();
		free(orig);
	}
	else
	{
		WeActualMeshVertex *Vertices2;
		Mesh->LockVertexBuffer((void **)&Vertices2, D3DLOCK_READONLY);

		for (int i = 0; i < NumVertices; ++i)
		{
			Vertices2[i].lu = LUVData[i].lu;
			Vertices2[i].lv = LUVData[i].lv;
		}
		Mesh->UnlockVertexBuffer();
	}	


}


void WeLightMap::Deserialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;
	int Ver;

	Ver = FS.ReadInt();
	if (Ver > 0)
	{
		NumVertices = Ver;
		if (NumVertices == 0) 
		{
			printf("Nema lightmape\n");
			return;
		}

		SAFE_RELEASE(ColorVB);
		if (FAILED(WeRenderDevice::Instance()->Device->CreateVertexBuffer(NumVertices*sizeof(D3DCOLOR), 0, 0, D3DPOOL_MANAGED, &ColorVB, NULL)))
		{
			MessageBox(0, "Error Generate default", 0, 0);
			ColorVB = NULL;
		} 


		D3DCOLOR *data;
		ColorVB->Lock(0, 0, (void **)&data, 0);
		for (int i = 0; i < NumVertices; i++)
		{
			data[i] = FS.ReadDWord();
		}
		ColorVB->Unlock();
		SAFE_RELEASE(ColorVB);
	} // end of default version

	if (Ver == -1)
	{
		int w,h,size;
		D3DFORMAT format;

		size = FS.ReadInt();
		if (size > 0)
		{
			w = FS.ReadInt();
			h = FS.ReadInt();
			format = (D3DFORMAT)FS.ReadInt();
			LightMapTexture = WeTextureManager::Instance()->CreateBlank(w, h, format, 0, D3DPOOL_MANAGED, true);
			void *Data;
			void *texdata;
			int tmp1, tmp2;

			Data = malloc(size);
			FS.ReadData(Data, size);
			LightMapTexture->Lock(&texdata, tmp1, tmp2);
			memcpy(texdata, Data, size);
			LightMapTexture->UnLock();
			free(Data);

			//int NumVertices;
			//NumVertices = FS.ReadInt();

			//if (LUVData)
			//	delete[] LUVData;

			//LUVData = new WeLightMap::SUV[NumVertices];
			//for (int i = 0; i < NumVertices; ++i)
			//{								
			//	LUVData[i].lu = FS.ReadFloat();
			//	LUVData[i].lv = FS.ReadFloat();
			//}
		}	
	}  // end of version -1	
}

