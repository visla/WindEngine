#include <CDialogTextureEditor.h>
#include <CEditorResourceManager.h>
#include <CMapEditorWindow.h>
#include <CDialogTools.h>
#include <CDialogSave.h>
#include <WeTextureManager.h>
#include <WePackage.h>
#include <CDialogMaterialEditor.h>
#include <CDialogMeshEditor.h>



//========================================================
//    Texture Editor Class functions
//========================================================


CDialogTextureEditor* TextureEditor = NULL;

bool CDialogTextureEditor::Create(HWND hParent, bool Show)
{
	if(!CDialog::Create(IDD_TEXTURE_EDITOR, EditorWindow->GetHWND(), false))
		return false;

	RenderBox = new CGroupBox(this, ID_TE_PREVIEW);
	RenderBox->SetVisible(true);

	TypeLabel = new CLabel(this, IDC_TE_TYPE);
	WidthLabel = new CLabel(this, IDC_TE_WIDTH);
	HeightLabel = new CLabel(this, IDC_TE_HEIGHT);
	DepthLabel = new CLabel(this, IDC_TE_DEPTH);
	LowestQuality = new CComboBox(this, IDC_TE_LOWESTQUALITY);

/*
	SaveSelected = new CButton(this, IDC_TE_SAVESELECTED);
	RemoveSelected = new CButton(this, IDC_TE_REMOVESELECTED);
*/

	TreeTextures = new CTreeView(this, IDC_TE_TREEVIEW);
	PrepareTextureList(TreeTextures);

	LowestQuality->Clear();
	LowestQuality->AddItem("Low");
	LowestQuality->AddItem("Medium");
	LowestQuality->AddItem("High");

	NoMipmaps = new CCheckBox(this, IDC_TEXTURE_NOMIPMAPS);

	Texture = NULL;
	SetTextureInfo();

	return true;
}



void CDialogTextureEditor::OnShow()
{
	CTreeViewItem *Item;
	String AutoTextureName = "";

	FillTree(TreeTextures, PACKAGEITEM_TEXTURE);
	UpdateTextureList(TreeTextures);

	Texture = NULL;
	SetTextureInfo();

	if (MaterialEditor->GetVisible())
	{
		Item = MaterialEditor->TextureList->GetSelected();
		if (Item)
		{
			AutoTextureName = GetFullItemName(Item);
		}
	}

	else if (MeshEditor->GetVisible())
	{
		if (MeshEditor->PreviewTexture)
		{
			AutoTextureName = MeshEditor->PreviewTexture->Name;
		}
	}

	Item = FindItem(TreeTextures, AutoTextureName);
	if (Item)
		TreeTextures->SelectItem(Item);
}


