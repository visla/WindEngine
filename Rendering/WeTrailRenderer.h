/*
   WeTrailRenderer.h

   Autor: Nebojsa Vislavski
   Date: 9.11.2005.
   Version: 1.0

   This is particle renderer. Used to render particle systems
  
*/


#ifndef _WeTrailRenderer_H
#define _WeTrailRenderer_H


#include <d3dx9.h>
#include <d3d9.h>
#include <WeBaseRenderer.h>
#include <WindEngine.h>
#include <SmartList.h>
#include <WeTrail.h>


class WeWorld;
class WeParticleSystem;
class WeParticleVertex;
class WeBatchItem;



#define MAX_TRAIL_BUFFER_SIZE 6000




class WINDENGINE_API WeTrailRenderer: public WeBaseRenderer
{
private:
	static WeTrailRenderer *_instance;
protected:

	class ShaderBatch
	{
	public:
		~ShaderBatch();
		WeShader *Shader;
		CSmartList <WeTrail *> Trail;
	};

	CSmartList <WeTrailRenderer::ShaderBatch *> ShaderBatches;



	LPDIRECT3DVERTEXBUFFER9			TrailBuffer; 
	LPDIRECT3DINDEXBUFFER9          TrailIndex;			
	LPDIRECT3DVERTEXDECLARATION9	TrailDeclaration;
	int								TrailBufferOffset;
	bool							bInitialized;


	void	Render(WeTrail *trail);

public:
	static WeTrailRenderer *Instance();
	WeTrailRenderer();
	virtual ~WeTrailRenderer() { ShutDown(); }

	virtual bool	Initialize();
	virtual void	ShutDown();

	void			Render(WeWorld *world, WeTrail *trail);   // renders particle systems

	void			OnLostDevice();
	void			OnResetDevice();

};


#endif