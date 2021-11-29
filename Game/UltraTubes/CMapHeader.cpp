/*
    CMapHeader.cpp

	Autor: nebojsa Vislavski
	Date: 20.3.2006.
	Version: 1.0


*/

#include <CMapHeader.h>
#include <CProfile.h>
#include <WeLoger.h>
#include <WeRenderDevice.h>




CMapHeader::CMapHeader()
{
	Version = 8;
	RaceTime = 120.0f;
	AmbientColor = WeColor(0.05f, 0.05f, 0.05f, 1.0f);
	SkyColor = WeColor(0.05f, 0.05f, 0.05f, 1.0f);
	GroundColor = WeColor(0.05f, 0.05f, 0.05f, 1.0f);

	FogEnabled = false;
	FogColor = WeColor(1,1,1,1);
	FogStart = 0;
	FogEnd = 0;

	FHDRI[0] = 0.0f;
	FHDRI[1] = 1.0f;
	FHDRI[2] = 2.0f;

	ClippingDistance = 1000.0f;
	MusicFile = "";
	StatMusic = "";
	// Just for temp
	//LoadingImage = "textures.temp.01";
	//StoryText = "Long time ago in a galaxy far far away\n\
	//			Lived some good shit";
	//MapDescription = "this is some good description\n\
	//				 for our new game";
	//MapDescriptionImage = "textures.temp.01";
	// ----------------------------
}

void CMapHeader::Serialize(WeFileStream &FS)
{
	FS.WriteInt(Version);
	FS.WriteString(MapDescription);
	FS.WriteString(MapDescriptionImage);
	FS.WriteString(LoadingImage);
	FS.WriteFloat(RaceTime);
	FS.WriteFloat(BestRaceTime);
	FS.WriteString(BestRacePlayer);
	FS.WriteColor(AmbientColor);

	FS.WriteBool(FogEnabled);
	FS.WriteFloat(FogStart);
	FS.WriteFloat(FogEnd);
	FS.WriteColor(FogColor);
	FS.WriteColor(SkyColor);
	FS.WriteColor(GroundColor);


	//FS.WriteInt(0);
	FS.WriteInt(StartLocations.GetItemsCount());
	for(StartLocations.First(); !StartLocations.End(); StartLocations.Next())
	{
		//MessageBox(0, "Za header", 0, 0);
		StartLocations.GetItem()->Serialize(FS);
	}

	FS.WriteFloat(ClippingDistance);
	FS.WriteString(MusicFile);
	FS.WriteString(StatMusic);

	for (int i = 0; i < 3; ++i)
		FS.WriteFloat(FHDRI[i]);
}


