/*
           File: WePackage.cpp
		   Author:  Tomsilav Kukic
		   Date: 13.09.2005.
		   Version: 1.0
*/





#include <WePackage.h>
#include <stdio.h>
#include <windows.h>
#include <malloc.h>
#include <WeLoger.h>
#include <WeTimer.h>


WePackage *WePackage::_instance = NULL;


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
WePackage::WePackage(void)
{
	ItemsCount = 0;
	MaxItemsCount = 0;
	IndexTable = NULL;
	PackageExtension = ".wep";
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WePackage::SeekToBegining()
{
	DataStream.Seek(0);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WePackage::SeekToEnd()
{
	DataStream.Seek(0, SEEK_END);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
WePackage* WePackage::Instance()
{
	if (_instance) return _instance;
	_instance = new WePackage;
	return _instance;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WePackage::OpenPackage(StringParam PackageName)
{	
	if (PackageOpened != PackageName)
	{
		DataStream.Open(PackageName);
		DataStream.Seek(0);

		ItemsCount = DataStream.ReadInt();
		MaxItemsCount = DataStream.ReadInt();

		IndexTableOffset = DataStream.Tell();
		// read index.
		ReadIndexTable(&DataStream);
		PackageOpened = PackageName;
	} else DataStream.Seek(0);
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WePackage::ReadIndexTable(WeFileStream *FS)
{
    if (IndexTable) delete[] IndexTable; 
	IndexTable = new WePackageIndex[MaxItemsCount];

	FS->Seek(IndexTableOffset);
	for (int i = 0; i < ItemsCount; i++)
	{
		FS->ReadStringName(IndexTable[i].Group);
		FS->ReadStringName(IndexTable[i].Name);
		IndexTable[i].TypeID = FS->ReadInt();
		IndexTable[i].Offset = FS->ReadInt();
		IndexTable[i].Size = FS->ReadInt();
	}
	
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WePackage::WriteIndexTable(WeFileStream *FS, bool AllocateTable)
{
	if (AllocateTable)
	{
      if (IndexTable) delete[] IndexTable; 
	  IndexTable = new WePackageIndex[MaxItemsCount];
	}

	StringName s = "";

	
	FS->Seek(IndexTableOffset);
	for (int i = 0; i < MaxItemsCount; i++)
	{
		FS->WriteStringName(s);
		FS->WriteStringName(s);
		FS->WriteInt(0);
		FS->WriteInt(0);
		FS->WriteInt(0); 
	}
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ShutDown
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  close client system
//
//////////////////////////////////////////////////////////////////////////
void WePackage::ClosePackage()
{
	if (DataStream.IsOpened)
	{
	  DataStream.Close();
	  delete[] IndexTable;
	  IndexTable = NULL;
	  PackageOpened = "";
	}
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  PrepareForReading.
//         Input Parametars :  name
//         Output Parametars:  N/A
//         Comments         :  prepares data stream for reading. name - "package.group.name"
//
//////////////////////////////////////////////////////////////////////////
bool WePackage::BeginReading(StringParam name, int TypeID)
{
	//WeLoger::DebugLog("BeginReading %s @ %.3f\n", name.GetString(), WeTimer::Instance()->GetAbsoluteTime());

	FILE *f;
	String PackageName, Group, Name;
	String tmp = name;
	PackageName = tmp.Extract('.', 0) + PackageExtension;
	Group = tmp.Extract('.', 1);
	Name = tmp.Extract('.', 2);

	// check to see if file exists		
	f = fopen(PackageName, "rb");
	if (f != NULL)
	{
		fclose(f);
		// if file exist then open it
		OpenPackage(PackageName);		

		// find in index this group and name
		for (int i = 0; i < ItemsCount; i++)
		{
			if ((IndexTable[i].Group == Group) && (IndexTable[i].Name == Name) && ((TypeID == 0) || (IndexTable[i].TypeID == TypeID)))
			{
				DataStream.Seek(IndexTable[i].Offset);
				return true;
			}
		}				
		return false;
	}
	else	
	{
		return false;
	}
}

bool WePackage::EndReading()
{
	//WeLoger::DebugLog("EndReading %.3f\n", WeTimer::Instance()->GetAbsoluteTime());

	return true;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  PrepareForWriting
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Adds new item to package. If item exists it will not be added unless ReplaceIfExist
//
//////////////////////////////////////////////////////////////////////////
void WePackage::WriteIndex(int ID, WePackageIndex &Index, WeFileStream *FS)
{
	long tmp = FS->Tell();
	
	//int tmpSize = (sizeof(WePackageIndex)*ID);
	//if(ID > 0) tmpSize -= (2 * ID);
	int IndexSize = sizeof(Index.Group) + sizeof(Index.Name) +
		sizeof(Index.TypeID) + sizeof(Index.Offset) + sizeof(Index.Size);

	FS->Seek(IndexTableOffset + ID * IndexSize);
	FS->WriteStringName(Index.Group);
	FS->WriteStringName(Index.Name);
	//printf("PACKAGE: WI: %s\n" ,Index.Name.GetString());
	FS->WriteInt(Index.TypeID);
	FS->WriteInt(Index.Offset);		
	FS->WriteInt(Index.Size);		
	FS->Seek(tmp); // just to return to position where we where
}




void WePackage::WriteItemsCount(WeFileStream *FS)
{
	long tmp = FS->Tell();
	FS->Seek(0);
	FS->WriteInt(ItemsCount);
	FS->WriteInt(MaxItemsCount);
	FS->Seek(tmp); // just to return to position where we where
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  PrepareForWriting
//         Input Parametars :  N/A
//         Output Parametars:  N/A
//         Comments         :  Adds new item to package. If item exists it will not be added unless ReplaceIfExist
//
//////////////////////////////////////////////////////////////////////////
bool WePackage::BeginWriting(StringParam name, int TypeID, bool ReplaceIfExist)
{
	//if (ReplaceIfExist)
	//{
	//	Remove(name);
	//}

	String tmp, PackageName, Group, Name;
	tmp = name;
	PackageName = tmp.Extract('.', 0) + PackageExtension;
	Group = tmp.Extract('.', 1);
	Name = tmp.Extract('.', 2);
	FILE *f;

	// check to see if file exists
	f = fopen(PackageName, "rb");
	if (f != NULL)
	{
		// if file exist then open it
		fclose(f);

		OpenPackage(PackageName);		
		

		WritingItemIndex = ItemsCount;

		// find in index this group and name
		
		for (int i = 0; i < ItemsCount; i++)
		{
			if ((IndexTable[i].Group == Group) && (IndexTable[i].Name == Name) && (IndexTable[i].TypeID == TypeID))
			{
				if (!ReplaceIfExist)
				{
					return false;
				}
				else
				{
					WritingItemIndex = i;
					// now we found that item should be replaced
					//ReplaceItem(PackageName, i);
					break;
					//return true;
				}
			}
		}

		// new item adding at end of file
		SeekToEnd();
		Index.Group = Group;
		Index.Name = Name;
		Index.TypeID = TypeID;
		Index.Offset = DataStream.Tell();

		StartWritePosition = DataStream.Tell();
		Writing = true;
		return true;

	}
	else	
	{
		// file doesnt exists so make one
		if (!DataStream.OpenNew(PackageName)) return false;

		// write Items Count and MaxItems
		ItemsCount = 0;
		MaxItemsCount = PACKAGE_MAXITEMS;
		PackageOpened = PackageName;

		DataStream.WriteInt(ItemsCount);
		DataStream.WriteInt(MaxItemsCount);
		IndexTableOffset = DataStream.Tell();
		WriteIndexTable(&DataStream, true);

		Index.Group = Group;
		Index.Name = Name;
		Index.TypeID = TypeID;
		Index.Offset = DataStream.Tell();

		StartWritePosition = DataStream.Tell();
		Writing = true;
		return true;
	}
}

bool WePackage::EndWriting()
{
	if (!Writing) return false;

	Index.Size = DataStream.Tell() - StartWritePosition;
	IndexTable[WritingItemIndex] = Index;
	WriteIndex(WritingItemIndex, Index, &DataStream);
	if (WritingItemIndex == ItemsCount)
	{
		ItemsCount++;
	}
	WriteItemsCount(&DataStream);

	//char tmp[100];
	//sprintf(tmp, "size: %d", Index.Size);
	//MessageBox(0, tmp, 0, 0);

			
	return true;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ReplaceItem
//         Input Parametars :  index
//         Output Parametars:  N/A
//         Comments         :  This will replace item in package. It is slow operation so be careful
//
//////////////////////////////////////////////////////////////////////////
void WePackage::ReplaceItem(StringParam PackageName, int index)
{
// make tmp file and copy there all indexes except ExistsIndex
	WeFileStream FS;

	FS.OpenNew(PackageName + ".tmp");
	WriteItemsCount(&FS);
	
	// first write hole empty table
	WriteIndexTable(&FS);
	// now write index table but without element found
	void *data;
	int   size;
	int	  newoff;
	int	  newindex = 0;

	for (int i = 0; i < ItemsCount; i++)
	{
		if (i != index) 
		{
			// if this is not exluding item
			printf("PACKAGE: writing: %s.%s\n", IndexTable[i].Group.GetString(), IndexTable[i].Name.GetString());
			size = ReadChunk(i, data);
			newoff = WriteChunk(data, size, &FS);
			IndexTable[i].Offset = newoff;
			WriteIndex(newindex, IndexTable[i], &FS);
			newindex++;
		}
	};	
	ItemsCount--;
	WriteItemsCount(&FS);

	FS.Close();
	DataStream.Close();

	if (!DeleteFile(PackageName)) return;

	if (!MoveFile(PackageName + ".tmp", PackageName)) return;

	// we got new package file just open it
	PackageOpened = ""; // just to be sure that package will be reloaded
	OpenPackage(PackageName);
}




/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  Remove
//         Input Parametars :  name
//         Output Parametars:  N/A
//         Comments         :  This will remove package item. and return true if successful
//
//////////////////////////////////////////////////////////////////////////
bool WePackage::Remove(StringParam name, int TypeID)
{
	String tmp, PackageName, Group, Name;
	FILE *f;
	tmp = name;
	PackageName = tmp.Extract('.', 0) + PackageExtension;
	Group = tmp.Extract('.', 1);
	Name = tmp.Extract('.', 2);

	// check to see if file exists
	f = fopen(PackageName, "rb");
	if (f == NULL) return false;
	fclose(f);

	// now find if this item exists
	OpenPackage(PackageName);

	// find in index this group and name
	bool Exists = false;
	int ExistsIndex = -1;

	for (int i = 0; i < ItemsCount; i++)
	{
		if ((IndexTable[i].Group == Group) && (IndexTable[i].Name == Name) && (IndexTable[i].TypeID == TypeID))
		{
			Exists = true;
			ExistsIndex = i;
			break;
		}
	}

	if (!Exists)
	{
		return false;
	}

	if (ItemsCount > 1)
	{
		Index = IndexTable[ItemsCount - 1];
		WriteIndex(ExistsIndex, Index, &DataStream);
	}

	--ItemsCount;
	WriteItemsCount(&DataStream);

	ClosePackage();

	return true;
}


/*
bool WePackage::Remove(StringParam name)
{
	String tmp, PackageName, Group, Name;
	FILE *f;
	tmp = name;
	PackageName = tmp.Extract('.', 0) + PackageExtension;
	Group = tmp.Extract('.', 1);
	Name = tmp.Extract('.', 2);

	// check to see if file exists
	f = fopen(PackageName, "rb");
	if (f == NULL) return false;
	fclose(f);

	// now find if this item exists
	OpenPackage(PackageName);

	// find in index this group and name
	bool Exists = false;
	int ExistsIndex = -1;

	for (int i = 0; i < ItemsCount; i++)
	{
		if (IndexTable[i].Group == Group && IndexTable[i].Name == Name)
		{
			Exists = true;
			ExistsIndex = i;
		}
	}

	if (!Exists)
	{
		return false;
	}

	// make tmp file and copy there all indexes except ExistsIndex
	WeFileStream FS;

	FS.OpenNew(PackageName + ".tmp");
	WriteItemsCount(&FS);

	// first write hole mpty table
	WriteIndexTable(&FS);
	// now write index table but without element found
	void *data;
	int   size;
	int	  newoff;
	int	  newindex = 0;

	for (int i = 0; i < ItemsCount; i++)
	{
		//WeLoger::DebugLog("Copying %d/%d\n", i, ItemsCount);
		if (i != ExistsIndex) 
		{
			// if this is not exluding item
			size = ReadChunk(i, data);
			newoff = WriteChunk(data, size, &FS);
			IndexTable[i].Offset = newoff;
			WriteIndex(newindex, IndexTable[i], &FS);
			newindex++;
		}
	}
	ItemsCount--;
	WriteItemsCount(&FS);

	FS.Close();
	DataStream.Close();

	if (!DeleteFile(PackageName))
	{
		return false;
	}

	if (!MoveFile(PackageName + ".tmp", PackageName))
	{
		return false;
	}

	// we got new package file just open it
	PackageOpened = ""; // just to be sure that package will be reloaded
	OpenPackage(PackageName);		

	return true;
}
*/


bool WePackage::CompactPackage(StringParam name)
{
	String tmp, PackageName, Group, Name;
	FILE *f;
	tmp = name;
	PackageName = tmp.Extract('.', 0) + PackageExtension;
	Group = tmp.Extract('.', 1);
	Name = tmp.Extract('.', 2);

	// check to see if file exists
	f = fopen(PackageName, "rb");
	if (f == NULL)
	{
		return false;
	}
	fclose(f);

	OpenPackage(PackageName);

	// make tmp file and copy there all indexes
	WeFileStream FS;

	FS.OpenNew(PackageName + ".tmp");
	WriteItemsCount(&FS);

	// first write hole empty table
	WriteIndexTable(&FS);

	// now write index table
	void *data;
	int   size;
	int	  newoff;

	for (int i = 0; i < ItemsCount; i++)
	{
		size = ReadChunk(i, data);
		newoff = WriteChunk(data, size, &FS);
		IndexTable[i].Offset = newoff;
		WriteIndex(i, IndexTable[i], &FS);
	}
	WriteItemsCount(&FS);

	FS.Close();
	ClosePackage();

	if (!DeleteFile(PackageName))
	{
		return false;
	}

	if (!MoveFile(PackageName + ".tmp", PackageName))
	{
		return false;
	}

	// we got new package file just open it
	//PackageOpened = ""; // just to be sure that package will be reloaded
	//OpenPackage(PackageName);		

	return true;
}



/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  ReadChunk
//         Input Parametars :  name
//         Output Parametars:  N/A
//         Comments         :  This will read chunk of data and return size of it
//
//////////////////////////////////////////////////////////////////////////
int WePackage::ReadChunk(int index, void *&lpdata)
{
	long temp = DataStream.Tell();
	DataStream.Seek(IndexTable[index].Offset);

	lpdata = malloc(IndexTable[index].Size);  
	DataStream.ReadData(lpdata, IndexTable[index].Size);
	DataStream.Seek(temp);		
	return IndexTable[index].Size;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  WriteChunk
//         Input Parametars :  name
//         Output Parametars:  N/A
//         Comments         :  This writes chunk of data to the end of file. Returns offset where it started writing
//
//////////////////////////////////////////////////////////////////////////
int WePackage::WriteChunk(void *data, int size, WeFileStream *FS)
{
	long temp = FS->Tell();
	long ret;

	FS->Seek(0, SEEK_END);
	ret = FS->Tell();

	FS->WriteData(data, size);

	FS->Seek(temp);
	return ret;
}


/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  GetItemsNames
//         Input Parametars :  name
//         Output Parametars:  N/A
//         Comments         :  This writes chunk of data to the end of file. Returns offset where it started writing
//
//////////////////////////////////////////////////////////////////////////
Strings *WePackage::GetItemsNames(StringParam package, int Type)
{
	String Package = package + PackageExtension;

	OpenPackage(Package);
	Names.Clear();

	for (int i = 0; i < ItemsCount; i++)
	{
		if (IndexTable[i].TypeID == Type)
		{
			Names.PushBack(IndexTable[i].Name);
		}
	}
	return &Names;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  GetItemsNames
//         Input Parametars :  name
//         Output Parametars:  N/A
//         Comments         :  This writes chunk of data to the end of file. Returns offset where it started writing
//
//////////////////////////////////////////////////////////////////////////
Strings *WePackage::GetItemsNames(StringParam package, StringParam Group, int Type)
{
	String Package = package + PackageExtension;
	OpenPackage(Package);
	Names.Clear();

	for (int i = 0; i < ItemsCount; i++)
	{
		if ((String)(Group) == IndexTable[i].Group)
		{
			if (IndexTable[i].TypeID == Type)
			{
				Names.PushBack(IndexTable[i].Name);
			}
		}
	}
	return &Names;
}

/////////////////////////////////////////////////////////////////////////
//    
//         Function Name    :  GetItemsNames
//         Input Parametars :  name
//         Output Parametars:  N/A
//         Comments         :  This writes chunk of data to the end of file. Returns offset where it started writing
//
//////////////////////////////////////////////////////////////////////////
Strings *WePackage::GetGroupNames(StringParam package, int Type)
{

	String Package = package + PackageExtension;
	Groups.Clear();

	FILE *f;
	f = fopen(Package, "rb");
	if (!f) return &Groups;
	fclose(f);  
	
	// if file exist then open it
	OpenPackage(Package);		


	for (int i = 0; i < ItemsCount; i++)
	{
		if (IndexTable[i].TypeID == Type)
			Groups.PushBackUnique(IndexTable[i].Group);
	}

	return &Groups;
}
