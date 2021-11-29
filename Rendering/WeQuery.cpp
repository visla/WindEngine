/*
    WeQuery.cpp

    Autor: Nebojsa Vislavski
	Version: 1.0
	Date: 28.10.2005.
	Platform: Win32


*/

#include <WeQuery.h>
#include <WeRenderDevice.h>
#include <stdio.h>


bool WeQuery::Initialize()
{
	if (FAILED(WeRenderDevice::Instance()->Device->CreateQuery(D3DQUERYTYPE_OCCLUSION, &lpQuery)))
	{
		lpQuery = NULL;
		return false;
	}
	return true;
}


void WeQuery::ShutDown()
{
	SAFE_RELEASE(lpQuery);
}