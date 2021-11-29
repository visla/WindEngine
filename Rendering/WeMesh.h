/*
          File: WeMesh.h
		  Author: Tomislav Kukic
		  Date: 12.09.2005
		  Version: 1.0
*/


#ifndef _WEMESH
#define _WEMESH



#include <d3dx9.h>
#include <CString.h>
#include <SmartList.h>
#include <WeAttribRange.h>
#include <WindEngine.h>
#include <WeBox.h>
#include <WeSphere.h>
#include <WeTextStream.h>


#define MAX_MATERIAL_PRESETS	10
#define MAX_MATERIAL_PRESETS2	30
#define MAX_HARDWAREMESHES		10



class WeBox;
class WeMaterial;
class WeMesh;








// this is structure of one vertex in mesh after loading or when creating dynamic
struct WeMeshVertex
{
	WeVector Position;
	WeVector Normal;
	D3DCOLOR Color;
	float    u, v;
};

struct WeActualMeshVertex
{
	WeVector Position;
	WeVector Normal;
	float u,v;
	float lu, lv;
};


struct WeNormalMappedMeshVertexOld
{
	WeVector Position;
	WeVector Normal;
	WeVector Tangent;
	float u, v;
	float lu, lv;
};


struct WeNormalMappedMeshVertex
{
	WeVector Position;
	WeVector Normal;
	float u, v;
	WeVector Tangent;
	float lu, lv;
};


extern D3DVERTEXELEMENT9 ActualMeshVertexElements[];
extern D3DVERTEXELEMENT9 NormalMappedMeshVertexElements[];



class WINDENGINE_API WeMesh
{
friend class WeStaticModel;
friend class WeModelFrame;

protected:
	LPD3DXMESH HardwareMesh;
	LPD3DXMESH *HardwareMeshes;
	LPD3DXMESH CurrentMesh;

	LPD3DXMESH ShadowMesh;
	LPDIRECT3DVERTEXBUFFER9 lpVB;
	LPDIRECT3DINDEXBUFFER9  lpIB;

	// Stuff needed for dynamic meshes
	int						Version;
	bool					RefreshPresets;
	int						DynamicFaces;
	int						DynamicVertices;
	WeMaterial*				DynamicMaterial;
	bool					DeclarationWithColor; // for dynamic meshes to now how it was created
	bool					UseNormalMap;
	bool					NewUseNormalMap;	
	bool					Unwrapped;
	DWORD					*Adjancency;

	HRESULT					GenerateShadowMesh( IDirect3DDevice9 *pd3dDevice, ID3DXMesh *pMesh, ID3DXMesh **ppOutMesh );
	bool					GetRefresh() { bool t; t = RefreshPresets; RefreshPresets = false; return t; }
	void					SimplifyMesh();

	LPDIRECT3DVERTEXDECLARATION9 GetActualVertexDecl();
	LPDIRECT3DVERTEXDECLARATION9 GetNormaMappedVertexDecl();

public:	
							WeMesh(void); // this is public for testing
	virtual					~WeMesh(void);

	CSmartList<WeAttribRange> MeshAttributes;
	CSmartList<WeMaterial*>	Materials[MAX_MATERIAL_PRESETS2];
	Strings					MaterialNames[MAX_MATERIAL_PRESETS2];

	WeBox					BoundingBox;
	WeSphere				BSphere;

	int						LightMapSize;		  // size of lightmap this mesh should use
	int						VerticesCount;
	int						FacesCount;
	String					Name;
	bool					Dynamic;
	bool					Loaded;
	bool					HasAttrTable;	

	int						MaxLODLevels;
	float					MaxLODDistance; 
	float					MaxLODFactor;

	int						OnlyVisibleAttribute;

	void					ClearAll();

	virtual void			SetMesh(LPD3DXMESH Mesh, bool CreateAttribs = true, UINT MaterialNum = 0);
	virtual LPD3DXMESH		GetMesh(){ return HardwareMesh; }
	virtual bool			Optimize();
	void					DeOptimize(); // this will make mesh like only triangle list

	virtual void			DrawSubset(WeAttribRange* SubAttr, float ZDist = 0);
	void					DrawShadowSubset(WeAttribRange* SubAttr);
	void					LockVertexBuffer(void **data, DWORD Flags);
	void					LockIndexBuffer(void **data, DWORD Flags);
	void					LockAttributeBuffer(DWORD **data, DWORD Flags);
	void					UnlockVertexBuffer();
	void					UnlockIndexBuffer();
	void					UnlockAttributeBuffer();

	bool					GetUseNormalMap() {return UseNormalMap; }
	//void					SetUseNormalMap(bool use) { UseNormalMap = use; }

	bool					GetNewUseNormalMap() {return NewUseNormalMap; }
	void					SetNewUseNormalMap(bool use) { NewUseNormalMap = use; }

	int 					GetNumBytesPerIndex();
	int						GetNumBytesPerVertex();
	int						GetNumVertices();
	int						GetNumFaces();
	void					ComputeNormals();
	void					ComputeBoundings();	
	void					OnLostDevice();
	void					OnResetDevice();
	void					UnwrapLightMap(); // unwrap for lightmapping
	
	virtual void			Clone(WeMesh* Mesh);
	void					Refresh() { RefreshPresets = true; } // call this to 

	void					SetMaterial(StringParam MaterialName, int forMaterialPreset);
	

	// Loads mesh from x file or from package. Also loads specific material preset
	// If you are not using material presets leave this default
	// MeshManager is responsible for regular calling of this method so use this method
	// only when you really need to.
	bool					Load(StringParam name, bool MeshIsX);
	void					CreateDynamic(int NumFaces, int NumVertices, WeMaterial *Material, bool DeclarationWithColor = true);

	// Export mesh vertices to OBJ
	void					ExportVerticesToOBJ(WeTextStream &TS, WeMatrix &TM);
	// Export mesh faces to OBJ, starting vertex index is FirstVertex
	void					ExportFacesToOBJ(WeTextStream &TS, int GroupIndex, int FirstVertex);
};



#endif
