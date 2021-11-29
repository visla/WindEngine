/*
    WeLightMapper.cpp

	Autor: Nebojsa Vislavski
	Date: 1.9.2006.

*/


#include <WeLightMapper.h>
#include <WeStaticModel.h>
#include <weMesh.h>



WeLightMapper *WeLightMapper::_instance = NULL;

WeLightMapper *WeLightMapper::Instance()
{
	if (_instance) return _instance;
	_instance = new WeLightMapper;
	return _instance;
}


WeLightMapper::WeLightMapper()
{
	GroupAngle = cosf(D3DX_PI / 6.0f);

}

WeLightMapper::~WeLightMapper()
{

}



void WeLightMapper::MakeLightMap(WeActor *Actor, WeWorld *World)
{
	if (!Actor) return;
	if (!Actor->Model) return;
	if (Actor->Model->IsModel) return; // we don`t generate light maps for animated models
	if (!Actor->Model->Mesh) return;

	if (Actor->Model->Mesh->GetUseNormalMap())
	{
		MakeUVNormalMapped(Actor->Model->Mesh);
	} else
	{
		MakeUV(Actor->Model->Mesh);
	}

	BakeLights(Actor, World);
}

void WeLightMapper::MakeUVNormalMapped(WeMesh *Mesh)
{	
	int NumVertices;	
	NumVertices = Mesh->GetNumVertices();

	WeNormalMappedMeshVertex *Vertices, *orig;
	DWORD *Faces, *tmpFaces;

	Vertices = (WeNormalMappedMeshVertex*)malloc(sizeof(WeNormalMappedMeshVertex)*NumVertices);
	Faces = (DWORD *)malloc(sizeof(DWORD)*Mesh->GetNumFaces()*3);

	Mesh->LockVertexBuffer((void **)&orig, D3DLOCK_READONLY);
	memcpy(Vertices, orig, sizeof(WeNormalMappedMeshVertex)*NumVertices);
	Mesh->UnlockVertexBuffer();

	Mesh->LockIndexBuffer((void **)&tmpFaces, D3DLOCK_READONLY);
	memcpy(Faces, tmpFaces, sizeof(DWORD)*Mesh->GetNumFaces()*3);
	Mesh->UnlockIndexBuffer();
	
	Mesh->LockVertexBuffer((void **)&orig, D3DLOCK_DISCARD);
	for (int i = 0; i < NumVertices; ++i)
	{
		orig[i] = Vertices[i];
		orig[i] = Vertices[i];
	}
	Mesh->UnlockVertexBuffer();

	// faces and verices are in array so we need to make lu, lv
	


	
	free(Vertices);
	free(Faces);
}


int WeLightMapper::DeterminePlane(SFace *Face, int index)
{
	WeVector fin, a, b;
	a = NMVertices[Face->V[1]].Position - NMVertices[Face->V[0]].Position;
	b = NMVertices[Face->V[2]].Position - NMVertices[Face->V[0]].Position;

	D3DXVec3Cross(&fin, &b, &a);	
	D3DXVec3Normalize(&FacesNormals[index], &fin);

	if (abs(fin.z) >= abs(fin.y) && abs(fin.z) >= abs(fin.x)) return 0;
	if (abs(fin.x) >= abs(fin.y) && abs(fin.x) >= abs(fin.z)) return 1;
	if (abs(fin.y) >= abs(fin.x) && abs(fin.y) >= abs(fin.z)) return 2;	
	return -1;
}


void WeLightMapper::ResetElementParams()
{
	Padding = 3.0f; // in pixels
	LightMapFactor = 1.0f / LIGHTMAP_SIZE;
	//SizeFactor = 1.0f;
	LastWidth = 0;
	LastHeight = 0;
	LastX = 0;
	LastY = 0;
	MaxWidth = 0;
	MaxHeight = 0;
	Root.Clear();
}

void WeLightMapper::SPartition::Clear()
{
	if (Child[0]) Child[0]->Clear();
	if (Child[1]) Child[1]->Clear();
	SAFE_DELETE(Child[0]);
	SAFE_DELETE(Child[1]);

	Used = false;
}