void CDialogTextureEditor::OnClick(DWORD ID)
{
	CTreeViewItem *Item;

	switch(ID)
	{
	case IDC_TE_IMPORTTEXTURE:
		Item = TreeTextures->GetSelected();
		//if (Item && (Item->GetLevel() == 1))
		{
			GetFile("Texture files (*.dds;*.jpg;*.gif;*.tga;*.bmp)\0*.dds;*.jpg;*.gif;*.tga;*.bmp\0\0","");
			if(FileName.GetLen() == 0)
				return;

			WeTextureEditor *Tex = (WeTextureEditor*)WeTextureManager::Instance()->Load(FileName, false, false, true);
			if (Tex)
			{
				String temp = FileName;
				int p;

				while ((p = temp.SubPos("\\")) >= 0)
					temp = temp.Sub(p + 1, temp.GetLen());

				SaveDialog->HoleName =
					SaveDialog->HoleName.Extract('.', 0) + "." +
					SaveDialog->HoleName.Extract('.', 1) + "." +
					temp.Extract('.', 0);

				SaveTexture(Tex, "");
			}
		}
		break;

	case IDC_TE_EXPORTDDS:
		if (Texture)
		{
			//if(!GetFile("DDS Texture files *.dds\0*.dds\0\0", "dds")) return;

			char TexName[MAX_PATH], PathName[MAX_PATH];
			sprintf(TexName, "_Textures.%s", Texture->Name.GetString());

			for (int i = 0; i < (int)strlen(TexName); ++i)
			{
				if (TexName[i] == '.')
				{
					TexName[i] = '\\';
					strncpy(PathName, TexName, i);
					PathName[i] = (char)0;

					CreateDirectory(PathName, NULL);
				}
			}

			strcpy(&TexName[strlen(TexName)], ".dds");
			D3DXSaveTextureToFile(TexName, D3DXIFF_DDS, Texture->GetTexture(), NULL);
		}
		break;

	case IDC_TE_TREEVIEW:
		Texture = NULL;
		Item = TreeTextures->GetSelected();
		if (Item && (Item->GetLevel() == 2))
		{
			String TexName = GetFullItemName(Item);
			Texture = (WeTextureEditor*)WeTextureManager::Instance()->Load(TexName, true, false, true);
		}
		SetTextureInfo();
		break;

	case IDC_TE_SAVESELECTED:
		if (Texture)
		{
			SaveTexture(Texture, Texture->Name);
		}
		break;

	case IDC_TE_SAVESELECTED_AS:
		if (Texture)
		{
			SaveTexture(Texture, "");
		}
		break;

	case IDC_TE_REMOVESELECTED:
		Item = TreeTextures->GetSelected();
		if (Item && (IDYES == MessageBox(GetHWND(), "Are you sure you want to remove selected item(s)?", "Remove", MB_YESNO | MB_ICONQUESTION)))
		{
			if (Item->GetLevel() == 0)
			{
				for (TreeTextures->Items.First(); !TreeTextures->Items.End(); TreeTextures->Items.Next())
				{
					CTreeViewItem *Item2 = &TreeTextures->Items.GetItem();
					if ((Item2->GetLevel() == 2) && (Item2->GetParent()->GetParent() == Item))
					{
						String ItemName = GetFullItemName(Item2);
						WePackage::Instance()->Remove(ItemName, PACKAGEITEM_TEXTURE);
						if (Texture && (Texture->Name == ItemName))
						{
							Texture = NULL;
							SetTextureInfo();
						}
					}
				}
				TreeTextures->DeleteSelected();
			}
			
			else if (Item->GetLevel() == 1)
			{
				for (TreeTextures->Items.First(); !TreeTextures->Items.End(); TreeTextures->Items.Next())
				{
					CTreeViewItem *Item2 = &TreeTextures->Items.GetItem();
					if ((Item2->GetLevel() == 2) && (Item2->GetParent() == Item))
					{
						String ItemName = GetFullItemName(Item2);
						WePackage::Instance()->Remove(ItemName, PACKAGEITEM_TEXTURE);
						if (Texture && (Texture->Name == ItemName))
						{
							Texture = NULL;
							SetTextureInfo();
						}
					}
				}
				TreeTextures->DeleteSelected();
			}

			else if (Item->GetLevel() == 2)
			{
				if (Texture)
				{
					WePackage::Instance()->Remove(Texture->Name, PACKAGEITEM_TEXTURE);
					Texture = NULL;
					SetTextureInfo();
					TreeTextures->DeleteSelected();
				}
			}
		}

		break;

	case IDC_TE_REFRESHPACKAGE:
		CTreeViewItem* PackageItem;
		PackageItem = TreeTextures->GetSelected();

		if (PackageItem && (PackageItem->GetLevel() == 0))
		{
			OnRefreshPackage(PackageItem->GetText());
		}
		break;

	case IDC_TE_LOWESTQUALITY:
		if (Texture)
		{
			Texture->SetLowestQuality((WeTextureQuality)(TEXTURES_LOW + LowestQuality->GetSelectionIndex()));
		}
		break;

	case IDC_TEXTURE_NOMIPMAPS:
		if (Texture)
			Texture->NoMipmaps = NoMipmaps->GetChecked();
		break;

	}
}


void CDialogTextureEditor::SaveTexture(WeTextureEditor *Tex, StringParam NewName)
// NewName == "" if user needs to choose new name
{
	bool Res = false, ok = false;
	String SaveName = NewName;

	if (!Tex)
	{
		return;
	}

	// Get NewName, if it is needed
	if (SaveName.GetLen() == 0)
	{
		SaveDialog->Type = PACKAGEITEM_TEXTURE;
		if (SaveDialog->ShowModal(GetHWND()) == MR_OK)
		{
			SaveName = SaveDialog->HoleName;
		}
	}

	if (SaveName.GetLen() == 0)
	{
		return;
	}

	// Save Texture
	ok = Tex->Save(SaveName, (NewName.GetLen() != 0));
	if (!ok)
	{
		if (IDYES == MessageBox(GetHWND(), "Item already exist, overwrite?","Save",MB_YESNO | MB_ICONQUESTION))
		{
			ok = Tex->Save(SaveName, true);
		}
	}

	if (ok && !FindItem(TreeTextures, SaveName))
	{
		AddItem(TreeTextures, SaveName);
	}
}



void CDialogTextureEditor::OnExit()
{
	SAFE_DELETE(Texture);
	SetTextureInfo();
	SetVisible(false);
}



