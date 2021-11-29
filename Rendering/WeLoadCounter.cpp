#include <WeLoadCounter.h>
#include <WeRenderDevice.h>
#include <WeTimer.h>
#include <WeLoger.h>


String LoadingName = (String)"";
float LoadingStartTime;



void SetLoadingName(StringParam name)
{
	if (name.GetLen() != 0)
	{
		LoadingName = name;
		LoadingStartTime = WeTimer::Instance()->GetAbsoluteTime();
#ifdef _DEBUG
		//WeLoger::DebugLog("LOADING %s\n", LoadingName.GetString());
#endif
	}
}

void ResetLoadingName()
{
	if (LoadingName.GetLen() != 0)
	{
		float t;
		t = WeTimer::Instance()->GetAbsoluteTime() - LoadingStartTime;
#ifdef _DEBUG
		WeLoger::DebugLog("LOADED [%6.3fs] %s\n", t, LoadingName.GetString());
#endif
		LoadingName = "";
	}
}

String GetLoadingName()
{
	return LoadingName;
}
