#include <CAbstractPhase.h>
#include <WeProfiler.h>



void CAbstractPhase::Tick()
{ 
	if (NextPhase) 
	{
		if (CurrentPhase)
			CurrentPhase->End();
		PrevPhase = CurrentPhase;
		CurrentPhase = NextPhase;
		NextPhase = NULL;	
		CurrentPhase->Start();
	}
	if (CurrentPhase)
		CurrentPhase->Tick();
}



void CAbstractPhase::Render()
{
	if (CurrentPhase)
		CurrentPhase->Render();
}



void CAbstractPhase::Start()
{
}



void CAbstractPhase::End()
{
	if (CurrentPhase)
	{
		CurrentPhase->End();
		CurrentPhase = NULL;
	}
}
