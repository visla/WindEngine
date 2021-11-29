/*
    CTubeSegment.h

	Autor: Nebojsa Vislavski
	Date: 21.12.2005.
	Version: 1.0


	This is segment for tube. It has loaded some text file
	Txt file format:
	 Name
	 Pivotx pivoty pivotz yaw pitch roll
	 Pivotx pivoty pivotz yaw pitch roll
	 Pivotx pivoty pivotz yaw pitch roll
	 ...
	 Name
	 Pivotx pivoty pivotz yaw pitch roll


	 Class used to faster adding of tube parts
	 



*/

#ifndef _CTUBESEGMENT_H
#define _CTUBESEGMENT_H


#include <CString.h>
#include <WeVector.h>
#include <SmartList.h>

class CTubeSegmentInfo
{
public:
	String Name;
	int NumPivots;
	WeVector Pivots[10];
	String   PivotNames[10];
	WeVector Orientations[10];

};

class CTubeSegment
{
private:
	static CTubeSegment *_instance;
protected:

public:
	static CTubeSegment *Instance();
	void LoadFromTxt(StringParam FileName);	
	void PrintToFile(StringParam FileName);
	CTubeSegmentInfo *GetSegment(StringParam Name);


	CSmartList <CTubeSegmentInfo> SegmentInfos;

};



#endif