void RefreshTexture(StringParam ItemName, StringParam FileName, void *Param)
{
	FILETIME OldModifyTime, ModifyTime;
	OldModifyTime.dwLowDateTime = 0;
	OldModifyTime.dwHighDateTime = 0;
	ModifyTime.dwLowDateTime = 0;
	ModifyTime.dwHighDateTime = 0;

	if (WePackage::Instance()->BeginReading(ItemName, PACKAGEITEM_TEXTURE))
	{
		int ver = 0;

		int fSize = WePackage::Instance()->DataStream.ReadInt();
		if (fSize < 0) // we use versioned textures!
		{
			fSize = -fSize;
			ver = WePackage::Instance()->DataStream.ReadInt();
		}

		if (ver == 1)
		{
			WePackage::Instance()->DataStream.Seek(fSize, SEEK_CUR);

			WePackage::Instance()->DataStream.ReadString(); // Name
			WePackage::Instance()->DataStream.ReadDWord(); // Type

			WePackage::Instance()->DataStream.ReadDWord(); // Format
			WePackage::Instance()->DataStream.ReadDWord(); // Usage
			WePackage::Instance()->DataStream.ReadDWord(); // Pool

			WePackage::Instance()->DataStream.ReadInt(); // Width
			WePackage::Instance()->DataStream.ReadInt(); // Height
			WePackage::Instance()->DataStream.ReadInt(); // Depth
		} // end of version 1

		else if (ver == 2)
		{
			WePackage::Instance()->DataStream.Seek(fSize, SEEK_CUR);

			WePackage::Instance()->DataStream.ReadString(); // Name
			WePackage::Instance()->DataStream.ReadDWord(); // Type

			WePackage::Instance()->DataStream.ReadDWord(); // Format
			WePackage::Instance()->DataStream.ReadDWord(); // Usage
			WePackage::Instance()->DataStream.ReadDWord(); // Pool

			WePackage::Instance()->DataStream.ReadInt(); // Width
			WePackage::Instance()->DataStream.ReadInt(); // Height
			WePackage::Instance()->DataStream.ReadInt(); // Depth

			WePackage::Instance()->DataStream.ReadInt(); // LowestQuality

			OldModifyTime.dwLowDateTime = WePackage::Instance()->DataStream.ReadDWord();
			OldModifyTime.dwHighDateTime = WePackage::Instance()->DataStream.ReadDWord();
		}

		else if (ver == 3)
		{
			WePackage::Instance()->DataStream.ReadDWord(); // Type

			WePackage::Instance()->DataStream.ReadDWord(); // Format
			WePackage::Instance()->DataStream.ReadDWord(); // Usage
			WePackage::Instance()->DataStream.ReadDWord(); // Pool

			WePackage::Instance()->DataStream.ReadInt(); // Width
			WePackage::Instance()->DataStream.ReadInt(); // Height
			WePackage::Instance()->DataStream.ReadInt(); // Depth

			WePackage::Instance()->DataStream.ReadInt(); // LowestQuality

			OldModifyTime.dwLowDateTime = WePackage::Instance()->DataStream.ReadDWord();
			OldModifyTime.dwHighDateTime = WePackage::Instance()->DataStream.ReadDWord();

			// rest
		}

		WePackage::Instance()->EndReading();
	}


	HANDLE f = CreateFile(FileName.GetString(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
	if (f != INVALID_HANDLE_VALUE)
	{
		GetFileTime(f, NULL, NULL, &ModifyTime);
		CloseHandle(f);

		if ((OldModifyTime.dwLowDateTime != ModifyTime.dwLowDateTime) || (OldModifyTime.dwHighDateTime != ModifyTime.dwHighDateTime))
		{
			WeTextureEditor *Tex = (WeTextureEditor*)WeTextureManager::Instance()->Load(FileName, false, false, true);
			if (Tex)
			{
				Tex->ModifyTime = ModifyTime;
				TextureEditor->SaveTexture(Tex, ItemName);
			}
		}
	}
}


void CDialogTextureEditor::OnRefreshPackage(StringParam PackageName)
{
	RefreshPackageItems("_Textures", PackageName, &RefreshTexture, this);
	if (PackageName.GetLen() != 0)
		MessageBox(0, ((String)"Package " + PackageName + (String)" refreshed").GetString(), "Texture Editor", 0);
}


void CDialogTextureEditor::SetTextureInfo()
{
	if(Texture)
	{
		DWORD tmpType = Texture->GetType();

		switch(tmpType)
		{
		case WETEXTYPE_2D:
			switch (Texture->Format)
			{
			case D3DFMT_DXT1:
				TypeLabel->SetText("2D (DXT1)");
				break;
			case D3DFMT_DXT2:
				TypeLabel->SetText("2D (DXT2)");
				break;
			case D3DFMT_DXT3:
				TypeLabel->SetText("2D (DXT3)");
				break;
			case D3DFMT_DXT4:
				TypeLabel->SetText("2D (DXT4)");
				break;
			case D3DFMT_DXT5:
				TypeLabel->SetText("2D (DXT5)");
				break;
			default:
				TypeLabel->SetText("2D");
			}

			break;
		case WETEXTYPE_CUBE:
			TypeLabel->SetText("CUBE");
			break;
		case WETEXTYPE_VOLUME:
			TypeLabel->SetText("VOLUME");
			break;
		case WETEXTYPE_NONE:
			TypeLabel->SetText("NONE");
			break;
		}

		char tmpText[100];

		sprintf(tmpText,"%d", Texture->Width);
		WidthLabel->SetText(tmpText);

		sprintf(tmpText,"%d", Texture->Height);
		HeightLabel->SetText(tmpText);

		sprintf(tmpText,"%d", Texture->Depth);
		DepthLabel->SetText(tmpText);

		LowestQuality->SetSelection(Texture->GetLowestQuality() - TEXTURES_LOW);

		NoMipmaps->SetChecked(Texture->NoMipmaps);
	}
	else
	{
		TypeLabel->SetText("");
		WidthLabel->SetText("");
		HeightLabel->SetText("");
		DepthLabel->SetText("");
		LowestQuality->SetSelection(-1);
	}
}


