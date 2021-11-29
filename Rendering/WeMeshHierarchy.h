/*
		File: WeMeshHierarchy.h
		Author: Tomislav Kukic
		Date: 10.07.2006
		Version: 1.0
*/


#ifndef _WEMESHHIERARCHY_H_
#define _WEMESHHIERARCHY_H_


#include <d3dx9.h>



class WeMeshHierarchy : public ID3DXAllocateHierarchy
{
public:	
	WeMeshHierarchy(){}
	~WeMeshHierarchy(){}


    // callback to create a D3DXFRAME extended object and initialize it
    STDMETHOD( CreateFrame )( THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame );



    // callback to create a D3DXMESHCONTAINER extended object and initialize it
    STDMETHOD( CreateMeshContainer )( THIS_ LPCSTR Name, CONST D3DXMESHDATA * pMeshData, 
                            CONST D3DXMATERIAL * pMaterials, CONST D3DXEFFECTINSTANCE * pEffectInstances,
                            DWORD NumMaterials, CONST DWORD * pAdjacency, LPD3DXSKININFO pSkinInfo, 
                            LPD3DXMESHCONTAINER * ppNewMeshContainer );



    // callback to release a D3DXFRAME extended object
    STDMETHOD( DestroyFrame )( THIS_ LPD3DXFRAME pFrameToFree );



    // callback to release a D3DXMESHCONTAINER extended object
    STDMETHOD( DestroyMeshContainer )( THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree );
};





#endif
