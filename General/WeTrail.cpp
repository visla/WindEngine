/*
     WeTrail.cpp

	 Autor: Nebojsa Vislavski
	 Date: 28.4.2006.

*/


#include <WeTrail.h>
#include <WeTimer.h>
#include <WeBatchItem.h>
#include <WeCamera.h>
#include <WeWorld.h>
#include <WeBatchRenderer.h>

WeTrail::WeTrail()
{
	Type = WETYPE_TRAIL;
	MaxSegments = 10;
	SegmentsCount = 0;
	SegmentWidth = 1.0f;
	StartFadeFactor = 0;
	SegmentCreationTime = .3f;
	VertexData = NULL;
	IndexData = NULL;
	VertexDataCount = 0;
	SegmentColor.r = 1.0f; 
	SegmentColor.g = 0.0f; 
	SegmentColor.b = 0.0f; 
	SegmentColor.a = 1.0f; 
	Counter = 0;

}


WeTrail::~WeTrail()
{
	SAFE_DELETE(VertexData);
	SAFE_DELETE(IndexData);
}


void WeTrail::Reset()
{
	VertexDataCount = 0;
}

void WeTrail::Tick()
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
			// need to init first four vertex
			VertexData[0].Position = Location + XA*SegmentWidth;
			VertexData[0].Color = SegmentColor;
			VertexData[1].Position = Location - XA*SegmentWidth;
			VertexData[1].Color = SegmentColor;
			VertexData[2].Position = Location + XA*SegmentWidth;
			VertexData[2].Color = SegmentColor;
			VertexData[3].Position = Location - XA*SegmentWidth;
			VertexData[3].Color = SegmentColor;	

			VertexDataCount = 4;
			SegmentsCount++;
		} 
		   else
		{

			// move everything one step forward except zero elements so we could insert new vertices
			for (int i = VertexDataCount-2; i >= 0; i-=2)
			{			

				// dont move last element if we go beyond boundaries of array
				if (i+2 < (MaxSegments+1)*2)
				{
					VertexData[i+2] = VertexData[i];
					VertexData[i+3] = VertexData[i+1];
				}
			}

			SegmentsCount++;
			if (SegmentsCount > MaxSegments) 
				SegmentsCount = MaxSegments;
			else
				VertexDataCount += 2;
			

			//VertexData[2].Position = Location + XA*SegmentWidth;
			//VertexData[2].Color = SegmentColor;
			//VertexData[3].Position = Location - XA*SegmentWidth;
			//VertexData[3].Color = SegmentColor;	
		}
		Counter = 0;
	}

	// make u,v values
	float u;
	//float v;
	for (int i = 0; i < VertexDataCount; i+=2)
	{
		u = (float)i / 2.0f;

		VertexData[i].u = u;
		VertexData[i].v = 1.0f;

		VertexData[i+1].u = u;
		VertexData[i+1].v = 0.0f;

		// make new alpha value, and u, v values		
		float k;
		k = (float)(i*2) / (float)MaxSegments;       
		k -= StartFadeFactor;
		k = 1.0f-k;
		if (k < 0.0f) k = 0.0f;
		if (k > 1.0f) k = 1.0f;

		WeColor c;
		c = VertexData[i].Color;
		c.a = k;
		VertexData[i].Color = c;

		c = VertexData[i+1].Color;
		c.a = k;
		VertexData[i+1].Color = c;		
	}

	// first two vertex are always moving with us
	VertexData[0].Position = Location + XA*SegmentWidth;
	VertexData[0].Color = SegmentColor;

	VertexData[1].Position = Location - XA*SegmentWidth;
	VertexData[1].Color = SegmentColor;
}


int WeTrail::GetIndexSize()
{
  return SegmentsCount*6*sizeof(WORD);
}

int WeTrail::GetFacesCount()
{
	return SegmentsCount*2;
}

void WeTrail::Serialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	WeActor::Serialize(FS);

	FS.WriteInt(MaxSegments);
	FS.WriteFloat(SegmentWidth);
	FS.WriteFloat(StartFadeFactor);
	FS.WriteColor(SegmentColor);
	FS.WriteFloat(SegmentCreationTime);
	FS.WriteString(MaterialName);
}


void WeTrail::Deserialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	WeActor::Deserialize(FS);

	MaxSegments = FS.ReadInt();
	SegmentWidth = FS.ReadFloat();
	StartFadeFactor = FS.ReadFloat();
	FS.ReadColor(SegmentColor);
	SegmentCreationTime = FS.ReadFloat();
	MaterialName = FS.ReadString();
}


void WeTrail::Clone(WeActor *Destination, bool DeepCopy)
{

	WeActor::Clone(Destination, DeepCopy);

	WeTrail *tmpT;
	tmpT = (WeTrail *)Destination;
	tmpT->MaxSegments = MaxSegments;
	tmpT->SegmentWidth = SegmentWidth;
	tmpT->StartFadeFactor = StartFadeFactor;
	tmpT->SegmentColor = SegmentColor;
	tmpT->SegmentCreationTime = SegmentCreationTime;
}


void WeTrail::Initialize()
{
	//WeActor::Initialize();

	SAFE_DELETE(VertexData);
	SAFE_DELETE(IndexData);


	VertexData = new WeTrail::STrailVertex[MaxSegments*4];
	IndexData = new WORD[MaxSegments*6];

	// fill index buffer cause it will not change.
	int j = 0;
	for (int i = 0; i < MaxSegments*6; i+=6)
	{
		IndexData[i] = j;
		IndexData[i+1] = j+1;
		IndexData[i+2] = j+2;

		IndexData[i+3] = j+1;
		IndexData[i+4] = j+3;
		IndexData[i+5] = j+2;
		j += 2;
	}

	SegmentsCount = 0;
	VertexDataCount = 0;
	Counter = 0;
}


bool WeTrail::IntersectFrustum(WeFrustum *frustum)
{
	// culling not supported yet
	return true;
}

void WeTrail::OnRender(WeCamera *Cam)
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
	D3DXVec3TransformNormal(&tmp, &Location, &Cam->View);
	BatchItem->ZDist = tmp.z;	
	if (BatchItem->Material->Type == WEMATERIALTYPE_REFRACTIVE)
		WeBatchRenderer::Instance()->QueueRefractiveBatch(BatchItem);
	else
		WeBatchRenderer::Instance()->QueueAlphaBatch(BatchItem);	

	LastRenderedFrame++;
}
