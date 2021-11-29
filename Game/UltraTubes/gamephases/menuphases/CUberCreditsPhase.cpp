/*
	Filename:	CUberCreditsPhase.cpp
	Author:		Goran Bogdanovic
*/

#include <CUberCreditsPhase.h>
#include <CGame.h>
#include <CMenuPhase.h>
#include <CMainMenuPhase.h>
#include <CUberText.h>
#include <WeTextureManager.h>
#include <WeTimer.h>
#include <WeCanvas.h>


CUberCreditsPhase *CUberCreditsPhase::Instance = new CUberCreditsPhase;
float CUberCreditsDude::WOffset = (float)WeRenderDevice::Instance()->GetResWidth() / 1024.0f;
float CUberCreditsDude::HOffset = (float)WeRenderDevice::Instance()->GetResHeight() / 768.0f;


//---CUberCreditsDude---

CUberCreditsDude::CUberCreditsDude(String DudesName, String DudesInfo, String Portrait, float Duration)
{
	ChosenPreset = (rand() % 4) + 1;
	float PartDuration;

	NameText = DudesName;
	InfoText = DudesInfo;
	TotalDuration = Duration;

	PartDuration = TotalDuration;
	Name = new CUberText(
		DudesName, 
		GetCoord(DudeCoordinateX, DudeElementsName, ChosenPreset), 
		GetCoord(DudeCoordinateY, DudeElementsName, ChosenPreset) );
	Name->SetTimeShow(PartDuration / 4.0f);
	Name->SetTimeStay(PartDuration / 2.0f);
	Name->SetTimeHide(PartDuration / 4.0f);
	Name->SetFont("Xirod", (int)(20.0f * HOffset));

	PartDuration = TotalDuration - Name->GetTimeShow();
	Info = new CUberText(
		InfoText, 
		GetCoord(DudeCoordinateX, DudeElementsInfo, ChosenPreset), 
		GetCoord(DudeCoordinateY, DudeElementsInfo, ChosenPreset), 
		UberTextTypeAnimLetterShowFO, 
		UberTextTypeAnimFade);
	Info->SetTimeShow(PartDuration / 4.0f);
	Info->SetTimeStay(PartDuration / 2.0f);
	Info->SetTimeHide(PartDuration / 4.0f);
	Info->SetFont("Xirod", (int)(16.0f * HOffset));

	char PortraitName[100];
	sprintf(PortraitName, "Ingame.Credits.%s", Portrait);
	Picture = WeTextureManager::Instance()->Load(String(PortraitName), true);
	PictureTimeShow = PartDuration / 4.0f;
	PictureTimeStay = PartDuration / 2.0f;
	PictureTimeHide = PartDuration / 4.0f;

	CurrentStage = DudeStageName;
	Completed = false;
	Timer = 0.0f;
	TimerPicture = 0.0f;
	PictureAlpha = 0.0f;
}

CUberCreditsDude::~CUberCreditsDude()
{
	SAFE_DELETE(Name);
	SAFE_DELETE(Info);
	WeTextureManager::Instance()->Free(Picture);
	Picture = NULL;
}

void CUberCreditsDude::Tick()
{
	if (Completed) return;

	float time = WeTimer::Instance()->GetDeltaTimeF();
	Timer += time;

	switch (CurrentStage)
	{
	case DudeStageName:
		if (Timer >= Name->GetTimeShow())
			CurrentStage = DudeStageInfo;

		if (Name) Name->Tick();
		break;

	case DudeStageInfo:
		if (Timer >= Name->GetTimeShow() + Info->GetTimeShow())
			CurrentStage = DudeStagePortrait;

		if (Name) Name->Tick();
		if (Info) Info->Tick();
		break;

	case DudeStagePortrait:
		if (Picture)
		{
			TimerPicture += time;
			if (TimerPicture <= PictureTimeShow)
				PictureAlpha = TimerPicture / PictureTimeShow;
			else if (TimerPicture <= PictureTimeShow + PictureTimeStay)
				PictureAlpha = 1.0f;
			else if (TimerPicture <= PictureTimeShow + PictureTimeStay + PictureTimeHide)
				PictureAlpha = 1.0f - ((TimerPicture - (PictureTimeShow + PictureTimeStay)) / PictureTimeHide);
			else
				PictureAlpha = 0.0f;
		}


		if (Name) Name->Tick();
		if (Info) Info->Tick();
		break;
	}

	if (Timer >= TotalDuration)
		Completed = true;
}

