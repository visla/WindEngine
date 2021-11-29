/*
    CStatisticsPhase.cpp

	Autor: Nebojsa Vislavski
	Date: 10.3.2006.
	Version: 1.0
    
*/

#include <CStatisticsPhase.h>
#include <CGame.h>
#include <CGameSettings.h>
#include <CMainMenuPhase.h>
#include <CMenuPhase.h>
#include <WeFont.h>
#include <WeCanvas.h>
#include <CFunnyPhase.h>
#include <CTournamentPhase.h>
#include <CTournamentMapSettings.h>
#include <CGameClient.h>
#include <CProfileManager.h>
#include <CStoryTellerPhase.h>
#include <CProfileManager.h>
#include <Constants.h>
#include <CTuber.h>
#include <CGamePlayerConfig.h>
#include <CGAmeServer.h>
#include <CGameClient.h>
#include <CLANServerBrowsePhase.h>
#include <CInternetServerBrowsePhase.h>
#include <WeTextureManager.h>
#include <WeMaterialManager.h>
#include <WeShaderManager.h>
#include <WeMeshManager.h>
#include <WeModelManager.h>
#include <WePostProcess.h>
#include <CStatistics.h>
#include <WeRenderDevice.h>
#include <WeTimer.h>
#include <WeCEGUI.h>
#include <CGUIHelper.h>
#include <CTuberPartFabric.h>
#include <CMainGameInterface.h>
#include <CLocalizationSettings.h>
#include <DInput.h>
#include <CMainGamePhase.h>



CStatisticsPhase *CStatisticsPhase::Instance = new CStatisticsPhase;


CStatisticsPhase::CStatisticsPhase()
{
	Background = NULL;
}

void CStatisticsPhase::Start()
{
	if (CGameSettings::ClientInstance->MapType != CGameSettings::MAPTYPE_MP_TOURNAMENT)
	{
		CGameServer::Instance->StopHosting();
		CGameServer::Instance->ShutDown();

		CGameClient::Instance->Disconnect();
		CGameClient::Instance->ShutDown();
	}
	CStatistics::Instance->Sort();
	// Ovde cemo da procitamo tabelu statistike i prikazemo na ekranu

	CEGUI::WindowManager &winMgr = CEGUI::WindowManager::getSingleton();

	CSmartList<String> ColumnNames;
	ColumnNames.PushBack("POS");
	ColumnNames.PushBack("PLAYER");
	ColumnNames.PushBack("TUBER");
	ColumnNames.PushBack("TEAM");
	ColumnNames.PushBack("FASTEST LAP");
	ColumnNames.PushBack("SCORE_");
	ColumnNames.PushBack("FRAGS");
	ColumnNames.PushBack("CASH");

	StatisticList = CGUIHelper::CreateMultiList("StatisticList", 0.05f, -2.0f, 0.9f, 0.4f, ColumnNames, true, true, "RootWindow");
	StatisticList->setAlpha(0.0f);


	CGUIHelper::CreateButton("STATISTICS_BACK_BUTTON", CLocalizationSettings::Instance()->GetLocalText("BACK"), 0.7f, 0.8f, 0.3f, 0.12f, "RootWindow", &CStatisticsPhase::OnBackToMenu);

	MyListItem *ml;
	CEGUI::colour c;
	c.setRed(1.0f);
	c.setGreen(1.0f);
	c.setBlue(1.0f);
	c.setAlpha(1.0f);


	String s;

	bool winer = false; 
	for (int i = 0; i < MAX_PLAYERS; i++)
	{			
		if (CStatistics::Instance->Table[i].Valid)
		{
			if (CStatistics::Instance->Table[i].MyProfile && CStatistics::Instance->Table[i].Winner) winer = true;

			char tmp[100];
			int r;

			r = StatisticList->addRow();            

			sprintf(tmp, "%.2d", CStatistics::Instance->Table[i].Position);
			StatisticList->setItem(new MyListItem(tmp, "StatisticList"), 0, r);

			ml = new MyListItem(CStatistics::Instance->Table[i].PlayerName, "StatisticList");
			ml->setTextColours(c);
			StatisticList->setItem(ml, 1, r);


			ml = new MyListItem(CStatistics::Instance->Table[i].TuberName, "StatisticList");
			StatisticList->setItem(ml, 2, r);

			sprintf(tmp, "%d", CStatistics::Instance->Table[i].TeamNumber);
			StatisticList->setItem(new MyListItem(tmp, "StatisticList"), 3, r);

			String s;
			s = CMainGameInterface::Instance->MakeTimeString(CStatistics::Instance->Table[i].FastestLap);

			sprintf(tmp, "%s", s.GetString());
			StatisticList->setItem(new MyListItem(tmp, "StatisticList"), 4, r);

			sprintf(tmp, "%d", CStatistics::Instance->Table[i].Score);
			StatisticList->setItem(new MyListItem(tmp, "StatisticList"), 5, r);

			sprintf(tmp, "%d", CStatistics::Instance->Table[i].Kills);
			StatisticList->setItem(new MyListItem(tmp, "StatisticList"), 6, r);

			sprintf(tmp, "%d", CStatistics::Instance->Table[i].Cash);
			StatisticList->setItem(new MyListItem(tmp, "StatisticList"), 7, r);
		}
	}

	StatisticList->setSortColumnByID(0);
	StatisticList->setProperty("SortDirection", "Descending");

	if (winer)
		Background = WeTextureManager::Instance()->Load("Textures.Statistics.YouWin", true); 
	else
		Background = WeTextureManager::Instance()->Load("Textures.Statistics.YouLose", true); 

	Ypos = -2.0f;
	Alpha = 0.0f;

	CMainGamePhase::Instance->PlaySong(SongEndRace);
}


