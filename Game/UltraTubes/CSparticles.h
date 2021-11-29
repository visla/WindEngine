/*
	CSparticles.h
*/


#ifndef _SPARTICLES
#define _SPARTICLES


#include <WeActor.h>
#include <WeVector.h>
#include <WeWorld.h>



class CSparticles: public WeActor
{
	struct CSparticle {
		bool			Visible;
		float			LifeTime;
		float			Speed;
		WeVector		SpawnDir;
		WeVector		SizeFrom;
		WeVector		SizeTo;
		WeMatrix		PrevTM;
		WeMatrix		TM;
	};

	struct CSparticleGroup {
		bool			Visible;

		WeActor			*Owner;
		WeVector		SpawnPos;
		WeVector		RelSpawnPos;
		WeVector		SpawnDir;

		int				Count;
		CSparticle		*Info;

		float			LifeTime;
		float			BornTime;
	};

protected:
	WeStaticModel	*SparticleModel;
	String			BaseConfigName;

	WeWorld			*OwnerWorld;
	float			LastSpawnTime;

	float			EmitRate;
	int				EmitCount, EmitCountDelta;
	float			ALifeTime, LifeTimeDelta;
	float			Speed, SpeedDelta;
	WeVector		SizeFrom, SizeFromDelta;
	WeVector		SizeTo, SizeToDelta;
	
	float			FollowFactor;
	WeVector		Gravity;
	float			Spread;

	int				MaxGroupCount;
	int				MaxGroups;
	CSparticleGroup	*Groups;

	bool			InitGroup(int iGroup, WeActor *aOwner, WeVector &aSpawnPos, WeVector &aSpawnDir);

public:
	CSparticles(WeWorld *aOwnerWorld, StringParam SparticleModelName, StringParam aBaseConfigName, int aMaxGroups, int aMaxGroupCount);
	// aOwnerWorld = world in which these sparticles will be shown
	// SparticleModelName = model which is used
	// aBaseConfigName = ini-file and section in which attributes for this Sparticle system are defined
	// aMaxGroups = max number of sparticle groups of this type
	// aMaxGroupCount = max number of sparticles in each group
	virtual ~CSparticles();

	void			Tick();
	virtual bool	IntersectFrustum(WeFrustum *frustum);
	virtual void	OnRender(WeCamera *Cam);
	virtual void	destroy();

	void			Init();
	void			Emit(WeActor *aOwner, WeVector &aSpawnPos, WeVector &aSpawnDir);
};


#endif
