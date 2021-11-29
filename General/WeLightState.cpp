/*
 
     WeLightState.cpp

	 Autor: Nebojsa Vislavski
	 Date: 15.1.2006.
	 Version: 1.0


	 This is light state for one light. 

*/

#include <WeLightState.h>

WeLightState::WeLightState()
{
	Diffuse = WeColor(0,0,0,0);
	Intensity = 1.0;
	SpecularIntensity = 1;
	Specular = WeColor(1,1,1,1); 
	Duration = 1;
	for (int i = 0; i < LIGHT_FLARE_TEXTURES; ++i)
		FlareAlpha[i] = 1.0f;

	CoronaAlpha = 1.0f;
	Version = 11;
}


void WeLightState::Serialize(WeFileStream &FS)
{
	FS.WriteInt(Version);
	FS.WriteColor(Diffuse);
	FS.WriteColor(Specular);
	FS.WriteFloat(Intensity);
	FS.WriteFloat(SpecularIntensity);
	FS.WriteFloat(Duration);
	FS.WriteFloat(CoronaAlpha);
	for (int i = 0; i < LIGHT_FLARE_TEXTURES; ++i)
		FS.WriteFloat(FlareAlpha[i]);
}

void WeLightState::Deserialize(WeFileStream &FS)
{
	int ver;
	ver = FS.ReadInt();
	
	if (ver == 10)
	{
		FS.ReadColor(Diffuse);
		FS.ReadColor(Specular);
		Intensity = FS.ReadFloat();
		SpecularIntensity = FS.ReadFloat();
		Duration = FS.ReadFloat();

	} else if (ver == 11)
	{
		FS.ReadColor(Diffuse);
		FS.ReadColor(Specular);
		Intensity = FS.ReadFloat();
		SpecularIntensity = FS.ReadFloat();
		Duration = FS.ReadFloat();
		CoronaAlpha = FS.ReadFloat();
		for (int i = 0; i < LIGHT_FLARE_TEXTURES; ++i)
			FlareAlpha[i] = FS.ReadFloat();
	} else
	{
		FS.Seek(-4, SEEK_CUR);
		FS.ReadColor(Diffuse);
		FS.ReadColor(Specular);
		Duration = FS.ReadFloat();
		Intensity = FS.ReadFloat();		
	}

}