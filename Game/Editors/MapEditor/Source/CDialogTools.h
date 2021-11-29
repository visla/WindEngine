#ifndef _DIALOG_TOOLS
#define _DIALOG_TOOLS

#include <CString.h>
#include <CComponent.h>

extern String FileName;
extern String FileNameNoPath;
bool GetFile(char Filter[256], String defExt = "");

void FillTree(CTreeView* PackageTree, int Type, bool FindNames = true, CComboBox* PackageName = NULL, StringParam Extension = "*.wep");

CTreeViewItem* FindItem(CTreeView* TreeView, String FullName);
CTreeViewItem* AddItem(CTreeView* TreeView, String FullName);
String GetFullItemName(CTreeViewItem* Item);

typedef void (*RefreshPackageCallback)(StringParam ItemName, StringParam FileName, void *Param);

void RefreshPackageItems(StringParam BaseDirPath, StringParam PackageName, RefreshPackageCallback cbRefresh, void *Param);

void PrepareTextureList(CTreeView* TextureList);
void UpdateTextureList(CTreeView* TextureList);

#endif