bool CStatisticsPhase::OnBackToMenu(const CEGUI::EventArgs &e)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	CStatisticsPhase::Instance->OnReturn();    
	return true;
}


void CStatisticsPhase::End()
{
	// pogasimo sve ono sto nam ne treba i necemo gasiti samo ako je MP_Turnir

	WePostProcess::Instance()->ClearEffects();

	WeModelManager::Instance()->FreeAll();
	WeMeshManager::Instance()->FreeAll();
	WeMaterialManager::Instance()->FreeAll();		
	WeTextureManager::Instance()->FreeAll();

	CGameSettings::ClientInstance->ClearPlayers();
	CGameSettings::ServerInstance->ClearPlayers();
	CTuberPartFabric::Instance()->ShutDown();


	Background = NULL;
	CEGUI::WindowManager::getSingleton().getWindow("StatisticList")->destroy();
	CEGUI::WindowManager::getSingleton().getWindow("STATISTICS_BACK_BUTTON")->destroy();
}


void CStatisticsPhase::OnReturn()
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	if (CGameSettings::ClientInstance->Funny)
	{
		CGame::Instance->SetPhase(CFunnyPhase::Instance);
	}
	else
	{
		if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_SINGLERACE ||
			CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_TIMETRIAL ||
			CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_LASTONE)
		{
			CGame::Instance->SetPhase(CMenuPhase::Instance);
			CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);
		}

		if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_SP_TOURNAMENT)
		{
			// ako je markiran kraj igre
			if (CGameSettings::ClientInstance->MapSettings->MarksEndOfGame)
			{
				// idi u StoryTeller phase ili kraj igre fazu
				CGame::Instance->SetPhase(CStoryTellerPhase::Instance);					
			}
			else
			{
				CGame::Instance->SetPhase(CMenuPhase::Instance);
				CMenuPhase::Instance->SetPhase(CTournamentPhase::Instance);
			}
		}

		if (CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_MP_LASTONE || 
			CGameSettings::ClientInstance->MapType == CGameSettings::MAPTYPE_MP_SINGLERACE)
		{
			if (CGame::Instance->ServerInfo.LAN)
			{
				CGame::Instance->SetPhase(CMenuPhase::Instance);
				CMenuPhase::Instance->SetPhase(CLANServerBrowsePhase::Instance);					
			}
			else
			{
				CGame::Instance->SetPhase(CMenuPhase::Instance);
				CMenuPhase::Instance->SetPhase(CInternetServerBrowsePhase::Instance);					
			}
		}				

		// TODO: Dodati ovde ako je MP Turnir
	}
}


void CStatisticsPhase::Tick()
{
	StatisticList->setWindowPosition(CGUIHelper::UVector2(0.05f, Ypos));
	StatisticList->setAlpha(Alpha);
	
	Ypos += WeTimer::Instance()->GetDeltaTimeF()*1.2f;
	Alpha += WeTimer::Instance()->GetDeltaTimeF()*0.2f;

	if (Alpha > 0.8f) Alpha = 0.8f;
	if (Ypos > 0.1f) Ypos = 0.1f;	

	if (CGame::Instance->CGame::Instance->InputMessage.Exit)
	{
		OnReturn();
	}
}




void CStatisticsPhase::Render()
{
		/*static WeFont *Font = NULL;
		static char StatisticsText[1500];
		static bool TextIsSet = false;
	
		if (!Font) Font = WeCanvas::Instance()->MakeFont("Arial", 24);

		sprintf(StatisticsText, "Player         Tuber          Position        FastestLap          TeamNumber");
		WeCanvas::Instance()->Draw2DText(Font, StatisticsText, 10, 10, 1600, 1600, 0xffffffff);

		String s;
		for (int i = 0; i < MAX_PLAYERS; i++)
		{			
			if (CStatistics::Instance->Table[i].Valid)
			{
				if (CStatistics::Instance->Table[i].Winner)
				{
				sprintf(StatisticsText, "%s  %s  %d  %4.2f  %d\n", CStatistics::Instance->Table[i].PlayerName.GetString(), CStatistics::Instance->Table[i].TuberName.GetString(), 
					CStatistics::Instance->Table[i].Position, CStatistics::Instance->Table[i].FastestLap, CStatistics::Instance->Table[i].TeamNumber);
				} else
				{
				sprintf(StatisticsText, "%s (game not finished)  %s  %d  %4.2f  %d\n", CStatistics::Instance->Table[i].PlayerName.GetString(), CStatistics::Instance->Table[i].TuberName.GetString(), 
					CStatistics::Instance->Table[i].Position, CStatistics::Instance->Table[i].FastestLap, CStatistics::Instance->Table[i].TeamNumber);
				}
				s = s + StatisticsText;
			}
		}
		WeCanvas::Instance()->Draw2DText(Font, s, 10, 60, 1600, 1600, 0xffffffff);*/
	
	WeCanvas::Instance()->DrawRectangle(0, 0, WeRenderDevice::Instance()->GetResWidth(), WeRenderDevice::Instance()->GetResHeight(), D3DCOLOR_XRGB(255,255,255), Background);
	CEGUI::System::getSingleton().renderGUI();
}