void CUberCreditsDude::Render()
{
	switch (CurrentStage)
	{
	case DudeStageName:
		if (Name) Name->Render();
		break;

	case DudeStageInfo:
		if (Name) Name->Render();
		if (Info) Info->Render();
		break;

	case DudeStagePortrait:
		if (Picture)
		{
			WeCanvas::Instance()->DrawRectangle(
				(int)GetCoord(DudeCoordinateX, DudeElementsPicture, ChosenPreset), 
				(int)GetCoord(DudeCoordinateY, DudeElementsPicture, ChosenPreset), 
				(int)((float)Picture->Width * WOffset), 
				(int)((float)Picture->Height * HOffset),
				WeColor(1.0f, 1.0f, 1.0f, PictureAlpha),
				Picture);
		}

		if (Name) Name->Render();
		if (Info) Info->Render();
		break;

	default:
		break;
	}
}

float CUberCreditsDude::GetCoord(DudeCoordinate Coord, DudeElements Elem, int Preset)
{
	switch (Elem)
	{
	case DudeElementsName:
		switch (Preset)
		{
		case 1:
			return Coord == DudeCoordinateX ? 0.1f : 0.1f;
		case 2:
			return Coord == DudeCoordinateX ? 0.1f : 0.5f;
		case 3:
			return Coord == DudeCoordinateX ? 0.5f : 0.1f;
		case 4:
			return Coord == DudeCoordinateX ? 0.5f : 0.5f;
		}
		break;

	case DudeElementsInfo:
		switch (Preset)
		{
		case 1:
			return Coord == DudeCoordinateX ? 0.1f : 0.2f;
		case 2:
			return Coord == DudeCoordinateX ? 0.6f : 0.5f;
		case 3:
			return Coord == DudeCoordinateX ? 0.5f : 0.5f;
		case 4:
			return Coord == DudeCoordinateX ? 0.5f : 0.6f;
		}
		break;

	case DudeElementsPicture:
		switch (Preset)
		{
		case 1:
			return Coord == DudeCoordinateX ? 0.6f : 0.1f;
		case 2:
			return Coord == DudeCoordinateX ? 0.1f : 0.1f;
		case 3:
			return Coord == DudeCoordinateX ? 0.1f : 0.1f;
		case 4:
			return Coord == DudeCoordinateX ? 0.1f : 0.5f;
		}
		break;
	}

	return 0.0f;
}

float CUberCreditsDude::GetTimer()
{
	return Timer;
}

bool CUberCreditsDude::IsComplete()
{
	return Completed;
}



//---CUberCreditsPhase---

CUberCreditsPhase::CUberCreditsPhase()
{
}


CUberCreditsPhase::~CUberCreditsPhase()
{
}


