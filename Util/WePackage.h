/*
           File: WePackage.h
		   Author:  Tomsilav Kukic
		   Date: 13.09.2005.
		   Version: 1.0
		   Platform: Win32

		   This package is given in engine as nice to have basis. If you dont want to use it
		   or just want to implement some cleaver method, inherit load methods of items like 
		   texture, material, ... or inherit package methods
		   
*/





#ifndef _WEPACKAGE
#define _WEPACKAGE

#include <WeFileStream.h>
#include <CString.h>
#include <CStaticString.h>
#include <WindEngine.h>




#define PACKAGE_MAXITEMS		1000


struct WePackageIndex
{
	StringName			Group;
	StringName			Name;
	int				    TypeID;
	unsigned int		Offset;
	unsigned int		Size;
};



class WINDENGINE_API WePackage
{
private:
	static WePackage* _instance;

protected:
	WePackageIndex *IndexTable;
	int		MaxItemsCount;
	int		ItemsCount;
	String	PackageOpened;
	int		IndexTableOffset;

	unsigned int		StartWritePosition;
	bool				Writing;
	int					WritingItemIndex;
	WePackageIndex		Index; // for temp using
	
	


	WePackage(void);
	virtual ~WePackage(void) { ClosePackage(); };


	void SeekToBegining();
	void SeekToEnd();

	virtual void	OpenPackage(StringParam PackageName);
	virtual void	ClosePackage();
	virtual void	ReadIndexTable(WeFileStream *FS);
	virtual void	WriteIndexTable(WeFileStream *FS, bool AllocateTable = false);
	virtual void	WriteIndex(int ID, WePackageIndex &Index, WeFileStream *FS);
	virtual void	WriteItemsCount(WeFileStream *FS);
	virtual int		ReadChunk(int index, void *&lpdata); // pointer must be created
	virtual int		WriteChunk(void *data, int size, WeFileStream *FS);
	virtual void	ReplaceItem(StringParam PackageName, int index);


public:
	WeFileStream DataStream;
	String  PackageExtension;  // ex. ".wep", ".txt" must include dot
	Strings Groups;
	Strings Names;

	static WePackage* Instance();


	int			GetItemsCount() { return ItemsCount; }
	int			GetMaxItems() { return MaxItemsCount; }


	// Get Item names from package with some properties. List of strings is then filled
	// with results
	virtual Strings  *GetItemsNames(StringParam Package, int Type);
	virtual Strings  *GetItemsNames(StringParam Package, StringParam Group, int Type);
	virtual Strings  *GetGroupNames(StringParam Package, int Type);


	// string is supposed to be in format. package.group.name. It does automatic opening 
	virtual bool BeginReading(StringParam name, int TypeID); // after this you can use Data stream to read data
	virtual bool EndReading();

	// after this use data stream to write. It opens or creates new package
    // MAke new item ready for writing with this name, and typeid 
	virtual bool BeginWriting(StringParam name, int TypeID = 0, bool ReplaceIfExist = false ); 
	virtual bool EndWriting();
	virtual bool Remove(StringParam name, int TypeID);

	virtual bool CompactPackage(StringParam name);
};




#endif