WeLightMapper::SPartition* WeLightMapper::SPartition::GetNode(float WWidth, float HHeight, float Padding)
{
	SPartition *retNode;
	if (Child[0] || Child[1])
	{
		if (Child[0])
		{
			retNode = Child[0]->GetNode(WWidth, HHeight, Padding);
			if (retNode) return retNode;
		}

		if (Child[1]) return Child[1]->GetNode(WWidth, HHeight, Padding);
	} else
	{
		if (Used) return NULL;

		// if rect fits 
		if (WWidth <= Width && HHeight <= Height)
		{
			// fits perfectly
			if ((Width == WWidth) && (Height == HHeight))
			{
				Used = true;
				return this;
			}

			// split
			// else split node
			float dw, dh;
			dw = (Width) - WWidth;
			dh = (Height) - HHeight;
			Child[0] = new SPartition;
			Child[1] = new SPartition;

			if (dw > dh)
			{
				Child[0]->X = X;
				Child[0]->Y = Y;
				Child[0]->Width = WWidth;
				Child[0]->Height = Height;

				Child[1]->X = X + WWidth + 1;
				Child[1]->Y = Y;
				Child[1]->Width = Width - WWidth - 1;
				Child[1]->Height = Height;

			} else
			{
				Child[0]->X = X;
				Child[0]->Y = Y;
				Child[0]->Width = Width;
				Child[0]->Height = HHeight;

				Child[1]->X = X;
				Child[1]->Y = Y + HHeight + 1;
				Child[1]->Width = Width;
				Child[1]->Height = Height - HHeight - 1;
			}

			return Child[0]->GetNode(WWidth, HHeight, Padding);			
		} else 
		{
			return NULL;		
		}
	}
	return NULL;
}

bool WeLightMapper::GetNewElement(float Width, float Height, float &OffsetX, float &OffsetY)
{
	SPartition *finPart;

	Root.X = Padding;
	Root.Y = Padding;
	Root.Width = LIGHTMAP_SIZE;	
	Root.Height = LIGHTMAP_SIZE;
	finPart = Root.GetNode(Width, Height, Padding);	
	if (!finPart)
	{
		return false;
	}	
	OffsetX = finPart->X;
	OffsetY = finPart->Y;

	return true;
}

