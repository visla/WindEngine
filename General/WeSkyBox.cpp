/*
    WeSkyBox.cpp

	Autor: Nebojsa Vislavski
	Date: 3.3.2006.
	Version: 1.0
	Platform: Win32/Linux

	This is sky box

*/


#include <WeSkyBox.h>
#include <WeSkyBoxRenderer.h>
#include <WeMaterialManager.h>
#include <WeRenderDevice.h>
#include <WeMeshManager.h>
#include <WeShaderManager.h>
#include <WeTimer.h>
#include <WeEngine.h>
#include <WeSkyBoxRenderer.h>
#include <WeCamera.h>


WeSkyBox::WeSkyBox()
{
	Type = WETYPE_SKYBOX;
	SkyBoxMesh = NULL;

	SkyBoxMeshName = "";
	Version = 1;
}


void WeSkyBox::OnRender(WeCamera *Cam, WeShader *UseEffect)
{
	if (Renderable && Editor_Renderable)
		if (SkyBoxMesh)
			WeSkyBoxRenderer::Instance()->Render(this, Cam, UseEffect);
}


void WeSkyBox::Initialize()
{
	if (SkyBoxMeshName.GetLen() == 0) return;

	SkyBoxMesh = WeMeshManager::Instance()->Load(SkyBoxMeshName, false);

	if (SkyBoxMesh)
	{
		CSmartList<WeMaterial*>* Materials;
		Materials = &SkyBoxMesh->Materials[0];
		if (Materials->Empty())
		{
			int Size;
			WeMaterial *tmpMat;

			if (!Materials->Empty()) Materials->Clear();
			Size = SkyBoxMesh->MeshAttributes.GetItemsCount();

			SkyBoxMesh->MaterialNames[0].First();
			for (int i = 0; i < Size; i++)
			{
				tmpMat = WeMaterialManager::Instance()->Load(SkyBoxMesh->MaterialNames[0].GetItem());
				if (!tmpMat)
					tmpMat = WeMaterialManager::Instance()->Load(WeEngine::Instance()->Desc.DefaultMaterialName);
				Materials->PushBack(tmpMat);
				SkyBoxMesh->MaterialNames[0].Next();
			}
		}
	}
}

void WeSkyBox::Serialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	FS.WriteInt(Version);
	FS.WriteString(SkyBoxMeshName);
}

void WeSkyBox::Deserialize(WeFileStream &FS)
{
	if (!FS.IsOpened) return;

	int ver = FS.ReadInt();
	switch(ver)
	{
	case 1:
		SkyBoxMeshName = FS.ReadString();
		break;
	}
}