//void CUberCreditsPhase::Start()
//{
//	int i = 0;
//
//	DudeNames[i] = "Tomislav Kukic";
//	DudeInfos[i] = """Teh Coder""\nof teh best GUI in\nteh world!";
//	DudePorts[i] = "";
//	i++;
//
//	DudeNames[i] = "Mirko Topalski";
//	DudeInfos[i] = """Teh D00de""\nthat pwns teh drumz";
//	DudePorts[i] = "";
//	i++;
//
//	DudeNames[i] = "Nebojsa Vislavski";
//	DudeInfos[i] = """Teh Code D00der""\nthat pwns juvenile girliez\n;)";
//	DudePorts[i] = "";
//	i++;
//
//	DudeNames[i] = "Tomislav Kukic";
//	DudeInfos[i] = """Teh Coder""\nthat pwns in\nSCORE technology!";
//	DudePorts[i] = "";
//	i++;
//
//	DudeNames[i] = "Goran Bogdanovic";
//	DudeInfos[i] = """Teh Game Coder""\nthat pwns teh hardest\nof teh all pwnerers\nergo: teh masterer =)";
//	DudePorts[i] = "";
//	i++;
//
//	DudeNames[i] = "Aleksa Todorovic";
//	DudeInfos[i] = """Teh Super Coder""\nd00de that is married\nto Nenad :P";
//	DudePorts[i] = "";
//	i++;
//
//	DudeNames[i] = "Vuk Vranjkovic";
//	DudeInfos[i] = """Teh Added Coder""\nold dude of teh\nold crew of dudez";
//	DudePorts[i] = "";
//	i++;
//
//	DudeNames[i] = "Tomislav Kukic";
//	DudeInfos[i] = """Teh Coder""\nthat simply pwns!!!\nnothing else, just PWNS!";
//	DudePorts[i] = "";
//	i++;
//
//	DudeNames[i] = "Nenad Tomic";
//	DudeInfos[i] = """Teh UBER Grapher""\nLegalize it\ndont criticize it\nand I will advertize it!";
//	DudePorts[i] = "";
//	i++;
//
//	DudeNames[i] = "Uros Banjesevic";
//	DudeInfos[i] = """Teh Grapher""\nteh young grapher\nfrom teh crazy city\nof teh DisneyWorld\nand teh bes c00ker of teh\nsoups =)";
//	DudePorts[i] = "";
//	i++;
//
//	DudeNames[i] = "Sandor Goli";
//	DudeInfos[i] = """Teh Grapher""\nthis d00de haz\nteh best pron\nin teh world :)";
//	DudePorts[i] = "";
//	i++;
//
//	DudeNames[i] = "Tomislav Kukic";
//	DudeInfos[i] = """Teh Coder""\nthat you may just wish havin\nin your winning team\nTEH OMGOMGOMG TEH BEST! =)";
//	DudePorts[i] = "";
//	i++;
//
//	DudesCount = i;
//	CurrentDudeIndex = -1;
//	CurrentDude = NULL;
//}


void CUberCreditsPhase::Start()
{
	int i = 0;

	DudeNames[i] = "Mirko Topalski";
	DudeInfos[i] = "Original Idea\nCEO\nProducer\nMusic & Sound";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Nebojsa Vislavski";
	DudeInfos[i] = "Original Idea\nLead Programmer\nExecutive Producer";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Goran Bogdanovic";
	DudeInfos[i] = "Senior Programmer";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Aleksa Todorovic";
	DudeInfos[i] = "Senior Programmer";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Tomislav Kukic";
	DudeInfos[i] = "Junior Programmer";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Vuk Vranjkovic";
	DudeInfos[i] = "Additional Programming";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Ivan Zorkic";
	DudeInfos[i] = "Concept Art";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Vladimir Susic";
	DudeInfos[i] = "Concept Art";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Uros Banjesevic";
	DudeInfos[i] = "Lead Artist & Story";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Sandor Goli";
	DudeInfos[i] = "Art & Weapons";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Nenad Tomic";
	DudeInfos[i] = "Art & Level Design";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Additional Art:";
	DudeInfos[i] = "Aleksandar Rodic\nBojan Vukic";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Additional Art:";
	DudeInfos[i] = "Bratislav Popovic\nVojislav Pavkov";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Dusan Jovanovic";
	DudeInfos[i] = "Web Support";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Milan Rancic";
	DudeInfos[i] = "Balance";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Special Thanx:";
	DudeInfos[i] = "Klan RUR";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "Special Thanx:";
	DudeInfos[i] = "Danko Vukovic\nPredrag Filipovic\nDusan Sijacki";
	DudePorts[i] = "";
	i++;

	DudeNames[i] = "And Very Special Thanx:";
	DudeInfos[i] = "Our families\nAll those who support us,\nart, science and world peace\nand of course the SG1 Team,\nand Matt Stone & Trey Parker";
	DudePorts[i] = "";
	i++;

	DudesCount = i;
	CurrentDudeIndex = -1;
	CurrentDude = NULL;
}


void CUberCreditsPhase::End()
{
}


void CUberCreditsPhase::Tick()
{
	if (CGame::Instance->InputMessage.Exit)
		CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);

	if (!CurrentDude || CurrentDude->IsComplete())
	{
		SAFE_DELETE(CurrentDude);
		CurrentDudeIndex++;

		if (CurrentDudeIndex == DudesCount)
			CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);

		CurrentDude = new CUberCreditsDude(DudeNames[CurrentDudeIndex], DudeInfos[CurrentDudeIndex], DudePorts[CurrentDudeIndex]);
	}

	if (CurrentDude)
		CurrentDude->Tick();
}


void CUberCreditsPhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();

	if (CurrentDude)
		CurrentDude->Render();
}
