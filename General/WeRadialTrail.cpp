/*
     WeRadialTrail.cpp

	 Autor: Nebojsa Vislavski
	 Date: 28.4.2006.

*/


#include <WeRadialTrail.h>
#include <WeTimer.h>
#include <WeBatchItem.h>
#include <WeCamera.h>
#include <WeWorld.h>
#include <WeBatchRenderer.h>

WeRadialTrail::WeRadialTrail()
{
	Type = WETYPE_TRAIL;
	MaxSegments = 10;
	SegmentsCount = 0;
	SegmentWidth = 1.0f;
	StartFadeFactor = 0;
	SegmentCreationTime = .3f;
	VertexData = NULL;
	IndexData = NULL;
	SegmentLocation = NULL;
	VertexDataCount = 0;
	SegmentColor = WeColor(1,1,1,1);
	FromAlpha = 1.0f;
	ToAlpha = 0.0f;
	FromRadius = 1.0f;
	ToRadius = 0.1f;	
	RingSegmentCount = 8;
	Counter = 0;
	RadialTrailVer = 1;
}


WeRadialTrail::~WeRadialTrail()
{
	SAFE_DELETE(VertexData);
	SAFE_DELETE(IndexData);
	SAFE_DELETE(SegmentLocation);
}


void WeRadialTrail::Reset()
{
	VertexDataCount = 0;
}

void WeRadialTrail::Tick()
{

	PrevTM = TM;
	ObjectAge += WeTimer::Instance()->GetDeltaTimeF();

	// suppose this actor is occluded. If it is not BatchRenderer will reset this to 0
	OccludedTime += WeTimer::Instance()->GetDeltaTimeF(); 
	if (LifeTime != 0.0f)
	{
		LifeTime -= WeTimer::Instance()->GetDeltaTimeF();
		if (LifeTime <= 0.0f)
		{
			delete this;
			return;
		}
	}



	if (MaxSegments <= 0) return;


	Counter += WeTimer::Instance()->GetDeltaTimeF();

	WeVector XA, YA, ZA;
	GetLocalAxis(XA, YA, ZA);
	D3DXVec3Normalize(&XA, &XA);

    
	if (Counter > SegmentCreationTime)
	{
		// we should create one more segment
		if (SegmentsCount == 0)
		{
			// need to init first ring count *2 vertices
			WeColor tmpCol;
			float ad, alfa = 0;
			ad = 2.0f*D3DX_PI / RingSegmentCount;
			for (int i = 0; i < RingVerticesCount; ++i)
			{
				tmpCol = SegmentColor;
				tmpCol.a = FromAlpha;

				VertexData[i].Position = Location + (XA*cosf(alfa) + YA*sinf(alfa))*FromRadius;
				VertexData[i].Color = tmpCol;
				VertexData[i+RingVerticesCount].Position = Location + (XA*cosf(alfa) + YA*sinf(alfa))*FromRadius;
				VertexData[i+RingVerticesCount].Color = tmpCol;
				alfa += ad;
			}

			VertexDataCount = RingVerticesCount*2;
			SegmentsCount++;
		} 
		   else
		{

			// move everything one step forward except zero elements so we could insert new vertices
			for (int i = VertexDataCount-RingVerticesCount; i >= 0; i-=RingVerticesCount)
			{	
				// dont move last element if we go beyond boundaries of array
				if (i+RingVerticesCount < (MaxSegments+1)*RingVerticesCount)
				{
					for (int j = 0; j < RingVerticesCount; ++j)
					{
						VertexData[i+RingVerticesCount+j] = VertexData[i+j];						
					}
				}
			}

			SegmentsCount++;
			if (SegmentsCount > MaxSegments) 
				SegmentsCount = MaxSegments;
			else
				VertexDataCount += RingVerticesCount;		
		}
		Counter = 0;
	}

	// make u,v values
	float u, v;
	int SegmentNum;

	//float v;
	for (int i = 0; i < VertexDataCount; i+=RingVerticesCount)
	{
		SegmentNum = i / RingVerticesCount;
		u = (float)SegmentNum;

		// make new alpha value, and new radius
		float k;
		float alpha;
		k = (float)(SegmentNum) / (float)MaxSegments;       

		alpha = (ToAlpha - FromAlpha)*k + FromAlpha;

		WeColor c;

		for (int j = 0; j < RingVerticesCount; ++j)
		{			
			v = (float)j / (float)RingSegmentCount;
			VertexData[i+j].u = u;
			VertexData[i+j].v = v;
			c = VertexData[i+j].Color;
			c.a = alpha;
			VertexData[i+j].Color = c;
		}
	}

	// first two vertex are always moving with us
	WeColor tmpCol;
	float ad, alfa = 0;
	ad = 2.0f*D3DX_PI / RingSegmentCount;
	for (int i = 0; i < RingVerticesCount; ++i)
	{
		tmpCol = SegmentColor;
		tmpCol.a = FromAlpha;

		VertexData[i].Position = Location + (XA*cosf(alfa) + YA*sinf(alfa))*FromRadius;
		VertexData[i].Color = tmpCol;
		SegmentLocation[0] = Location;
		alfa += ad;
	}
}


