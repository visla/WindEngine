/*
      File: WePathNode.h
	  Author: Vislavski Nebojsa
	  Date: 22.02.2005.
	  Version: 1.0
	  Platform: Linux/Windows
*/



#ifndef _WEPATHNODE_
#define _WEPATHNODE_

#include <WindEngine.h>
#include <WeActor.h>


// HeadLight
#define HEADLINE_NOTHING	0
#define HEADLINE_OFF		1
#define HEADLINE_ON			2


// NodeType
#define NODETYPE_NORMAL			0
#define NODETYPE_SPLIT			1
#define NODETYPE_CONNECT		2
#define NODETYPE_TELEPORTING	3
#define NODETYPE_TELEPORTED		4


class WINDENGINE_API WePathNode: public WeActor
{
friend class WeWorld;

protected:
	static int					LastSequenceNumber;

	int							Version;

public:
	WePathNode *PrevPathNode;
	WePathNode *NextPathNode;
								WePathNode();
	void						Tick();

	virtual void				Clone(WeActor *Destination, bool DeepCopy = false); 
	void						Initialize();

	void						Serialize(WeFileStream &FS);
	void						Deserialize(WeFileStream &FS);

	virtual String				GetInfo();

	int							SequenceNumber;
	int							SortedIndex;
	float						PathIndex;

	float						HitDistances[4];
	// 0 = up, 1 = down, 2 = left, 3 = right
	float						Radius;

	float						FHDRI;
	float						HealingRange;
	float						HealingSpeed; // HP/s
	int							HeadLight;

	int							NodeType;
	int							LeftNum;
	int							RightNum;

	float						FollowTime;

	bool						Shaking;

	bool						IsActive;
	WePathNode					*NextNode;
	WePathNode					*PrevNode;
	WePathNode					*NextAltNode;
	WePathNode					*PrevAltNode;
	float						AltScale;

	bool						Refraction;
	int							TubeID;

	WeVector					RefDir;

	static void					ResetSequenceNumbers();
	static int					NewSequenceNumber();
};

#endif