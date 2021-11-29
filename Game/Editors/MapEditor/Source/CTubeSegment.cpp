/*
    CTubeSegment.cpp

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





*/

#include <CTubeSegment.h>
#include <stdio.h>


CTubeSegment *CTubeSegment::_instance = NULL;
CTubeSegment *CTubeSegment::Instance()
{
	if (_instance) return _instance;
	_instance = new CTubeSegment;
	return _instance;
}

void CTubeSegment::LoadFromTxt(StringParam FileName)
{
	CTubeSegmentInfo SegmentInfo;
	FILE *f = fopen(FileName, "rt");
	if (!f) return;

	SegmentInfos.Clear();

	char tmp[200];

	while (!feof(f))
	{

		
		fgets(tmp,200, f);
		String tmps;
		tmps = tmp;
		tmps.RemoveChar(0);
		tmps.RemoveChar(tmps.GetLen()-1);
		tmps.RemoveChar(tmps.GetLen()-1);
		SegmentInfo.Name = tmps;


		fscanf(f, "%d\n", &SegmentInfo.NumPivots);


		for (int i = 0; i < SegmentInfo.NumPivots; i++)
		{
			if (SegmentInfo.NumPivots == 1)
			{
			fscanf(f, "%f\t%f\t%f\t%f\t%f\t%f\n", 
				&SegmentInfo.Pivots[i].x, &SegmentInfo.Pivots[i].z, &SegmentInfo.Pivots[i].y,
				&SegmentInfo.Orientations[i].z, &SegmentInfo.Orientations[i].y, &SegmentInfo.Orientations[i].x);
			}
			if (SegmentInfo.NumPivots > 1)
			{
			fgets(tmp, 200, f);
			fscanf(f, "%f\t%f\t%f\t%f\t%f\t%f\n", 
				&SegmentInfo.Pivots[i].x, &SegmentInfo.Pivots[i].z, &SegmentInfo.Pivots[i].y,
				&SegmentInfo.Orientations[i].z, &SegmentInfo.Orientations[i].y, &SegmentInfo.Orientations[i].x);
			
			SegmentInfo.PivotNames[i] = tmp;
			SegmentInfo.PivotNames[i].RemoveChar(SegmentInfo.PivotNames[i].GetLen()-1);


			}

			SegmentInfo.Orientations[i].x = SegmentInfo.Orientations[i].x*D3DX_PI/180.0f;
			SegmentInfo.Orientations[i].y = SegmentInfo.Orientations[i].y*D3DX_PI/180.0f;
			SegmentInfo.Orientations[i].z = SegmentInfo.Orientations[i].z*D3DX_PI/180.0f;

		}
		// Transfer orient to radians

		SegmentInfos.PushBack(SegmentInfo);
	}
	fclose(f);
}


CTubeSegmentInfo *CTubeSegment::GetSegment(StringParam Name)
{
	for (SegmentInfos.First(); !SegmentInfos.End(); SegmentInfos.Next())
	{
		//MessageBox(0, SegmentInfos.GetItem().Name, Name, 0);
		if (SegmentInfos.GetItem().Name == Name)
		{
			return &SegmentInfos.GetItem();
		}
	}
	return NULL;
}

void CTubeSegment::PrintToFile(StringParam FileName)
{
	FILE *f;
	f = fopen(FileName, "wt+");
	for (SegmentInfos.First(); !SegmentInfos.End(); SegmentInfos.Next())
	{
		fprintf(f, "[%s]\n", SegmentInfos.GetItem().Name);
		for (int i = 0; i < SegmentInfos.GetItem().NumPivots; i++)
		{
			if (SegmentInfos.GetItem().NumPivots == 1)
			{
			fprintf(f, "%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\n", 
				SegmentInfos.GetItem().Pivots[i].x,SegmentInfos.GetItem().Pivots[i].y,SegmentInfos.GetItem().Pivots[i].z,
				SegmentInfos.GetItem().Orientations[i].x, SegmentInfos.GetItem().Orientations[i].y, SegmentInfos.GetItem().Orientations[i].z);
			} 

			if (SegmentInfos.GetItem().NumPivots > 1)
			{
			fprintf(f, "%s%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\t%3.2f\n", 
				SegmentInfos.GetItem().PivotNames[i].GetString(),
				SegmentInfos.GetItem().Pivots[i].x,SegmentInfos.GetItem().Pivots[i].y,SegmentInfos.GetItem().Pivots[i].z,
				SegmentInfos.GetItem().Orientations[i].x, SegmentInfos.GetItem().Orientations[i].y, SegmentInfos.GetItem().Orientations[i].z);
			} 

		}
	}
	fclose(f);
}

