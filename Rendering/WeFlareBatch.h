#ifndef _WeFlareBatch
#define _WeFlareBatch


#include <SmartList.h>
#include <d3dx9math.h>


class WeBatchItem;
class WeLight;



class WeFlareBatch
{
public:
	~WeFlareBatch();

    WeLight		*Light;
	bool		InFrustum;
	bool		Occluded;
	float		OccludedTime;
	float		VisibleTime;
	D3DXVECTOR3	LightLocation; // in screen coordinates
};



#endif