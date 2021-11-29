/*
		File: WePortal.cpp
		Author: Tomislav Kukic
		Date: 05.07.2006
		Version: 1.0
*/


#include <WePortal.h>
#include <WePhysics.h>
#include <WeFrustum.h>



WePortal::WePortal()
{
	//SpecialModelName = "MapEditor.Geometry.Portal";
	Type = WETYPE_PORTAL;
	PortalVer = 1;	
	Zone1 = NULL;
	Zone2 = NULL;
	Zone1ID = -1;
	Zone2ID = -1;
	TwoWayPortal = true;
	SizeX = 1.0f;
	SizeY = 1.0f;
	//CanBePrefab = false;
}



WePortal::~WePortal()
{
}



void WePortal::Initialize()
{
	if (InEditor)
	{
		WeActor::Initialize();

		//D3DXMatrixIdentity(&TM);

		//TM._11 = 1.0f;
		//TM._22 = 1.0f;
		//TM._33 = 1.0f;
		//TM._41 = Location.x;
		//TM._42 = Location.y;
		//TM._43 = Location.z;
		//Model->ComputeBoundingBox(this);
		//// make this sclaing be 10m
		////OrigScale.x = 10.0f / (BBox.Max.x - BBox.Min.x);
		////OrigScale.y = 10.0f / (BBox.Max.y - BBox.Min.y);
		////OrigScale.z = 10.0f / (BBox.Max.z - BBox.Min.z);

		SAFE_DELETE(PhysicHandle);
		WeVector v;
		v.x = SizeX / 2.0f;
		v.y = SizeY / 2.0f;
		v.z = 1.0f;
		PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, v, 10, 10, false, 0, false);		
		PhysicHandle->SetKinematic(true);
	} else
	{
		Renderable = false;
	}
	// Take vertices from portal
	WeVector V;
	V.x = -.5f; V.y = -.5f; V.z = 0.0f;
	PortalVertices.PushBack(V);
	V.x = -.5f; V.y = .5f; V.z = 0.0f;
	PortalVertices.PushBack(V);
	V.x = .5f; V.y = .5f; V.z = 0.0f;
	PortalVertices.PushBack(V);
	V.x = .5f; V.y = -.5f; V.z = 0.0f;
	PortalVertices.PushBack(V);

	Vertices[0] = WeVector(-.5f, -.5f, 0.0f);
	Vertices[1] = WeVector(-.5f, .5f, 0.0f);
	Vertices[2] = WeVector(.5f, .5f, 0.0f);
	Vertices[3] = WeVector(.5f, -.5f, 0.0f);
}





void WePortal::Clone(WeActor* Destination, bool DeepCopy)
{
	WeActor::Clone(Destination, DeepCopy);

	WePortal *tmp;
	tmp = (WePortal *)Destination;
	tmp->Zone1 = Zone1;
	tmp->Zone2 = Zone2;
	tmp->Zone1Name = Zone1Name;
	tmp->Zone2Name = Zone2Name;
	tmp->Zone1ID = Zone1ID;
	tmp->Zone2ID = Zone2ID;	
	tmp->SizeX = SizeX;
	tmp->SizeY = SizeY;
}



