/*
   WeEngineDesc.h

   Autor: nebojsa Vislavski
   Date: 27.12.2005.
   Version: 1.0


*/

#ifndef _WEENGINEDESC_H
#define _WEENGINEDESC_H

class WeEngineDesc
{
public:
	WeEngineDesc(): FullScreen(false), ResWidth(1024), ResHeight(768), Dedicated(false) 
	{
		DefaultShaderName = "Effects.Default.Standard";
		DefaultTextureName = "Textures.Default.Texture";
		DistortionMaterialName = "Materials.Default.Default";
		DefaultMaterialName = "Materials.Default.Default";
		DistortionShaderName = "Effects.Default.Standard";
		ItemQuering = false;
		DynamicLights = true;
		RenderParticles = true;
		AntialiasFactor = 0;
		ImmediatePresent = true;
		AutomaticRendering = true;
		MaxDynamicLights = 2;
	}

	// rendering
	bool	FullScreen;
	int		ResWidth;
	int		ResHeight;
	String  DefaultShaderName;
	String	DefaultTextureName;
	String	DefaultMaterialName;	
	String	DistortionMaterialName; // this texture will be used as perturbation map for heat simulation
	String  DistortionShaderName;
	bool	ItemQuering; // maybe will increase performance maybe not. Leave it as default true
	bool	DynamicLights; // should we use dynamic lights	
	bool	RenderParticles;
	int		AntialiasFactor; // od 1..16
	bool	ImmediatePresent;
	bool	AutomaticRendering; // default true. If this is set then you dont need to call RenderDevice->DoRender manualy
	int		MaxDynamicLights; // defualt is 2. How many lights will be processed by each object

	// network
	bool	Dedicated;

};

#endif