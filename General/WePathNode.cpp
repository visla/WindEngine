/*
      File: WePathNode.cpp
	  Author: Vislavski Nebojsa
	  Date: 22.02.2005.
	  Version: 1.0
	  Platform: Linux/Windows
*/



#include <WePathNode.h>


int WePathNode::LastSequenceNumber = 0;


void WePathNode::ResetSequenceNumbers()
{
	LastSequenceNumber = 0;
}


int WePathNode::NewSequenceNumber()
{
	LastSequenceNumber = LastSequenceNumber + 10;
	return LastSequenceNumber;
}


WePathNode::WePathNode()
{
	Type = WETYPE_PATHNODE;	
	ModelName = "MapEditor.Geometry.PathNode";
	PrevPathNode = this;
	NextPathNode = this;
	Version = 10;
	SequenceNumber = 0;
	SortedIndex = -1;
	for (int i = 0; i < 4; ++i)
		HitDistances[i] = -1.0f;
	Radius = 0.0f;
	FHDRI = 0.0f;
	HealingRange = 0.0f;
	HealingSpeed = 0.0f;
	HeadLight = HEADLINE_NOTHING;
	NodeType = NODETYPE_NORMAL;
	LeftNum = 0;
	RightNum = 0;
	FollowTime = 0.0f;
	IsActive = false;
	NextNode = NULL;
	PrevNode = NULL;
	NextAltNode = NULL;
	PrevAltNode = NULL;
	PathIndex = 0.0f;
	AltScale = 1.0f;
	Refraction = false;
	TubeID = 0;
	RefDir.x = 0.0f;
	RefDir.y = 0.0f;
	RefDir.z = 1.0f;
	Shaking = false;
}

void WePathNode::Tick()
{
	//if (InEditor) WeActor::Tick();
	WeActor::Tick();
}


void WePathNode::Clone(WeActor *Destination, bool DeepCopy)
{
	WeActor::Clone(Destination);

	WePathNode *Dest = (WePathNode *)Destination;
	Dest->SequenceNumber = NewSequenceNumber();
	Dest->HeadLight = HeadLight;
	Dest->HealingRange = HealingRange;
	Dest->HealingSpeed = HealingSpeed;
	Dest->NodeType = NodeType;
	Dest->LeftNum = LeftNum;
	Dest->RightNum = RightNum;
	Dest->FollowTime = FollowTime;
	Dest->Shaking = Shaking;
}


void WePathNode::Serialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	WeActor::Serialize(FS);

	FS.WriteInt(Version);
	FS.WriteInt(SequenceNumber);
	for (int i = 0; i < 4; ++i)
		FS.WriteFloat(HitDistances[i]);
	FS.WriteFloat(Radius);
	FS.WriteFloat(FHDRI);
	FS.WriteFloat(HealingRange);
	FS.WriteFloat(HealingSpeed);
	FS.WriteInt(HeadLight);
	FS.WriteInt(NodeType);
	FS.WriteInt(LeftNum);
	FS.WriteInt(RightNum);
	FS.WriteFloat(FollowTime);
	FS.WriteBool(Refraction);
	FS.WriteBool(Shaking);
}


