#include <CDialogTools.h>
#include <CComponent.h>
#include <WePackage.h>
#include <direct.h>
#include <windows.h>
#include <Commdlg.h>
#include <CMapEditorWindow.h>
#include <CDialogSave.h>


String FileName;
String FileNameNoPath;


bool GetFile(char Filter[256], String defExt)
{
	static char CurrentDir[MAX_PATH] = ".";
	char OldDir[MAX_PATH];
	bool ok;

	getcwd(OldDir, MAX_PATH);


	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name
	char szFileName[260];   // buffer for file name without path information
	//HANDLE hf;              // file handle

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = EditorWindow->GetHWND();
	ofn.lpstrFile = szFile;
	ofn.lpstrFileTitle = szFileName;
	ofn.lpstrInitialDir = CurrentDir;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.nMaxFileTitle = sizeof(szFileName);
	ofn.lpstrFilter = Filter;//"Effect files *.fx\0*.fx\0\0";
	ofn.nFilterIndex = 1;
	if(defExt == (String)"")
		ofn.lpstrDefExt = NULL;
	else
		ofn.lpstrDefExt = defExt.GetString();

	if(!SaveDialog)
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
		
	// Display the Open dialog box. 
	ok = GetOpenFileName(&ofn) != 0;
	getcwd(CurrentDir, MAX_PATH);

	if (ok)
	{
		FileName = szFile;
		FileNameNoPath = szFileName;
	}
	else	
	{
		FileName = "";
		FileNameNoPath = "";
	}

	chdir(OldDir);
	return ok;
}



void FillTree(CTreeView* PackageTree, int Type, bool FindNames, CComboBox* PackageName, StringParam Extension)
{
	Strings PackageList;

	WIN32_FIND_DATA FindData;
	HANDLE fHandle;
	String tmp;

	PackageList.Clear();
	PackageTree->Clear();
	if(PackageName) PackageName->Clear();

	fHandle = FindFirstFile(Extension.GetString(), &FindData);
	if (fHandle != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			tmp = FindData.cFileName;
			tmp = tmp.Extract('.', 0);

			PackageList.PushBack(tmp);
		} while (FindNextFile(fHandle, &FindData));
			FindClose(fHandle);
	}

	if(PackageList.GetItemsCount() == 0)
	{
		PackageTree->Add(NULL,"NO ITEMS FOUND!!!");
		return;
	}

	String PExt = Extension;
	PExt.RemoveChar(0);
	WePackage::Instance()->PackageExtension = PExt;

	CTreeViewItem *ItemPackage, *ItemGroup;

	for(PackageList.First(); !PackageList.End(); PackageList.Next())
	{
		ItemPackage = NULL;
		if (PackageName)
		{
			PackageName->AddItem(PackageList.GetItem());
		}

		Strings* GroupNames;
		GroupNames = WePackage::Instance()->GetGroupNames(PackageList.GetItem(), Type);

		for(GroupNames->First(); !GroupNames->End(); GroupNames->Next())
		{
			ItemGroup = NULL;

			if(FindNames)
			{
				Strings* NameList;
				NameList = WePackage::Instance()->GetItemsNames(PackageList.GetItem(), GroupNames->GetItem(), Type);

				for(NameList->First(); !NameList->End(); NameList->Next())
				{
					String tmpName = NameList->GetItem().Extract('*',1);
					if(tmpName != (String)"Info")
					{
						if (!ItemPackage)
						{
							ItemPackage = PackageTree->Add(NULL,PackageList.GetItem());
						}
						if (!ItemGroup)
						{
							ItemGroup = PackageTree->Add(ItemPackage, GroupNames->GetItem());
						}
						PackageTree->Add(ItemGroup, NameList->GetItem());
					}
				}
			}
		}
	}
}


CTreeViewItem* FindItem(CTreeView* TreeView, String FullName)
{
	CTreeViewItem* Item;
	int level;
	String LevelName;

	level = 0;
	Item = NULL;

	LevelName = FullName.Extract('.', level);
	while (LevelName.GetLen() != 0)
	{
		Item = TreeView->FindChild(Item, LevelName);
		if (!Item)
			return NULL;

		level++;
		LevelName = FullName.Extract('.', level);
	}

	return Item;
}