void CMapHeader::Deserialize(WeFileStream &FS)
{
	int V = 0;
	V = FS.ReadInt();
	if (V == 1)
	{
		MapDescription = FS.ReadString();
		MapDescriptionImage = FS.ReadString();
		LoadingImage = FS.ReadString();
		BestRaceTime = FS.ReadFloat();
		BestRacePlayer = FS.ReadString();

		int NumSL = FS.ReadInt();
		CStartLocation *SL;
		StartLocations.Clear();
		for(int i=0; i<NumSL; i++)
		{
			SL = new CStartLocation;
			SL->Deserialize(FS);
			StartLocations.PushBack(SL);
		}
	} // end of version 1


	if (V == 2)
	{
		MapDescription = FS.ReadString();
		MapDescriptionImage = FS.ReadString();
		LoadingImage = FS.ReadString();
		RaceTime = FS.ReadFloat();
		BestRaceTime = FS.ReadFloat();
		BestRacePlayer = FS.ReadString();

		int NumSL = FS.ReadInt();
		CStartLocation *SL;
		StartLocations.Clear();
		for(int i=0; i<NumSL; i++)
		{
			SL = new CStartLocation;
			SL->Deserialize(FS);
			StartLocations.PushBack(SL);
		}
	} // End of version 2
	
	if (V == 3)
	{
		MapDescription = FS.ReadString();
		MapDescriptionImage = FS.ReadString();
		LoadingImage = FS.ReadString();
		RaceTime = FS.ReadFloat();
		BestRaceTime = FS.ReadFloat();
		BestRacePlayer = FS.ReadString();
		FS.ReadColor(AmbientColor);
		// set ambient color
		//WeRenderDevice::Instance()->SetAmbientColor(AmbientColor);

		int NumSL = FS.ReadInt();
		CStartLocation *SL;
		StartLocations.Clear();
		for(int i=0; i<NumSL; i++)
		{
			SL = new CStartLocation;
			SL->Deserialize(FS);
			StartLocations.PushBack(SL);
		}
	} // End of version 3

	if (V == 4)
	{
		MapDescription = FS.ReadString();
		MapDescriptionImage = FS.ReadString();
		LoadingImage = FS.ReadString();
		RaceTime = FS.ReadFloat();
		BestRaceTime = FS.ReadFloat();
		BestRacePlayer = FS.ReadString();
		FS.ReadColor(AmbientColor);

		FogEnabled = FS.ReadBool();
		FogStart = FS.ReadFloat();
		FogEnd = FS.ReadFloat();
		FS.ReadColor(FogColor);

		// set ambient color
		//WeRenderDevice::Instance()->SetAmbientColor(AmbientColor);
		//WeRenderDevice::Instance()->SetFog(FogEnabled, FogColor, 0);
		//WeRenderDevice::Instance()->SetFogStart(FogStart);
		//WeRenderDevice::Instance()->SetFogEnd(FogEnd);

		int NumSL = FS.ReadInt();
		CStartLocation *SL;
		StartLocations.Clear();
		for(int i=0; i<NumSL; i++)
		{
			SL = new CStartLocation;
			SL->Deserialize(FS);
			StartLocations.PushBack(SL);
		}
	} // End of version 4

	if (V == 5)
	{
		MapDescription = FS.ReadString();
		MapDescriptionImage = FS.ReadString();
		LoadingImage = FS.ReadString();
		RaceTime = FS.ReadFloat();
		BestRaceTime = FS.ReadFloat();
		BestRacePlayer = FS.ReadString();
		FS.ReadColor(AmbientColor);

		FogEnabled = FS.ReadBool();
		FogStart = FS.ReadFloat();
		FogEnd = FS.ReadFloat();
		FS.ReadColor(FogColor);
		FS.ReadColor(SkyColor);
		FS.ReadColor(GroundColor);

		// set ambient color
		//WeRenderDevice::Instance()->SetAmbientColor(AmbientColor);
		//WeRenderDevice::Instance()->SetSkyColor(SkyColor);
		//WeRenderDevice::Instance()->SetGroundColor(GroundColor);
		//WeRenderDevice::Instance()->SetFog(FogEnabled, FogColor, 0);
		//WeRenderDevice::Instance()->SetFogStart(FogStart);
		//WeRenderDevice::Instance()->SetFogEnd(FogEnd);

		int NumSL = FS.ReadInt();
		CStartLocation *SL;
		StartLocations.Clear();
		for(int i=0; i<NumSL; i++)
		{
			SL = new CStartLocation;
			SL->Deserialize(FS);
			StartLocations.PushBack(SL);
		}
	} // End of version 5

	if (V == 6)
	{
		MapDescription = FS.ReadString();
		MapDescriptionImage = FS.ReadString();
		LoadingImage = FS.ReadString();
		RaceTime = FS.ReadFloat();
		BestRaceTime = FS.ReadFloat();
		BestRacePlayer = FS.ReadString();
		FS.ReadColor(AmbientColor);

		FogEnabled = FS.ReadBool();
		FogStart = FS.ReadFloat();
		FogEnd = FS.ReadFloat();
		FS.ReadColor(FogColor);
		FS.ReadColor(SkyColor);
		FS.ReadColor(GroundColor);

		// set ambient color
		//WeRenderDevice::Instance()->SetAmbientColor(AmbientColor);
		//WeRenderDevice::Instance()->SetSkyColor(SkyColor);
		//WeRenderDevice::Instance()->SetGroundColor(GroundColor);
		//WeRenderDevice::Instance()->SetFog(FogEnabled, FogColor, 0);
		//WeRenderDevice::Instance()->SetFogStart(FogStart);
		//WeRenderDevice::Instance()->SetFogEnd(FogEnd);

		int NumSL = FS.ReadInt();
		CStartLocation *SL;
		StartLocations.Clear();
		for(int i=0; i<NumSL; i++)
		{
			SL = new CStartLocation;
			SL->Deserialize(FS);
			StartLocations.PushBack(SL);
		}

		ClippingDistance = FS.ReadFloat();
		MusicFile = FS.ReadString();
	} // End of version 6

	if (V == 7)
	{
		MapDescription = FS.ReadString();
		MapDescriptionImage = FS.ReadString();
		LoadingImage = FS.ReadString();
		RaceTime = FS.ReadFloat();
		BestRaceTime = FS.ReadFloat();
		BestRacePlayer = FS.ReadString();
		FS.ReadColor(AmbientColor);

		FogEnabled = FS.ReadBool();
		FogStart = FS.ReadFloat();
		FogEnd = FS.ReadFloat();
		FS.ReadColor(FogColor);
		FS.ReadColor(SkyColor);
		FS.ReadColor(GroundColor);

		// set ambient color
		//WeRenderDevice::Instance()->SetAmbientColor(AmbientColor);
		//WeRenderDevice::Instance()->SetSkyColor(SkyColor);
		//WeRenderDevice::Instance()->SetGroundColor(GroundColor);
		//WeRenderDevice::Instance()->SetFog(FogEnabled, FogColor, 0);
		//WeRenderDevice::Instance()->SetFogStart(FogStart);
		//WeRenderDevice::Instance()->SetFogEnd(FogEnd);

		int NumSL = FS.ReadInt();
		CStartLocation *SL;
		StartLocations.Clear();
		for(int i=0; i<NumSL; i++)
		{
			SL = new CStartLocation;
			SL->Deserialize(FS);
			StartLocations.PushBack(SL);
		}

		ClippingDistance = FS.ReadFloat();
		MusicFile = FS.ReadString();

		for (int i = 0; i < 3; ++i)
			FHDRI[i] = FS.ReadFloat();
	} // End of version 7

	if (V == 8)
	{
		MapDescription = FS.ReadString();
		MapDescriptionImage = FS.ReadString();
		LoadingImage = FS.ReadString();
		RaceTime = FS.ReadFloat();
		BestRaceTime = FS.ReadFloat();
		BestRacePlayer = FS.ReadString();
		FS.ReadColor(AmbientColor);

		FogEnabled = FS.ReadBool();
		FogStart = FS.ReadFloat();
		FogEnd = FS.ReadFloat();
		FS.ReadColor(FogColor);
		FS.ReadColor(SkyColor);
		FS.ReadColor(GroundColor);

		// set ambient color
		//WeRenderDevice::Instance()->SetAmbientColor(AmbientColor);
		//WeRenderDevice::Instance()->SetSkyColor(SkyColor);
		//WeRenderDevice::Instance()->SetGroundColor(GroundColor);
		//WeRenderDevice::Instance()->SetFog(FogEnabled, FogColor, 0);
		//WeRenderDevice::Instance()->SetFogStart(FogStart);
		//WeRenderDevice::Instance()->SetFogEnd(FogEnd);

		int NumSL = FS.ReadInt();
		CStartLocation *SL;
		StartLocations.Clear();
		for(int i=0; i<NumSL; i++)
		{
			SL = new CStartLocation;
			SL->Deserialize(FS);
			StartLocations.PushBack(SL);
		}

		ClippingDistance = FS.ReadFloat();
		MusicFile = FS.ReadString();
		StatMusic = FS.ReadString();

		for (int i = 0; i < 3; ++i)
			FHDRI[i] = FS.ReadFloat();
	} // End of version 8
}



void CMapHeader::SetupRenderDevice()
{
	WeRenderDevice::Instance()->SetAmbientColor(AmbientColor);
	WeRenderDevice::Instance()->SetSkyColor(SkyColor);
	WeRenderDevice::Instance()->SetGroundColor(GroundColor);
	WeRenderDevice::Instance()->SetFog(FogEnabled, FogColor, 0);
	WeRenderDevice::Instance()->SetFogStart(FogStart);
	WeRenderDevice::Instance()->SetFogEnd(FogEnd);
}