void WeRadialTrail::Serialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	WeActor::Serialize(FS);

	FS.WriteInt(RadialTrailVer);
	FS.WriteInt(MaxSegments);
	FS.WriteFloat(SegmentWidth);
	FS.WriteFloat(StartFadeFactor);
	FS.WriteColor(SegmentColor);
	FS.WriteFloat(SegmentCreationTime);
	FS.WriteString(MaterialName);
	FS.WriteFloat(FromAlpha);
	FS.WriteFloat(ToAlpha);
	FS.WriteFloat(FromRadius);
	FS.WriteFloat(ToRadius);
	FS.WriteInt(RingVerticesCount);
}


void WeRadialTrail::Deserialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	WeActor::Deserialize(FS);

	int Ver;
	Ver = FS.ReadInt();
	if (Ver == 1)
	{
		MaxSegments = FS.ReadInt();
		SegmentWidth = FS.ReadFloat();
		StartFadeFactor = FS.ReadFloat();
		FS.ReadColor(SegmentColor);
		SegmentCreationTime = FS.ReadFloat();
		MaterialName = FS.ReadString();
		FromAlpha = FS.ReadFloat();
		ToAlpha = FS.ReadFloat();
		FromRadius = FS.ReadFloat();
		ToRadius = FS.ReadFloat();
		RingVerticesCount = FS.ReadInt();
	}
}


void WeRadialTrail::Clone(WeActor *Destination, bool DeepCopy)
{

	WeActor::Clone(Destination, DeepCopy);

	WeRadialTrail *tmpT;
	tmpT = (WeRadialTrail *)Destination;
	tmpT->MaxSegments = MaxSegments;
	tmpT->SegmentWidth = SegmentWidth;
	tmpT->StartFadeFactor = StartFadeFactor;
	tmpT->SegmentColor = SegmentColor;
	tmpT->SegmentCreationTime = SegmentCreationTime;
	tmpT->FromAlpha = FromAlpha;
	tmpT->ToAlpha = ToAlpha;
	tmpT->FromRadius = FromRadius;
	tmpT->ToRadius = ToRadius;
	tmpT->RingVerticesCount = RingVerticesCount;
}


void WeRadialTrail::Initialize()
{
	//WeActor::Initialize();

	RingVerticesCount = RingSegmentCount+1;
	SAFE_DELETE(VertexData);
	SAFE_DELETE(IndexData);


	VertexData = new WeRadialTrail::STrailVertex[MaxSegments*RingVerticesCount+RingVerticesCount];
	IndexData = new WORD[MaxSegments*RingVerticesCount*6];
	SegmentLocation = new WeVector[MaxSegments+1];

	// fill index buffer cause it will not change.
	int j = 0;
	for (int i = 0; i < MaxSegments*RingVerticesCount*6; i+=6)
	{
		IndexData[i] = j;
		IndexData[i+1] = j+1;
		IndexData[i+2] = j+RingVerticesCount+1;
		
		IndexData[i+3] = j;
		IndexData[i+4] = j+RingVerticesCount+1;
		IndexData[i+5] = j+RingVerticesCount;
		j++;
	}

	SegmentsCount = 0;
	VertexDataCount = 0;
	Counter = 0;
}


bool WeRadialTrail::IntersectFrustum(WeFrustum *frustum)
{
	// culling not supported yet
	return true;
}

void WeRadialTrail::OnRender(WeCamera *Cam)
{
	if (!Renderable || !Editor_Renderable) return;
	if (!VertexData) return;
	if (World) 
		if (LastRenderedFrame >= World->GetWorldFrame()) return;		


	WeBatchItem *BatchItem = new WeBatchItem;

	BatchItem->Material = Material;
	BatchItem->Actor = this;
	BatchItem->Mesh = NULL;
	BatchItem->Alpha = true;
	BatchItem->WorldMatrix = TM;
	BatchItem->PrevWorldMatrix = PrevTM;

	WeVector tmp;
	D3DXVec3TransformCoord(&tmp, &Location, &Cam->View);
	BatchItem->ZDist = tmp.z;	
	if (BatchItem->Material->Type == WEMATERIALTYPE_REFRACTIVE)
		WeBatchRenderer::Instance()->QueueRefractiveBatch(BatchItem);
	else
		WeBatchRenderer::Instance()->QueueAlphaBatch(BatchItem);	
	LastRenderedFrame++;
}


int WeRadialTrail::GetIndexSize()
{
	return SegmentsCount*RingSegmentCount*6*sizeof(WORD);
}

int WeRadialTrail::GetFacesCount()
{
	return SegmentsCount*2*RingSegmentCount;
}