/*
		File: WeModel.cpp
		Author: Tomislav Kukic
		Date: 10.07.2006
		Version: 1.0
*/



#include <assert.h>
#include <direct.h>
#include <WeModel.h>
#include <WeMeshHierarchy.h>
#include <WeRenderDevice.h>
#include <WeTimer.h>
#include <WeActor.h>
#include <WePackage.h>
#include <WeLoger.h>




WeModel::WeModel()
{
	IsModel = true;
	Version = 1;
	AnimationSpeed = 1.0f;
	UseNormalMap = true;
	maxBones = 0;
	FirstContainer = NULL;
	BoneMatrices = NULL;
	CurrentAnimationSet = -1;
}



WeModel::~WeModel()
{
}






void WeModel::UpdateTransformations(WeMatrix &TM, WeMatrix &PrevTM)
{
	AnimController->AdvanceTime(WeTimer::Instance()->GetDeltaTimeF()*AnimationSpeed, NULL);
	RootFrame->Update(TM, PrevTM, NULL);

	if(FirstContainer && FirstContainer->pSkinInfo)
	{
		int Bones = FirstContainer->pSkinInfo->GetNumBones();

		if(!BoneMatrices)
		{
			BoneMatrices = new WeMatrix[Bones];
			ZeroMemory(BoneMatrices, sizeof(WeMatrix)*Bones);
		}

		/*FILE* f = fopen("c:\\Test.txt","wt");

		fprintf(f,"Bone Offset 11 : %.2f, Bone Offset 12 : %.2f, Bone Offset 13 : %.2f, Bone Offset 14 : %.2f\n\
					   Bone Offset 21 : %.2f, Bone Offset 22 : %.2f, Bone Offset 23 : %.2f, Bone Offset 24 : %.2f\n\
					   Bone Offset 31 : %.2f, Bone Offset 32 : %.2f, Bone Offset 33 : %.2f, Bone Offset 34 : %.2f\n\
					   Bone Offset 41 : %.2f, Bone Offset 42 : %.2f, Bone Offset 43 : %.2f, Bone Offset 44: %.2f\n",
                       FirstContainer->BoneOffsets[2]._11, FirstContainer->BoneOffsets[2]._12, FirstContainer->BoneOffsets[2]._13, FirstContainer->BoneOffsets[2]._14,
					   FirstContainer->BoneOffsets[2]._21, FirstContainer->BoneOffsets[2]._22, FirstContainer->BoneOffsets[2]._23, FirstContainer->BoneOffsets[2]._24,
					   FirstContainer->BoneOffsets[2]._31, FirstContainer->BoneOffsets[2]._32, FirstContainer->BoneOffsets[2]._33, FirstContainer->BoneOffsets[2]._34,
					   FirstContainer->BoneOffsets[2]._41, FirstContainer->BoneOffsets[2]._42, FirstContainer->BoneOffsets[2]._43, FirstContainer->BoneOffsets[2]._44);

		fprintf(f,"FrameCombinedMatrices 11 : %.2f, FrameCombinedMatrices 12 : %.2f, FrameCombinedMatrices 13 : %.2f, FrameCombinedMatrices 14 : %.2f\n\
					   FrameCombinedMatrices 21 : %.2f, FrameCombinedMatrices 22 : %.2f, FrameCombinedMatrices 23 : %.2f, FrameCombinedMatrices 24 : %.2f\n\
					   FrameCombinedMatrices 31 : %.2f, FrameCombinedMatrices 32 : %.2f, FrameCombinedMatrices 33 : %.2f, FrameCombinedMatrices 34 : %.2f\n\
					   FrameCombinedMatrices 41 : %.2f, FrameCombinedMatrices 42 : %.2f, FrameCombinedMatrices 43 : %.2f, FrameCombinedMatrices 44: %.2f\n",
                       FirstContainer->FrameCombinedMatrices[2]->_11, FirstContainer->FrameCombinedMatrices[2]->_12, FirstContainer->FrameCombinedMatrices[2]->_13, FirstContainer->FrameCombinedMatrices[2]->_14,
					   FirstContainer->FrameCombinedMatrices[2]->_21, FirstContainer->FrameCombinedMatrices[2]->_22, FirstContainer->FrameCombinedMatrices[2]->_23, FirstContainer->FrameCombinedMatrices[2]->_24,
					   FirstContainer->FrameCombinedMatrices[2]->_31, FirstContainer->FrameCombinedMatrices[2]->_32, FirstContainer->FrameCombinedMatrices[2]->_33, FirstContainer->FrameCombinedMatrices[2]->_34,
					   FirstContainer->FrameCombinedMatrices[2]->_41, FirstContainer->FrameCombinedMatrices[2]->_42, FirstContainer->FrameCombinedMatrices[2]->_43, FirstContainer->FrameCombinedMatrices[2]->_44);
		fclose(f);*/

		for (int i = 0; i < Bones; ++i)
		{
			D3DXMatrixMultiply(&BoneMatrices[i], &FirstContainer->BoneOffsets[i], FirstContainer->FrameCombinedMatrices[i]);
		}


		void *srcPtr;
		FirstContainer->MeshData.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&srcPtr);

		void *destPtr;
		FirstContainer->Mesh->GetMesh()->LockVertexBuffer(0, (void**)&destPtr);

		FirstContainer->pSkinInfo->UpdateSkinnedMesh(BoneMatrices, NULL, srcPtr, destPtr);

		FirstContainer->MeshData.pMesh->UnlockVertexBuffer();
		FirstContainer->Mesh->GetMesh()->UnlockVertexBuffer();
	}
}