CTreeViewItem* AddItem(CTreeView* TreeView, String FullName)
{
	CTreeViewItem* Item = NULL;
	int level = 0;
	String LevelName;

	LevelName = FullName.Extract('.', level);
	while (LevelName.GetLen() != 0)
	{
		CTreeViewItem* ParentItem = Item;
		Item = TreeView->FindChild(ParentItem, LevelName);
		if (!Item)
			Item = TreeView->Add(ParentItem, LevelName);

		level++;
		LevelName = FullName.Extract('.', level);
	}

	return Item;
}


String GetFullItemName(CTreeViewItem* Item)
{
	String Name = "";
	
	while (Item != NULL)
	{
		if (Name.GetLen() != 0)
		{
			Name = (String)"." + Name;
		}
		Name = Item->GetText() + Name;
		Item = Item->GetParent();
	}

	return Name;
}


// BaseDirPath je bez \\ na kraju
void RefreshPackageItems(StringParam BaseDirPath, StringParam aPackageName, RefreshPackageCallback cbRefresh, void *Param)
{
	WIN32_FIND_DATA FindDataPackage, FindDataGroup, FindDataItem;
	HANDLE hFindPackage, hFindGroup, hFindItem;
	String PackageName, GroupName, ItemName;
	String FullName, FullFileName;

	//PackagePath = BaseDirPath + (String)"\\" + PackageName;

	hFindPackage = FindFirstFile((char*)(BaseDirPath + "\\*.*"), &FindDataPackage);
	if (hFindPackage != INVALID_HANDLE_VALUE) 
	{
		do
		{
			PackageName = (String)FindDataPackage.cFileName;
			if ((PackageName[0] != '.') && ((FindDataPackage.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
			{
				if ((aPackageName.GetLen() == 0) || (PackageName.EqualCaseInsensitive(aPackageName)))
				{
					hFindGroup = FindFirstFile((char*)(BaseDirPath + "\\" + PackageName + "\\*.*"), &FindDataGroup);
					if (hFindGroup != INVALID_HANDLE_VALUE) 
					{
						do
						{
							GroupName = (String)FindDataGroup.cFileName;
							if ((GroupName[0] != '.') && ((FindDataGroup.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0))
							{
								hFindItem = FindFirstFile((char*)(BaseDirPath + "\\" + PackageName + "\\" + GroupName + "\\*.*"), &FindDataItem);
								if (hFindItem != INVALID_HANDLE_VALUE) 
								{
									do
									{
										if ((FindDataItem.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
										{
											ItemName = (String)FindDataItem.cFileName;
											FullFileName = BaseDirPath + "\\" + PackageName + "\\" + GroupName + "\\" + ItemName;
											FullName = PackageName + "." + GroupName + "." + ItemName.Extract('.', 0);

											(*cbRefresh)(FullName, FullFileName, Param);
										}
									} while (FindNextFile(hFindItem, &FindDataItem));

									if (GetLastError() == ERROR_NO_MORE_FILES) 
										FindClose(hFindItem);
								}
							}
						} while (FindNextFile(hFindGroup, &FindDataGroup));

						if (GetLastError() == ERROR_NO_MORE_FILES) 
							FindClose(hFindGroup);
					}
				}
			}
		} while (FindNextFile(hFindPackage, &FindDataPackage));

		if (GetLastError() == ERROR_NO_MORE_FILES) 
			FindClose(hFindPackage);
	}
}


void PrepareTextureList(CTreeView* TextureList)
{
	Strings StateImageNames;
	StateImageNames.PushBack("transparent.bmp"); // 0
	StateImageNames.PushBack("diffuse.bmp"); // 1
	StateImageNames.PushBack("specular.bmp"); // 2
	StateImageNames.PushBack("normal.bmp"); // 3
	StateImageNames.PushBack("cube.bmp"); // 4
	StateImageNames.PushBack("other.bmp"); // 5
	TextureList->SetStateImages(StateImageNames);
}


void UpdateTextureList(CTreeView* TextureList)
{
	for (TextureList->Items.First(); !TextureList->Items.End(); TextureList->Items.Next())
	{
		CTreeViewItem *Item = &TextureList->Items.GetItem();
		if (Item->GetLevel() == 2)
		{
			String name = GetFullItemName(Item);
			int len = name.GetLen();
			int state = 0;

			if (name.Sub(len - 7, len) == (String)"Diffuse")
				state = 1;
			else if (name.Sub(len - 8, len) == (String)"Specular")
				state = 2;
			else if (name.Sub(len - 6, len) == (String)"Normal")
				state = 3;
			else if (name.Sub(len - 5, len) == (String)"_CUBE")
				state = 4;
			else
				state = 5;

			if (state > 0)
				Item->SetState(state);
		}
	}
}