void WeLightMapper::MakeUV(WeMesh *Mesh)
{


	


/*
	int NumVertices;
	int NumFaces;

	NumVertices = Mesh->GetNumVertices();
	NumFaces = Mesh->GetNumFaces();
	
	float a,b,c,fin;
	a = (Mesh->BoundingBox.Max.x - Mesh->BoundingBox.Min.x) / 2.0f;
	b = (Mesh->BoundingBox.Max.y - Mesh->BoundingBox.Min.y) / 2.0f;
	c = (Mesh->BoundingBox.Max.z - Mesh->BoundingBox.Min.z) / 2.0f;

	//if (a >= b && a >= c) SizeFactor = 1.0f / (a * 0.06f);
	//if (b >= a && b >= c) SizeFactor = 1.0f / (b * 0.06f);
	//if (c >= a && c >= b) SizeFactor = 1.0f / (c * 0.06f);
	SizeFactor = 1.0f;


	WeNormalMappedMeshVertex *Vertices;
	
	NMVertices = (WeNormalMappedMeshVertex*)malloc(sizeof(WeNormalMappedMeshVertex)*NumVertices);
	Mesh->LockVertexBuffer((void **)&Vertices, D3DLOCK_READONLY);
	memcpy(NMVertices, Vertices, sizeof(WeNormalMappedMeshVertex)*NumVertices);
	Mesh->UnlockVertexBuffer();

	Mesh->LockIndexBuffer((void **)&Faces, D3DLOCK_READONLY);		


	Planes = new DWORD[NumFaces];
	FacesNormals = new WeVector[NumFaces];

	
	// Modify normal and make uv`s
	int plane;
	for (int i = 0; i < NumFaces; ++i)
	{
		plane = DeterminePlane(&Faces[i], i);
		switch (plane)
		{
		case -1: 
			MessageBox(0, "ERROR PLANE", 0, 0);
			break;
		case 0: 
			Planes[i] = 0;
			NMVertices[Faces[i].V[0]].lu = NMVertices[Faces[i].V[0]].Position.x;
			NMVertices[Faces[i].V[0]].lv = NMVertices[Faces[i].V[0]].Position.y;
			NMVertices[Faces[i].V[1]].lu = NMVertices[Faces[i].V[1]].Position.x;
			NMVertices[Faces[i].V[1]].lv = NMVertices[Faces[i].V[1]].Position.y;
			NMVertices[Faces[i].V[2]].lu = NMVertices[Faces[i].V[2]].Position.x;
			NMVertices[Faces[i].V[2]].lv = NMVertices[Faces[i].V[2]].Position.y;
			break;
		case 1: 
			Planes[i] = 1;
			NMVertices[Faces[i].V[0]].lu = NMVertices[Faces[i].V[0]].Position.y;
			NMVertices[Faces[i].V[0]].lv = NMVertices[Faces[i].V[0]].Position.z;
			NMVertices[Faces[i].V[1]].lu = NMVertices[Faces[i].V[1]].Position.y;
			NMVertices[Faces[i].V[1]].lv = NMVertices[Faces[i].V[1]].Position.z;
			NMVertices[Faces[i].V[2]].lu = NMVertices[Faces[i].V[2]].Position.y;
			NMVertices[Faces[i].V[2]].lv = NMVertices[Faces[i].V[2]].Position.z;
			break;
		case 2: 
			Planes[i] = 2;
			NMVertices[Faces[i].V[0]].lu = NMVertices[Faces[i].V[0]].Position.z;
			NMVertices[Faces[i].V[0]].lv = NMVertices[Faces[i].V[0]].Position.x;
			NMVertices[Faces[i].V[1]].lu = NMVertices[Faces[i].V[1]].Position.z;
			NMVertices[Faces[i].V[1]].lv = NMVertices[Faces[i].V[1]].Position.x;
			NMVertices[Faces[i].V[2]].lu = NMVertices[Faces[i].V[2]].Position.z;
			NMVertices[Faces[i].V[2]].lv = NMVertices[Faces[i].V[2]].Position.x;
			break;
		}	
	}		


	// group planes
	int MarkedCount = 1;
	int Similar = 0;

	Marked = new DWORD[NumFaces];
	ZeroMemory(Marked, sizeof(DWORD)*NumFaces);

	for (int CurFace = 0; CurFace < NumFaces; ++CurFace)
	{
		if (Marked[CurFace] == 0)		
		{
			// if two planes are similar and can go into one group
			for (int Face = 0; Face < CurFace; ++Face)
			{				
				if (Planes[Face] == Planes[CurFace])
				{
					if (D3DXVec3Dot(&FacesNormals[CurFace], &FacesNormals[Face]) > GroupAngle)
					{
						Similar = 0;
						for (int j = 0; j < 3; ++j)
						{
							// if they share at least one vertex they are similar
							if ((Vertices[Faces[CurFace].V[j]].Position == Vertices[Faces[Face].V[0]].Position) ||
								(Vertices[Faces[CurFace].V[j]].Position == Vertices[Faces[Face].V[1]].Position) ||
								(Vertices[Faces[CurFace].V[j]].Position == Vertices[Faces[Face].V[2]].Position))
							{
								Similar++;
							}
						}	

						if (Similar > 0)
						{
							Marked[CurFace] = Marked[Face];
							break;
						}
					}
				}
			}

			if (Marked[CurFace] == 0)
			{
				Marked[CurFace] = MarkedCount;
				MarkedCount++;
			}

		}
	}



	// make final uv`s on texture
	float MinX, MinY, MaxX, MaxY;
	float OffX, OffY;

	ResetElementParams();



	for (int i = 1; i < MarkedCount; ++i)	
	{
		MinX = 100000;
		MinY = 100000;
		MaxX = -100000;
		MaxY = -100000;

		// find min, max for this group of triangles
		for (int Face = 0; Face < NumFaces; ++Face)
		{
			if (Marked[Face] == i)
			{
				// find min max for face or faces group
				//FILE *f; 
				//f = fopen("nesto.txt", "at+");
				//fprintf(f, "FACE %d, MaxFaces: %d Name: %s\n", i, NumFaces, Mesh->Name.GetString());
				for (int k = 0; k < 3; k++)
				{
					if (NMVertices[Faces[Face].V[k]].lu < MinX) MinX = NMVertices[Faces[Face].V[k]].lu;
					if (NMVertices[Faces[Face].V[k]].lu > MaxX) MaxX = NMVertices[Faces[Face].V[k]].lu;

					if (NMVertices[Faces[Face].V[k]].lv < MinY) MinY = NMVertices[Faces[Face].V[k]].lv;
					if (NMVertices[Faces[Face].V[k]].lv > MaxY) MaxY = NMVertices[Faces[Face].V[k]].lv;
					//fprintf(f, "     V[%d]: %3.3f, %3.3f\n", j, NMVertices[Faces[i].V[j]].lu, NMVertices[Faces[i].V[j]].lv);
				}
				//fflush(f);
				//fclose(f);
			}
		}

		MaxX = (MaxX - MinX)*SizeFactor;
		MaxY = (MaxY - MinY)*SizeFactor;

		// make new element
		if (!GetNewElement(MaxX+Padding, MaxY+Padding, OffX, OffY))
		{
			// TODO: Just temp message box. put some clever idea here sometime...
			//char tmp[100];
			//sprintf(tmp, "Nema mesta %s, %d: MaxX %3.3f, MaxY: %3.3f", Mesh->Name.GetString(), i ,MaxX, MaxY);
			//MessageBox(0, tmp, 0, 0);			
		} 

		for (int Face = 0; Face < NumFaces; ++Face)
		{
			if (Marked[Face] == i)
			{
				for (int j = 0; j < 3; j++)
				{
					NMVertices[Faces[Face].V[j]].lu = ((NMVertices[Faces[Face].V[j]].lu - MinX)*SizeFactor + OffX)*LightMapFactor;
					NMVertices[Faces[Face].V[j]].lv = ((NMVertices[Faces[Face].V[j]].lv - MinY)*SizeFactor + OffY)*LightMapFactor;		
				}								
			}
		}
	}


	
	Mesh->UnlockIndexBuffer();
	
	Mesh->LockVertexBuffer((void **)&Vertices, D3DLOCK_DISCARD);
	memcpy(Vertices, NMVertices, sizeof(WeNormalMappedMeshVertex)*NumVertices);
	Mesh->UnlockVertexBuffer();

	free(NMVertices);
	delete[] Planes;
	delete[] Marked;
	delete[] FacesNormals;*/
}

void WeLightMapper::BakeLights(WeActor *Actor, WeWorld *World)
{

}

float WeLightMapper::CalcMeshArea(int NumFaces, int NumVertices)
{
	float a,b,c,s ;
	float P;
	WeVector V;
	P = 0.0f;
	for (int i = 0; i < NumFaces; i++)	
	{
		V = NMVertices[Faces[i].V[0]].Position - NMVertices[Faces[i].V[1]].Position;
		a = D3DXVec3Length(&V);
		V = NMVertices[Faces[i].V[0]].Position - NMVertices[Faces[i].V[2]].Position;
		b = D3DXVec3Length(&V);
		V = NMVertices[Faces[i].V[1]].Position - NMVertices[Faces[i].V[2]].Position;
		c = D3DXVec3Length(&V);

		s = (a+b+c) / 2.0f;
		P += sqrtf(s*(s-a)*(s-b)*(s-c));
	}
	return P;
}