bool WePortal::ClipPortal(WeFrustum *frustum, WeFrustum *clippedFrustum)
{
	for (int iPlane = 5; iPlane >= 0; iPlane--)
		// going from far plane downto near, to check portal against far and near plane first
	{
		float minDist = -1; // distance from frustum plane to the nearest portal vertex on the positive side
		int nearestVertex; // index of the nearest portal vertex
		int nPositive = 0; // number of protal vertices on the positive side of current frustum plane

		//FILE *f;
		//f = fopen("portal.log", "at");

		//fprintf(f, "Camera: %3.3f, %3.3f, %3.3f\n",
			//frustum->CameraLocation.x, frustum->CameraLocation.y, frustum->CameraLocation.z);

		// original frustum plane; near plane is slided along normal to camera eye position
		D3DXPLANE *frustumPlane;

		if (iPlane == 4) // for near plane, clip at camera eye
		{
			// generate near plane normal
			D3DXPLANE *nearOriginalPlane = &frustum->Planes[iPlane];
			D3DXVECTOR3 planeNormal;
			planeNormal.x = nearOriginalPlane->a;
			planeNormal.y = nearOriginalPlane->b;
			planeNormal.z = nearOriginalPlane->c;

			// calculate near plane
			D3DXPLANE nearPlane;
			D3DXPlaneFromPointNormal(&nearPlane, &frustum->CameraLocation, &planeNormal);
			frustumPlane = &nearPlane;
		}
		else
			frustumPlane = &frustum->Planes[iPlane];
		

		//fprintf(f, "Plane[%d]: %3.3f, %3.3f, %3.3f, %3.3f\n", iPlane, 
			//frustumPlane->a, frustumPlane->b, frustumPlane->c, frustumPlane->d);

		for (int iVertex = 0; iVertex < 4; iVertex++)
		{
			// calculate distance of portal vertex from frustum plane
			float d = D3DXPlaneDotCoord(frustumPlane, &Vertices[iVertex]);

			//fprintf(f, "   Vertex[%d]: %3.3f, %3.3f, %3.3f, D: %3.3f\n", iVertex, 
				//Vertices[iVertex].x, Vertices[iVertex].y, Vertices[iVertex].z, d);

			if (d >= -0.1) // should be 0, but we have some tollerance
			{
				nPositive++;
				if ((minDist < 0) || (d < minDist))
				{
					minDist = d;
					nearestVertex = iVertex;
				}
			}
		}

		// if all vertices are on the negative side of frustum plane,
		// portal is not visible inside frustum
		if (nPositive == 0)
			return false;

		// for left/right/bottom/top, if all portal vertices are on the positive side,
		// clip to the nearest portal vertex
		if ((iPlane < 4) && (nPositive == 4))
		{
			if (frustum->Projection._34 != 0) // perspective projection
			{
				// calculate up-vector for clipped frustum plane
				// this is vector which is normal to (Vertices[iVertex] - frustum->CameraLocation),
				// and is parallel with both original and clipped frustum planes
				D3DXVECTOR3 upVector;
				if (iPlane == 0) // left -> use view +Y
				{
					upVector.x = frustum->View._12;
					upVector.y = frustum->View._22;
					upVector.z = frustum->View._32;
				}
				else if (iPlane == 1) // right -> use view -Y
				{
					upVector.x = -frustum->View._12;
					upVector.y = -frustum->View._22;
					upVector.z = -frustum->View._32;
				}
				else if (iPlane == 2) // bottom -> use view -X
				{
					upVector.x = -frustum->View._11;
					upVector.y = -frustum->View._21;
					upVector.z = -frustum->View._31;
				}
				else // top -> use view +X
				{
					upVector.x = frustum->View._11;
					upVector.y = frustum->View._21;
					upVector.z = frustum->View._31;
				}

				// calculate Z-vector for clipped frustum plane
				D3DXVECTOR3 zVector;
				D3DXVec3Subtract(&zVector, &Vertices[nearestVertex], &frustum->CameraLocation);

				// calculate normal for clipped frustum plane
				D3DXVECTOR3 planeNormal;
				D3DXVec3Cross(&planeNormal, &upVector, &zVector);

				// create clipped frustum plane
				D3DXPlaneFromPointNormal(&clippedFrustum->Planes[iPlane], &frustum->CameraLocation, &planeNormal);
			}
			else { // orthogonal projection
				D3DXPLANE *plane = &frustum->Planes[iPlane];

				// generate plane normal
				D3DXVECTOR3 planeNormal;
				planeNormal.x = plane->a;
				planeNormal.y = plane->b;
				planeNormal.z = plane->c;

				// generate clipped frustum plane
				D3DXPlaneFromPointNormal(&clippedFrustum->Planes[iPlane], &Vertices[nearestVertex], &planeNormal);
			}
		}
		else // otherwise, do not clip plane
			clippedFrustum->Planes[iPlane] = frustum->Planes[iPlane];

		//fprintf(f, "Plane[%d]: %3.3f, %3.3f, %3.3f, %3.3f -> %3.3f, %3.3f, %3.3f, %3.3f\n", iPlane, 
			//frustumPlane->a, frustumPlane->b, frustumPlane->c, frustumPlane->d,
			//clippedFrustum->Planes[iPlane].a, clippedFrustum->Planes[iPlane].b,
			//clippedFrustum->Planes[iPlane].c, clippedFrustum->Planes[iPlane].d);

		//fflush(f);
		//fclose(f);
	}

	return true;

/* OLD by Visla
		// clip and make new vertices if we need
		// check if protal is visible inside frustum
		//{
			//FILE *f;
			//f = fopen("Nesto.txT", "wt+");
			//for (int i = 0; i < 4; i++)
			//{			
			//	d1 = D3DXPlaneDotCoord(&frustum->Planes[j], &Vertices[i]);
			//	fprintf(f, "Plane[%d]: %3.3f, %3.3f, %3.3f, %3.3f, Vertex: %3.3f, %3.3f, %3.3f, D: %3.3f\n",j, 
			//		frustum->Planes[j].a, frustum->Planes[j].b, frustum->Planes[j].c, frustum->Planes[j].d, 
			//		Vertices[i].x, Vertices[i].y, Vertices[i].z, d1);
			//}			
			//fflush(f);
			//fclose(f);
			//return false; // all vertices are on opposite side of plane so quit
		//}
	}
*/

/* OLD by Visla

	//// first vertex is in array
	//WeVector v1, v2;
	//WeVector tmpV, W[2];
	//float tmp1, tmp2, t[2];
	//int k;
	//bool clipped = false;

	//for (int i = 0; i < 4; i++)
	//{	
	//	k = 0;
	//	clipped = false;
	//	
	//	for (int j = 0; j < 6; j++)
	//	{
	//		v1 = Vertices[i];
	//		if (i < 3) v2 = Vertices[i+1];
	//		else v2 = Vertices[0];

	//		d1 = D3DXPlaneDotCoord(&frustum->Planes[j], &v1);
	//		d2 = D3DXPlaneDotCoord(&frustum->Planes[j], &v2);

	//		if ((d1 < -0.01f && d2 > 0.01f) || (d2 < -0.01f && d1 > 0.01f))  
	//		{
	//			// need to clip vertices
	//			tmp1 = -d1;
	//			tmpV = v2 - v1;				
	//			tmp2 = D3DXPlaneDotNormal(&frustum->Planes[j], &tmpV);				
	//			t[k] = tmp1 / tmp2;				

	//			if (tmp1 > 0 && tmp1 < 1.0f)
	//			{
	//				W[k] = v1 + t[k]*tmpV;
	//				k++;					
	//				clipped = true;
	//			}
	//		} 
	//	}

	//	// add clipped vertices adding one with smallest t first
	//	if (clipped)
	//	{
	//		if (k > 1) 
	//		{
	//			if (t[0] > t[1]) 
	//			{
	//				ClippedPortalVertices.PushBack(W[1]);
	//				ClippedPortalVertices.PushBack(W[0]);
	//			} else
	//			{
	//				ClippedPortalVertices.PushBack(W[0]);
	//				ClippedPortalVertices.PushBack(W[1]);
	//			}
	//		} else ClippedPortalVertices.PushBack(W[0]);
	//	} else
	//	{ 		
	//		if (i < 3)	ClippedPortalVertices.PushBack(Vertices[i+1]);
	//	}
	//}
	*clippedFrustum = *frustum;
*/

/* OLD by Alexa

	// IDEA: for left/right/top/bottom frustum plane, clip it to the nearest control point
	// Control points:
	//   - portal vertices inside frustum (max 6)
	//   - intersection points of portal edges with frustum planes (max 4 * 6 = 24)
	//   - intersection points of frustum edges with portal (max 12)
	// total max 6 + 24 + 12 = 42 points

	D3DXVECTOR3 ControlPoints[42];
	int nControlPoints = 0;

	// add to control points portal vertices inside frustum
	for (int iVertex = 0; iVertex < 4; iVertex++)
	{
		
	}

	D3DXMatrixInverse

	// calculate portal plane
	D3DXPLANE PortalPlane;
	D3DXPlaneFromPoints(PortalPlane, &Vertices[0], &Vertices[3], &Vertices[1]);
		// orientation doesn't matter!

	//   1) if portal completely on the positive side of portal, clip to portal vertex which has
	//      smalles positive distance from the plane
	//   2)
*/
}