void WePathNode::Deserialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	WeActor::Deserialize(FS);

	int ver;

	if (FS.Eof())
		ver = 0;
	else
		ver = FS.ReadInt();

	if (ver <= 0) // we hit next actor or end of file
	{
		if (ver < 0)
			FS.Seek(-4, SEEK_CUR);
		SequenceNumber = NewSequenceNumber();
	}

	if (ver == 1)
	{
		SequenceNumber = FS.ReadInt();
		if (SequenceNumber > LastSequenceNumber)
			LastSequenceNumber = SequenceNumber;
	} // end of version 1

	else if (ver == 2)
	{
		SequenceNumber = FS.ReadInt();
		if (SequenceNumber > LastSequenceNumber)
			LastSequenceNumber = SequenceNumber;

		for (int i = 0; i < 4; ++i)
			HitDistances[i] = FS.ReadFloat();
	} // end of version 2*/

	else if (ver == 3)
	{
		SequenceNumber = FS.ReadInt();
		if (SequenceNumber > LastSequenceNumber)
			LastSequenceNumber = SequenceNumber;

		for (int i = 0; i < 4; ++i)
			HitDistances[i] = FS.ReadFloat();

		FHDRI = FS.ReadFloat();
	} // end of version 3

	else if (ver == 4)
	{
		SequenceNumber = FS.ReadInt();
		if (SequenceNumber > LastSequenceNumber)
			LastSequenceNumber = SequenceNumber;

		for (int i = 0; i < 4; ++i)
			HitDistances[i] = FS.ReadFloat();

		FHDRI = FS.ReadFloat();
		HealingRange = FS.ReadFloat();
		HealingSpeed = FS.ReadFloat();
	} // end of version 4

	else if (ver == 5)
	{
		SequenceNumber = FS.ReadInt();
		if (SequenceNumber > LastSequenceNumber)
			LastSequenceNumber = SequenceNumber;

		for (int i = 0; i < 4; ++i)
			HitDistances[i] = FS.ReadFloat();

		FHDRI = FS.ReadFloat();
		HealingRange = FS.ReadFloat();
		HealingSpeed = FS.ReadFloat();
		HeadLight = FS.ReadInt();
	} // end of version 5

	else if (ver == 6)
	{
		SequenceNumber = FS.ReadInt();
		if (SequenceNumber > LastSequenceNumber)
			LastSequenceNumber = SequenceNumber;

		for (int i = 0; i < 4; ++i)
			HitDistances[i] = FS.ReadFloat();

		FHDRI = FS.ReadFloat();
		HealingRange = FS.ReadFloat();
		HealingSpeed = FS.ReadFloat();
		HeadLight = FS.ReadInt();
		NodeType = FS.ReadInt();
	} // end of version 6

	else if (ver == 7)
	{
		SequenceNumber = FS.ReadInt();
		if (SequenceNumber > LastSequenceNumber)
			LastSequenceNumber = SequenceNumber;

		for (int i = 0; i < 4; ++i)
			HitDistances[i] = FS.ReadFloat();

		FHDRI = FS.ReadFloat();
		HealingRange = FS.ReadFloat();
		HealingSpeed = FS.ReadFloat();
		HeadLight = FS.ReadInt();
		NodeType = FS.ReadInt();
		LeftNum = FS.ReadInt();
		RightNum = FS.ReadInt();
		FollowTime = FS.ReadFloat();
	} // end of version 7

	else if (ver == 8)
	{
		SequenceNumber = FS.ReadInt();
		if (SequenceNumber > LastSequenceNumber)
			LastSequenceNumber = SequenceNumber;

		for (int i = 0; i < 4; ++i)
			HitDistances[i] = FS.ReadFloat();

		FHDRI = FS.ReadFloat();
		HealingRange = FS.ReadFloat();
		HealingSpeed = FS.ReadFloat();
		HeadLight = FS.ReadInt();
		NodeType = FS.ReadInt();
		LeftNum = FS.ReadInt();
		RightNum = FS.ReadInt();
		FollowTime = FS.ReadFloat();
		Refraction = FS.ReadBool();
	} // end of version 8

	else if (ver == 9)
	{
		SequenceNumber = FS.ReadInt();
		if (SequenceNumber > LastSequenceNumber)
			LastSequenceNumber = SequenceNumber;

		for (int i = 0; i < 4; ++i)
			HitDistances[i] = FS.ReadFloat();
		Radius = FS.ReadFloat();

		FHDRI = FS.ReadFloat();
		HealingRange = FS.ReadFloat();
		HealingSpeed = FS.ReadFloat();
		HeadLight = FS.ReadInt();
		NodeType = FS.ReadInt();
		LeftNum = FS.ReadInt();
		RightNum = FS.ReadInt();
		FollowTime = FS.ReadFloat();
		Refraction = FS.ReadBool();
	} // end of version 9

	else if (ver == 10)
	{
		SequenceNumber = FS.ReadInt();
		if (SequenceNumber > LastSequenceNumber)
			LastSequenceNumber = SequenceNumber;

		for (int i = 0; i < 4; ++i)
			HitDistances[i] = FS.ReadFloat();
		Radius = FS.ReadFloat();

		FHDRI = FS.ReadFloat();
		HealingRange = FS.ReadFloat();
		HealingSpeed = FS.ReadFloat();
		HeadLight = FS.ReadInt();
		NodeType = FS.ReadInt();
		LeftNum = FS.ReadInt();
		RightNum = FS.ReadInt();
		FollowTime = FS.ReadFloat();
		Refraction = FS.ReadBool();
		Shaking = FS.ReadBool();
	} // end of version 10
}


void WePathNode::Initialize()
{
	if (InEditor)
	{		
		PhysicModel.Type = PHYSICMODEL_STATIC_SHAPE;
		WeActor::Initialize();
	}
	else
	{
		//WeActor::Initialize();		
		//PhysicHandle->SetCollision(false);
		Renderable = false;
	}
}


String WePathNode::GetInfo()
{
	char tmp[30];
	sprintf_s(tmp, "#%d [%.3f]", SequenceNumber, FHDRI);
	return (String)tmp;
}