void WeModel::Draw(WeWorld* World, WeCamera* Cam, WeActor* Owner)
{
	if(!RootFrame) return;

	DrawAllFrames(World, Cam, Owner, RootFrame);
}







void WeModel::DrawAllFrames(WeWorld* World, WeCamera* Cam, WeActor* Owner, WeModelFrame* Frame)
{
	Frame->Draw(World, Cam, Owner);

	if(Frame->pFrameSibling)
		DrawAllFrames(World, Cam, Owner, (WeModelFrame*)Frame->pFrameSibling);

	if(Frame->pFrameFirstChild)
		DrawAllFrames(World, Cam, Owner, (WeModelFrame*)Frame->pFrameFirstChild);
}






void WeModel::SetAnimation(int Index)
{
	if (Index < 0)
		Index = 0;
	else if (Index >= NumAnimationSets)
		Index = NumAnimationSets - 1;

	if (Index != CurrentAnimationSet)
	{
		LPD3DXANIMATIONSET AnimSet;

		CurrentAnimationSet = Index;

		AnimController->ResetTime();
		AnimController->GetAnimationSet(CurrentAnimationSet, &AnimSet);
		AnimController->SetTrackAnimationSet(0, AnimSet);

		AnimSet->Release();
	}
}



bool WeModel::Load(StringParam name, bool FromPackage, bool ForceReload)
{
	RootFrame = NULL;
	AnimController = NULL;

	WeMeshHierarchy MeshHierarchy;

	if (FromPackage)
	{
		if (WePackage::Instance()->BeginReading(name, 10))
		{
			WeFileStream* FS = &WePackage::Instance()->DataStream;
			int tmpTest = FS->ReadInt();
			if(tmpTest != 11000)
			{
				WePackage::Instance()->EndReading();
				return false;
			}

			int Ver = FS->ReadInt();
			if (Ver == 1)
			{
				Version = Ver;

				int Size = FS->ReadInt();
				char* Data = new char[Size];
				FS->ReadData((void*)Data,(int)Size);

				HRESULT hr = D3DXLoadMeshHierarchyFromXInMemory((void*)Data, Size, D3DXMESH_32BIT | D3DXMESH_MANAGED, WeRenderDevice::Instance()->Device, 
					&MeshHierarchy, NULL, (LPD3DXFRAME*)&RootFrame, &AnimController);

				delete [] Data;

				if (FAILED(hr))
				{
					WePackage::Instance()->EndReading();
					return false;
				}

				if(RootFrame)
					RootFrame->Load(FS, NULL);

				FS->ReadInt(); // SetAnimation
				AnimationSpeed = FS->ReadFloat();
				FS->ReadBool(); // UseNormalMap
			}

			WePackage::Instance()->EndReading();
		}
	}
	else
	{
		FILE *f = fopen(name, "rb");
		if (!f)
			return false;

		fseek(f, 0, SEEK_END);
		int size = ftell(f);
		fseek(f, 0, SEEK_SET);
		char *data = new char[size];
		fread((void*)data, size, 1, f);
		fclose(f);

		//HRESULT hr = D3DXLoadMeshHierarchyFromX(Name.GetString(), D3DXMESH_32BIT | D3DXMESH_MANAGED, WeRenderDevice::Instance()->Device, 
		//	&MeshHierarchy, NULL, (LPD3DXFRAME*)&RootFrame, &AnimController);
		HRESULT hr = D3DXLoadMeshHierarchyFromXInMemory((void*)data, size, D3DXMESH_32BIT | D3DXMESH_MANAGED, WeRenderDevice::Instance()->Device, 
			&MeshHierarchy, NULL, (LPD3DXFRAME*)&RootFrame, &AnimController);

		delete [] data;

		if (FAILED(hr))
			return false;
	}

	if (!AnimController)
		return false;

	NumAnimationSets = AnimController->GetMaxNumAnimationSets();

	Name = name;

	if(RootFrame)
		SetupBoneMatrices(RootFrame, NULL);

	return true;
}



