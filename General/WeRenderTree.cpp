/*

       File: WeRenderTree.cpp
	   Author: Tomislav Kukic
	   Date: 13.09.2005.
	   Version: 1.0
*/




#include <WeRenderTree.h>
#include <WeWorld.h>
#include <WeCamera.h>
#include <WeStaticModel.h>
#include <WeMesh.h>
#include <WeProfiler.h>
#include <WeFrustum.h>

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeRenderTree::WeRenderTree(void)
{
	ParentNode = NULL;	
	World = NULL;
	bInitialized = false;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
WeRenderTree::~WeRenderTree(void)
{	
	ShutDown();
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeRenderTree::PopPrefab(WeActor *prefab)
{
	if (!bInitialized) return;
	PopPrefab(ParentNode, prefab);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  returns true if need to delete node
//
//////////////////////////////////////////////////////////////////////////
bool WeRenderTree::PopPrefab(WeRenderableNode *ParentNode, WeActor *prefab)
{
	// if this prefab is in this node remove it	
	//vector <WePrefab *>::iterator I;
	//for (I = ParentNode->Prefabs.begin(); I != ParentNode->Prefabs.end(); I++)
	for (ParentNode->Prefabs.First(); !ParentNode->Prefabs.End(); ParentNode->Prefabs.Next())
	{
		//if ((*I) == prefab)
		if (ParentNode->Prefabs.GetItem() == prefab)
		{
			//ParentNode->Prefabs.erase(I);			
			ParentNode->Prefabs.Delete();
			//if (ParentNode->Prefabs.size() > 0) return false;
			if (ParentNode->Prefabs.GetItemsCount() > 0)
			{
				return false;
			}

			// this is now empty node, if it has no children then delete it
			for (int i = 0; i < 8; i++)
			{
				if (ParentNode->Children[i] != NULL) return false;
			}
			return true;			
		}
	}

	// check children
	for (int i = 0; i < 8; i++)
	{
		if (ParentNode->Children[i])
		{
			if (PopPrefab(ParentNode->Children[i], prefab))
			{
				SAFE_DELETE( ParentNode->Children[i] );
			}
		}
	}
	return false;

}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeRenderTree::PushPrefab(WeActor *prefab)
{
	if (!bInitialized) return;

	MakeBoundingBox(prefab);
	PushPrefab(ParentNode, prefab);
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeRenderTree::MakeBoundingBox(WeActor *prefab)
{
	if (!prefab) return;
	if (!prefab->Model) return;

	prefab->Model->ComputeBoundingBox(prefab);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeRenderTree::PushPrefab(WeRenderableNode *Node, WeActor *prefab, int Depth)
{
	WeBox TmpBox;
	TmpBox = prefab->BBox;
	
	int tmpi = -1;

	if (Depth > ProcessedDepth) ProcessedDepth = Depth;

	if (Node->BBox.Contains(&TmpBox))
	{
		// maybe its max depth so just add this object to node
		//char tmp[100];
		//sprintf(tmp, "Min: %3.3f, %3.3f, %3.3f, Max: %3.3f, %3.3f, %3.3f", TmpBox.Min.x, TmpBox.Min.y, TmpBox.Min.z, 
		//	TmpBox.Max.x, TmpBox.Max.y, TmpBox.Max.z);
		//MessageBox(0, tmp, 0, 0);
		if (Depth >= MaxDepth)
		{					
			//Node->Prefabs.push_back(prefab);
			Node->Prefabs.PushBack(prefab);
			Node->Used = true;
		} else
		{	
			// Create all or only missing children
			CreateChildren(Node);
			// are some of its children fully containing it
			tmpi = -1;

			for (int i = 0; i < 8; i++)
			{
				// if just one node contains it completely 
				if (Node->Children[i]->BBox.Contains(&TmpBox))
				{
                    tmpi = i;
				} else
				{
					// if this child doesnt contain object then erase it if its not used
					if (!Node->Children[i]->Used)
					{
						NodesCount--;
						SAFE_DELETE(Node->Children[i]);
					}					
				}
			}
			// some children completely contains it so mark used
			if (tmpi >= 0) 
			{
				Node->Used = true;
				// recurse down to this child
				PushPrefab(Node->Children[tmpi], prefab, Depth+1);
			} else
			{
				// no children will fully contain it so add it to this node				
				//Node->Prefabs.push_back(prefab);						
				Node->Prefabs.PushBack(prefab);
				Node->Used = true;				
			}
		}	
	}			
}





/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeRenderTree::SaveToXMLFile(String FileName)
{
   FILE *f;
   f = fopen(FileName, "wt+");
   if (!f) return;

   fprintf(f, "<ROOT>\n"); 
   SaveToXMLFile(f, ParentNode, 0);
   fprintf(f, "</ROOT>\n"); 
   
   fclose(f);
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeRenderTree::SaveToXMLFile(FILE *f, WeRenderableNode *ParentNode, int depth)
{
	if (!ParentNode) return;

	for (int i = 0; i < depth; i++)
		fprintf(f, "   ");

	fprintf(f, "<BBOX MIN=\"%3.3f, %3.3f, %3.3f\" MAX=\"%3.3f, %3.3f, %3.3f\" >\n", ParentNode->BBox.Min.x,ParentNode->BBox.Min.y, ParentNode->BBox.Min.z, 
					ParentNode->BBox.Max.x, ParentNode->BBox.Max.y, ParentNode->BBox.Max.z);

	// write objects in this node
	for (ParentNode->Prefabs.First(); !ParentNode->Prefabs.End(); ParentNode->Prefabs.Next())
	{
		fprintf(f, "<OBJECT MODEL=\"%s\" BBOX=\"%3.3f, %3.3f, %3.3f MAX=%3.3f, %3.3f, %3.3f\"  />\n", ParentNode->Prefabs.GetItem()->ModelName.GetString(),
			ParentNode->Prefabs.GetItem()->BBox.Min.x, ParentNode->Prefabs.GetItem()->BBox.Min.y, ParentNode->Prefabs.GetItem()->BBox.Min.z, 
			ParentNode->Prefabs.GetItem()->BBox.Max.x, ParentNode->Prefabs.GetItem()->BBox.Max.y, ParentNode->Prefabs.GetItem()->BBox.Max.z);
	}

	for (int i = 0; i < 8; i++)
	{
		SaveToXMLFile(f, ParentNode->Children[i], depth+1);
	}

	fprintf(f, "</BBOX>\n");
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeRenderTree::CreateChildren(WeRenderableNode *ParentNode)
{
	WeBox Box;
	float HalfSize = (ParentNode->BBox.Max.x - ParentNode->BBox.Min.x) / 2.0f;
	

	// Bottom boxes 
	Box.Min = ParentNode->BBox.Min;
	Box.Max = Box.Min + (WeVector(1,1,1)*HalfSize);
	if (!ParentNode->Children[0])
	{
		ParentNode->Children[0] = new WeRenderableNode(Box);
		NodesCount++;
	}

	Box.Min = ParentNode->BBox.Min + (WeVector(1,0,0)*HalfSize);
	Box.Max = Box.Min + (WeVector(1,1,1)*HalfSize);
	if (!ParentNode->Children[1])
	{
		ParentNode->Children[1] = new WeRenderableNode(Box);
		NodesCount++;
	}

	Box.Min = ParentNode->BBox.Min + (WeVector(0,0,1)*HalfSize);
	Box.Max = Box.Min + (WeVector(1,1,1)*HalfSize);
	if (!ParentNode->Children[2])
	{
		ParentNode->Children[2] = new WeRenderableNode(Box);
		NodesCount++;
	}

	Box.Min = ParentNode->BBox.Min + (WeVector(1,0,1)*HalfSize);
	Box.Max = Box.Min + (WeVector(1,1,1)*HalfSize);
	if (!ParentNode->Children[3])
	{
		ParentNode->Children[3] = new WeRenderableNode(Box);
		NodesCount++;
	}

	// Top boxes
	Box.Min = ParentNode->BBox.Min + (WeVector(0,1,0)*HalfSize);
	Box.Max = Box.Min + (WeVector(1,1,1)*HalfSize);
	if (!ParentNode->Children[4])
	{
		ParentNode->Children[4] = new WeRenderableNode(Box);
		NodesCount++;
	}

	Box.Min = ParentNode->BBox.Min + (WeVector(1,1,0)*HalfSize);
	Box.Max = Box.Min + (WeVector(1,1,1)*HalfSize);
	if (!ParentNode->Children[5])
	{
		ParentNode->Children[5] = new WeRenderableNode(Box);
		NodesCount++;
	}

	Box.Min = ParentNode->BBox.Min + (WeVector(0,1,1)*HalfSize);
	Box.Max = Box.Min + (WeVector(1,1,1)*HalfSize);
	if (!ParentNode->Children[6])
	{
		ParentNode->Children[6] = new WeRenderableNode(Box);
		NodesCount++;
	}

	Box.Min = ParentNode->BBox.Min + (WeVector(1,1,1)*HalfSize);
	Box.Max = Box.Min + (WeVector(1,1,1)*HalfSize);
	if (!ParentNode->Children[7])
	{
		ParentNode->Children[7] = new WeRenderableNode(Box);
		NodesCount++;
	}
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeRenderTree::BuildRenderTree()
{
	if (!bInitialized) return;

	// first delete octree we already maybe have.
	if (ParentNode)
	{
	  SAFE_DELETE(ParentNode);
	}

	
	NodesCount = 0;
	ProcessedDepth = 0;
	ParentNode = new WeRenderableNode(SpaceBox);

	// Mark all engine tags of prefabs to 0
	WeActor *Iterator;
	for (Iterator = World->Actors->NextActor; Iterator != World->Actors; Iterator = Iterator->NextActor)
	{
		if (Iterator->IsPrefab) PushPrefab(ParentNode, Iterator);
	}

	//printf("NodesCount:%d MaxDepth:%d", NodesCount, ProcessedDepth);?
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeRenderTree::Initialize(WeWorld *world, int maxDepth, WeBox &spaceBox)
{
	if (bInitialized) return;
	World = world;
	MaxDepth = maxDepth;
	SpaceBox = spaceBox;
	NodesCount = 0;
	ProcessedDepth = 0;

	// Create parent node with bounding box
	ParentNode = new WeRenderableNode(SpaceBox);
	bInitialized = true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
void WeRenderTree::ShutDown()
{
	if (!bInitialized) return;
	// Release hole octree
	if (ParentNode) delete ParentNode;
	ParentNode = NULL;
	World = NULL;
	bInitialized = false;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  OpenUDPSocket
//         Input Parametars :  SOCKET s, data, len, port
//         Output Parametars:  N/A
//         Comments         :  broadcasts data in LAN
//
//////////////////////////////////////////////////////////////////////////
//void WeRenderTree::GetNodesInView(WeCamera *Cam, vector <WeRenderableNode *> &Nodes)
void WeRenderTree::GetNodesInView(WeCamera *Cam, CSmartList<WeRenderableNode *> &Nodes)
{
	// empty list of nodes
	Nodes.Clear();

	ProcessedNodes = 0;

	if (!ParentNode) return;
	NodesInView(ParentNode, &Cam->Frustum, Nodes);
}


//void WeRenderTree::GetNodesInView(WeFrustum *Frustum, vector <WeRenderableNode *> &Nodes)
void WeRenderTree::GetNodesInView(WeFrustum *Frustum, CSmartList<WeRenderableNode *> &Nodes)
{
	// empty list of nodes
	Nodes.Clear();

	ProcessedNodes = 0;

	if (!ParentNode) return;
	NodesInView(ParentNode, Frustum, Nodes);
}





//void WeRenderTree::NodesInView(WeRenderableNode *ParentNode, WeFrustum* Frustum, vector <WeRenderableNode*> &nodes)
void WeRenderTree::NodesInView(WeRenderableNode *ParentNode, WeFrustum* Frustum, CSmartList<WeRenderableNode*> &Nodes)
{	
	ProcessedNodes++;

	if (Frustum->Collide(ParentNode->BSphere))
	{
		// if we have prefabs in this node than add this node to list for rendering
		//if (ParentNode->Prefabs.size() > 0) 
		if (ParentNode->Prefabs.GetItemsCount() > 0) 
		{
			WeProfiler::Instance()->RenderTreeNodesVisible++;
			//nodes.push_back(ParentNode);
			Nodes.PushBack(ParentNode);
		}

		for (int i = 0; i < 8; i++)
		{
			if (ParentNode->Children[i])
			{				
				NodesInView(ParentNode->Children[i], Frustum, Nodes);
			}
		}
	}
}





void WeRenderTree::GetNodesInSphere(WeSphere &Sphere, CSmartList <WeRenderableNode *> &Nodes)
{
	// empty list of nodes
	Nodes.Clear();
	NodesInSphere(ParentNode, Sphere, Nodes);
}





void WeRenderTree::NodesInSphere(WeRenderableNode *ParentNode, WeSphere &Sphere, CSmartList<WeRenderableNode*> &Nodes)
{
	if (Sphere.Intersect(&ParentNode->BBox))
	{
		//if (ParentNode->Prefabs.size() > 0) 
		if (ParentNode->Prefabs.GetItemsCount() > 0) 
		{
			WeProfiler::Instance()->RenderTreeNodesVisible++;
			Nodes.PushBack(ParentNode);
		}
		
		for (int i = 0; i < 8; i++)
		{
			if (ParentNode->Children[i])
			{
				NodesInSphere(ParentNode->Children[i], Sphere, Nodes);
			}
		}
	}
	
}

