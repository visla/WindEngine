/*
    WeQuery.h

	Autor: Nebojsa Vislavski
	Date: 28.10.2005.
	Version: 1.0
	Platform: Win32


	Serbian: Ovo je query koji sluzi za tipican culling jednostavnih objekata

*/

#ifndef _WEQUERY_H
#define _WEQUERY_H

#include <d3dx9.h>

class WeQuery
{
protected:

public:
	~WeQuery() { ShutDown(); }

	LPDIRECT3DQUERY9 lpQuery;

	bool Initialize();
	void ShutDown();

	void Begin() { lpQuery->Issue(D3DISSUE_BEGIN); }
	void End() { lpQuery->Issue(D3DISSUE_END); }

	DWORD GetPixelsDrawn() {
		DWORD tmp = 0;
		while(S_OK != lpQuery->GetData( &tmp, 
                                  sizeof(DWORD), D3DGETDATA_FLUSH ))
		{
		}
		return tmp;
	}


	
};




#endif