bool WeModel::Save(StringParam HoleName, bool Replace)
{
	int Size;
	char *MeshData;
	FILE* f;

	f = fopen(Name, "rb+");
	if (f)
	{
		fseek(f,0,SEEK_END);
		Size = ftell(f);

		fseek(f,0,SEEK_SET);
		MeshData = new char[Size];
		fread((void*)MeshData, Size, 1, f);
		fflush(f);
		fclose(f);
	}
	else
	{
		if (WePackage::Instance()->BeginReading(Name, 10))
		{
			WeFileStream* tmpFS = &WePackage::Instance()->DataStream;

			tmpFS->ReadInt(); // 11000
			tmpFS->ReadInt(); // Version

			Size = tmpFS->ReadInt();
			MeshData = new char[Size];
			tmpFS->ReadData((void*)MeshData, Size);

			WePackage::Instance()->EndReading();
		}
	}


	//if(Replace) WePackage::Instance()->Remove(HoleName);

	if(WePackage::Instance()->BeginWriting(HoleName, 10, Replace))
	{
		WeFileStream* FS = &WePackage::Instance()->DataStream;

		FS->WriteInt(11000);
		FS->WriteInt(Version);

		//============ Find file and copy it to package ==========================
		
		FS->WriteInt(Size);
		FS->WriteData(MeshData, Size);

		//=====================================================================

		// Save material names for frames that have meshes...
		RootFrame->Save(FS, NULL);

		FS->WriteInt(CurrentAnimationSet); // unused
		FS->WriteFloat(AnimationSpeed);
		FS->WriteBool(UseNormalMap); // unused

		WePackage::Instance()->EndWriting();

	}else 
		return false;

	return true;
}






