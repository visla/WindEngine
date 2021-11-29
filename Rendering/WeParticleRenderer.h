/*
   WeParticleRenderer.h

   Autor: Nebojsa Vislavski
   Date: 9.11.2005.
   Version: 1.0

   This is particle renderer. Used to render particle systems
  
*/


#ifndef _WEPARTICLERENDERER_H
#define _WEPARTICLERENDERER_H


#include <d3dx9.h>
#include <d3d9.h>
#include <WeBaseRenderer.h>
#include <WindEngine.h>
#include <SmartList.h>



class WeWorld;
class WeParticleSystem;
class WeParticleVertex;
class WeShader;

const int ParticleBucket = 500;  // render in buckets of 500 items

//using namespace std;





class WINDENGINE_API WeParticleRenderer: public WeBaseRenderer
{
private:
	static WeParticleRenderer *_instance;
protected:

	class ShaderBatch
	{
	public:
		~ShaderBatch();
		WeShader *Shader;
		CSmartList <WeParticleSystem *> ParticleSystems;
	};

	CSmartList <WeParticleRenderer::ShaderBatch *> ShaderBatches;


	LPDIRECT3DVERTEXBUFFER9			ParticlesBuffer; 
	LPDIRECT3DINDEXBUFFER9          ParticlesIndex;			
	LPDIRECT3DVERTEXDECLARATION9	ParticleDeclaration;
	int								ParticleBufferOffset;
	WeShader						*CurrentShader;
	bool							bInitialized;


	void	PreRender();
	void	PostRender();
	void	Render(WeParticleSystem *ps);


public:
	static WeParticleRenderer *Instance();
	WeParticleRenderer();
	virtual ~WeParticleRenderer() { ShutDown(); }


	virtual	void	AddParticleSystem(WeParticleSystem *particlesys);


	virtual bool	Initialize();
	virtual void	ShutDown();

    void			ClearAll();
	virtual void	Render(WeWorld *world, WeParticleSystem *ps);   // renders particle systems

	void			OnLostDevice();
	void			OnResetDevice();

};


#endif