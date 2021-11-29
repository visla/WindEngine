/*

       File: WeRenderTree.h
	   Author: Tomislav Kukic
	   Date: 13.09.2005.
	   Version: 1.0
	   Platform: Linux/Win32

	   Serbian: 
	   Ovo je klasa koja sluzi da organizuje prefab-e u octree.
	   Stvari kao sto su prefabi cine staticku scenu i ne mogu se brisati tek tako iz
	   scene. Kada se pozove BuildRenderTree onda ovaj prolazi kroz sve Actor-e world-a i sredjuje 
	   ih u octree.
*/




#ifndef _WERENDERTREE
#define _WERENDERTREE



#include <Smartlist.h>
#include <WeRenderableNode.h>
#include <WeRenderTree.h>
#include <WindEngine.h>
#include <WeBox.h>
#include <WeSphere.h>
#include <CString.h>
//#include <vector>

//using namespace std;


class WeSphere;
class WeRenderableNode;
class WeCamera;
class WeFrustum;

class WINDENGINE_API WeRenderTree
{
friend class WeWorld;
friend class WeZone;


protected:

	int ProcessedNodes; // this counts how many nodes has been processed
	int NodesCount;   // this counts how many nodes this tree has
	int ProcessedDepth; // what is depth of tree



	int MaxDepth;     // max depth of octree that will go in
	WeRenderableNode *ParentNode; // parent node in octree
	WeBox SpaceBox; // space box. What is the space of interest
	WeWorld *World; // for what world is this rendertree
	bool	bInitialized;

	WeRenderTree(void);
	virtual ~WeRenderTree(void);

	void	CreateChildren(WeRenderableNode *ParentNode);

	bool	PopPrefab(WeRenderableNode *ParentNode, WeActor *prefab); 
	// this pushes item down the tree
	void	PushPrefab(WeRenderableNode *ParentNode, WeActor *prefab, int Depth = 0); 
	// recursive methods
	//void	NodesInView(WeRenderableNode *ParentNode, WeFrustum* Frustum, vector <WeRenderableNode*> &nodes);
	void	NodesInView(WeRenderableNode *ParentNode, WeFrustum* Frustum, CSmartList<WeRenderableNode*> &Nodes);
	void	NodesInSphere(WeRenderableNode *ParentNode, WeSphere &Sphere, CSmartList<WeRenderableNode*> &Nodes);

	void	SaveToXMLFile(FILE *f, WeRenderableNode *ParentNode, int depth);


public:
   
	// Initialize Octree with this params. MaxDepth shouldnt be greater than 15
	virtual void	Initialize(WeWorld *world, int maxDepth, WeBox &SpaceBox);
	virtual void	ShutDown();

	void			SaveToXMLFile(String FileName);

	// this will pop out item frrom tree view
	void			PopPrefab(WeActor *prefab);
	// this will drop prefab to the octree. It is automaticly called when adding prefabs.
	virtual void	PushPrefab(WeActor *prefab); 

	// BuildRenderTree - this is very slow operation and call this very rarely when 
	// you really need to rebuild your octree.
	// Building of octree is performed automaticly when world detects prefabs	
	virtual void	BuildRenderTree();
	// this will take mesh from prefab, lock vb and transform every vertex to 
	// world and find min and max vectors
			void	MakeBoundingBox(WeActor *prefab);
	//virtual void	GetNodesInView(WeCamera* Cam, vector <WeRenderableNode*> &nodes);
	virtual void	GetNodesInView(WeCamera* Cam, CSmartList<WeRenderableNode*> &nodes);
	//virtual void	GetNodesInView(WeFrustum* Frustum, vector <WeRenderableNode*> &nodes);
	virtual void	GetNodesInView(WeFrustum* Frustum, CSmartList<WeRenderableNode*> &nodes);
	virtual void	GetNodesInSphere(WeSphere &Sphere, CSmartList<WeRenderableNode*> &nodes);


	int				GetNodesCount() { return NodesCount; }
	int				GetProcessedDepth() { return ProcessedDepth; }
	int				GetProcessedNodes() { return ProcessedNodes; }
};



#endif