void WeModel::SetupBoneMatrices(WeModelFrame* frame, WeMatrix* ParentMatrix)
{
	WeMeshContainer* tmpContainer = (WeMeshContainer*)frame->pMeshContainer;

	// If this frame has a mesh
	if(tmpContainer)
	{
		if(tmpContainer->pSkinInfo && tmpContainer->Mesh)
		{
			D3DVERTEXELEMENT9 Declaration[MAX_FVF_DECL_SIZE];
			if (FAILED(tmpContainer->Mesh->GetMesh()->GetDeclaration(Declaration)))
				return;

			LPD3DXMESH tmpForClon = NULL;

			tmpContainer->MeshData.pMesh->CloneMesh(D3DXMESH_MANAGED, 
				Declaration, WeRenderDevice::Instance()->Device, 
				&tmpForClon);

			tmpContainer->Mesh->SetMesh(tmpForClon,true);

			maxBones = tmpContainer->pSkinInfo->GetNumBones();

			for (int i = 0; i < (int)tmpContainer->pSkinInfo->GetNumBones(); i++)
			{   
				WeModelFrame* tempFrame = (WeModelFrame*)D3DXFrameFind(RootFrame, 
						tmpContainer->pSkinInfo->GetBoneName(i));

				tmpContainer->FrameCombinedMatrices[i] = &tempFrame->TM;
			}

			if (!FirstContainer)
			{
				FirstContainer = tmpContainer;
			}
		}
	}

	if(frame->pFrameSibling)
		SetupBoneMatrices((WeModelFrame*)frame->pFrameSibling, ParentMatrix);

	if(frame->pFrameFirstChild)
		SetupBoneMatrices((WeModelFrame*)frame->pFrameFirstChild, &frame->TM);

	if (!ParentMatrix)
	{
		BoneMatrices = new D3DXMATRIX[maxBones];
		ZeroMemory(BoneMatrices, sizeof(WeMatrix)*maxBones);
	}
}



void WeModel::ComputeBoundingBox(WeActor *Owner, WeBox *outBox, WeModelFrame *Frame)
{
	// calculate bbox of this frame. take mesh bbox from this frame.
	WeBox box;
	WeVector Min, Max;
	if (Frame->pMeshContainer)
	{
		if(((WeMeshContainer*)Frame->pMeshContainer)->Mesh)
		{
			box = Frame->BBox;
			if (outBox->Min.x > box.Min.x) outBox->Min.x = box.Min.x; 
			if (outBox->Min.y > box.Min.y) outBox->Min.y = box.Min.y; 
			if (outBox->Min.z > box.Min.z) outBox->Min.z = box.Min.z; 

			if (outBox->Max.x < box.Max.x) outBox->Max.x = box.Max.x; 
			if (outBox->Max.y < box.Max.y) outBox->Max.y = box.Max.y; 
			if (outBox->Max.z < box.Max.z) outBox->Max.z = box.Max.z; 
		}
	}
	
	if(Frame->pFrameSibling)
		ComputeBoundingBox(Owner, outBox, (WeModelFrame*)Frame->pFrameSibling);

	if(Frame->pFrameFirstChild)
		ComputeBoundingBox(Owner, outBox, (WeModelFrame*)Frame->pFrameFirstChild);	
}



void WeModel::ComputeBoundingBox(WeActor *Owner)
{
	WeBox finBox;
	// set to some really big values
	finBox.Max.x = -10000000.0f;
	finBox.Max.y = -10000000.0f;
	finBox.Max.z = -10000000.0f;
	finBox.Min.x = 10000000.0f;
	finBox.Min.y = 10000000.0f;
	finBox.Min.z = 10000000.0f;

	RootFrame->ComputeBoundingBox(Owner, Owner->TM, NULL);

	ComputeBoundingBox(Owner, &finBox, RootFrame);
	Owner->BBox.Min = finBox.Min;
	Owner->BBox.Max = finBox.Max;

	Owner->BSphere.Position = finBox.Max - finBox.Min / 2.0f;
	WeVector v;
	v = finBox.Max - finBox.Min;
	Owner->BSphere.Radius = D3DXVec3Length(&v);	
}





