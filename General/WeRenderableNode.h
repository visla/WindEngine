/*
    File: WeRenderableNode.h
	Author: Tomislav Kukic
	Date: 13.09.2005.
	Version: 1.0
*/




#ifndef _WERENDERABLENODE
#define _WERENDERABLENODE


/*
#ifndef DEBUG

#ifdef _HAS_ITERATOR_DEBUGGING
#undef _HAS_ITERATOR_DEBUGGING
#endif // _HAS_ITERATOR_DEBUGGING

#define _HAS_ITERATOR_DEBUGGING 0


#ifdef _DEBUG
#undef _DEBUG
#endif


#endif // DEBUG
*/


#include <WeActor.h>
#include <WindEngine.h>
#include <SmartList.h>
#include <WeBox.h>
#include <WeSphere.h>
//#include <vector>

//using namespace std;

class WINDENGINE_API WeRenderableNode
{
friend class WeRenderTree;

protected:
	WeRenderableNode(void);
	WeRenderableNode(WeBox &Box);
	virtual ~WeRenderableNode(void);

public:

	CSmartList<WeActor*>	Prefabs;
	WeRenderableNode		*Children[8];	
	WeBox					BBox;
	WeSphere				BSphere; // bounding sphere used for fast culling calculation
	bool					Used;
};



#endif