void WePortal::Serialize(WeFileStream &FS)
{
	WeActor::Serialize(FS);

	FS.WriteInt(PortalVer);
	FS.WriteInt(Zone1ID);
	FS.WriteInt(Zone2ID);
	FS.WriteString(Zone1Name);
	FS.WriteString(Zone2Name);
	FS.WriteBool(TwoWayPortal);
	FS.WriteFloat(SizeX);
	FS.WriteFloat(SizeY);

}




void WePortal::Deserialize(WeFileStream &FS)
{
	WeActor::Deserialize(FS);

	int v;
	v = FS.ReadInt();
	if (v == 1)
	{
		Zone1ID = FS.ReadInt();
		Zone2ID = FS.ReadInt();
		Zone1Name = FS.ReadString();
		Zone2Name = FS.ReadString();
		TwoWayPortal = FS.ReadBool();
		SizeX = FS.ReadFloat();
		SizeY = FS.ReadFloat();

	} // end of version 1
}








void WePortal::Tick()
{
	if (InEditor)
	{
		Scale.x = SizeX;
		Scale.y = SizeY;
		Scale.z = 1.0f;

		WeActor::Tick();
		if (PrevScale != Scale)
		{
			// reinitialize object
			SAFE_DELETE(PhysicHandle);
			WeVector v;
			v.x = SizeX / 2.0f;
			v.y = SizeY / 2.0f;
			v.z = 1.0f;
			PhysicHandle = WePhysics::Instance()->CreateBox(this, Location, v, 10, 10, false, 0, false);		
			PhysicHandle->SetKinematic(true);
			Model->ComputeBoundingBox(this);
		}
		PrevScale = Scale;

		// Reset rotation 
		//D3DXMatrixIdentity(&TM);			
	}

	float sx, sy;
	sx = SizeX / 2.0f;
	sy = SizeY / 2.0f;

	WeVector x,y,z;
	GetLocalAxis(x,y,z);
	//D3DXVec3Normalize(&x, &x);
	//D3DXVec3Normalize(&y, &y);

	Vertices[0] = Location - x*sx - y*sy;
	Vertices[1] = Location - x*sx + y*sy;
	Vertices[2] = Location + x*sx + y*sy;
	Vertices[3] = Location + x*sx - y*sy;
}