void WeModel::Clone(WeModel* Original, bool DeepCopy)
{
	Name = Original->Name;

	Original->AnimController->CloneAnimationController(
		Original->AnimController->GetMaxNumAnimationOutputs(),
		Original->AnimController->GetMaxNumAnimationSets(),
		Original->AnimController->GetMaxNumTracks(),
		Original->AnimController->GetMaxNumEvents(),
		&AnimController);

	RootFrame = CloneAllFrames(Original->RootFrame, DeepCopy);
	D3DXFrameRegisterNamedMatrices(RootFrame, AnimController);

	AnimationSpeed = Original->AnimationSpeed;

	//FindFirstContainer(RootFrame);
	SetupBoneMatrices(RootFrame, NULL);	

	NumAnimationSets = Original->NumAnimationSets;
	SetAnimation(Original->CurrentAnimationSet);
}


WeModelFrame* WeModel::CloneAllFrames(WeModelFrame* frame, bool DeepClone)
{
	WeModelFrame* tmpCurrent = NULL;

	if (frame)
	{
		tmpCurrent = new WeModelFrame();
		tmpCurrent->Clone(frame, DeepClone);

		if (frame->pFrameSibling)
		{
			tmpCurrent->pFrameSibling = CloneAllFrames((WeModelFrame*)frame->pFrameSibling, DeepClone);
		}

		if (frame->pFrameFirstChild)
		{
			tmpCurrent->pFrameFirstChild = CloneAllFrames((WeModelFrame*)frame->pFrameFirstChild, DeepClone);
		}
	}

	return tmpCurrent;
}



void WeModel::FindFirstContainer(WeModelFrame* frame)
{
	if (frame->pMeshContainer && frame->pMeshContainer->pSkinInfo && !FirstContainer)
	{
		FirstContainer = (WeMeshContainer*)frame->pMeshContainer;
	}

	if (frame->pFrameSibling)
	{
		FindFirstContainer((WeModelFrame*)frame->pFrameSibling);
	}

	if (frame->pFrameFirstChild)
	{
		FindFirstContainer((WeModelFrame*)frame->pFrameFirstChild);
	}
}



void WeModel::SetMaterial(StringParam MaterialName, int MaterialPreset)
{
	SetMaterial(MaterialName, MaterialPreset, RootFrame);
}



void WeModel::SetMaterial(StringParam MatName, int MaterialPreset, WeModelFrame* frame)
{
	if(frame->pMeshContainer)
	{
		((WeMeshContainer*)(frame->pMeshContainer))->Mesh->SetMaterial(MatName, MaterialPreset);
	}

	if(frame->pFrameSibling)
		SetMaterial(MatName, MaterialPreset, (WeModelFrame*)frame->pFrameSibling);

	if(frame->pFrameFirstChild)
		SetMaterial(MatName, MaterialPreset, (WeModelFrame*)frame->pFrameFirstChild);
}



Strings* WeModel::GetAnimationNames()
{
	LPD3DXANIMATIONSET tmpSet;
	AnimationNames.Clear();
	for(int i=0; i<(int)NumAnimationSets; i++)
	{
		AnimController->GetAnimationSet(i, &tmpSet);
		AnimationNames.PushBack(tmpSet->GetName());
	}
	return &AnimationNames;
}



void WeModel::SetUseNormalMaps(WeModelFrame* frame, bool Use)
{
	frame->SetUseNormalMaps(Use);

	if(frame->pFrameSibling)
		SetUseNormalMaps((WeModelFrame*)frame->pFrameSibling, Use);

	if(frame->pFrameFirstChild)
		SetUseNormalMaps((WeModelFrame*)frame->pFrameFirstChild, Use);

	UseNormalMap = Use;
}



void WeModel::LoadMaterials(WeActor* Owner)
{
	LoadMaterials(Owner->MaterialPreset);
}


void WeModel::LoadMaterials(int Preset)
{
	RootFrame->LoadMaterials(Preset);
}
