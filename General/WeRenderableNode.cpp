/*
    File: WeRenderableNode.cpp
	Author: Tomislav Kukic
	Date: 13.09.2005.
	Version: 1.0
*/





#include <WeRenderableNode.h>



WeRenderableNode::WeRenderableNode(void)
{
	for (int i = 0; i < 8; i++)
	{
		Children[i] = NULL;
	}
	Used = false;
}

WeRenderableNode::WeRenderableNode(WeBox &Box)
{
	for (int i = 0; i < 8; i++)
	{
		Children[i] = NULL;
	}
	BBox = Box;
	// Make BSphere
	BSphere.Position = Box.Min + (WeVector(1,1,1)*(Box.GetSize()/2.0f));
	BSphere.Radius = Box.GetSize()*0.8660254f;
	Used = false;
}

WeRenderableNode::~WeRenderableNode(void)
{
	Prefabs.Clear();

    for (int i = 0; i < 8; i++)
	{
		SAFE_DELETE(Children[i]);
	}		
	Used = false;
}


