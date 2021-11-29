/*
    File    : CLANServerBrowsePhase.cpp
	Autor   : Vuk Vranjkovic
	Date    : 23.3.2006.
	Version : 1.0
*/
#include <CLANServerBrowsePhase.h>
#include <CGame.h>
#include <CGUIHelper.h>
#include <CMenuPhase.h>
#include <CMainMenuPhase.h>
#include <CJoiningGamePhase.h>
#include <CGameClient.h>
#include <SmartList.h>
#include <WeNetworkMessage.h>
#include <WeTimer.h>
#include <WeNetworkDevice.h>
#include <CChooseMapPhase.h>
#include <CMultiRacePhase.h>
#include <CLocalizationSettings.h>



CLANServerBrowsePhase *CLANServerBrowsePhase::Instance = new CLANServerBrowsePhase;

//****************************** Helper Class ******************************//
CGameClient *CGameClient::Instance = new CGameClient();
struct SServerGame
{
	CEGUI::String MapName;
	CEGUI::String GameName;
	CEGUI::String IP;
	SServerGame(){}
};

bool operator==(const SServerGame &s1, const SServerGame &s2)
{
	if((s1.GameName==s2.GameName)&&(s1.MapName==s2.MapName)&&(s1.IP==s2.IP)) return true;
	return false;
}
CSmartList<SServerGame> Games;
//*************************************************************************//



CLANServerBrowsePhase::CLANServerBrowsePhase()
{
	pServerInfo = &CGame::Instance->ServerInfo;
}

void CLANServerBrowsePhase::Start()
{
	Games.Clear();

	bServerChoosen=false;
	Time=5.1f;
	bRefreshServers=false;

	CGUIHelper::CreateText("LANBrowse_LANGames",CLocalizationSettings::Instance()->GetLocalText("LAN GAMES"), 0.05f, 0.05f, 0.4f, 0.05f, "Font_24", "", "", "");

	CGUIHelper::CreateButton("LANBrowse_RefreshButton",CLocalizationSettings::Instance()->GetLocalText("REFRESH"),0.05f,0.65f,0.2f,0.1f, "RootWindow", &CLANServerBrowsePhase::OnRefresh);

	CGUIHelper::CreateButton("LANBrowse_JoinButton",CLocalizationSettings::Instance()->GetLocalText("JOIN"),0.05f,0.5f,0.2f,0.1f, "RootWindow", &CLANServerBrowsePhase::OnJoin);

	CGUIHelper::CreateButton("LANBrowse_CreateButton",CLocalizationSettings::Instance()->GetLocalText("CREATE"),0.25f,0.5f,0.2f,0.1f, "RootWindow", &CLANServerBrowsePhase::OnCreate);

	CGUIHelper::CreateButton("LANBrowse_BackButton",CLocalizationSettings::Instance()->GetLocalText("BACK"),0.75f, 0.8f, 0.25f, 0.1f, "RootWindow", &CLANServerBrowsePhase::OnBack);

	CGameClient::Instance->Initialize();
	CreateServerTable();
	CEGUI::Window *pText = CGUIHelper::CreateText("LANBrowse_WarningText",CLocalizationSettings::Instance()->GetLocalText("CHOOSE GAME FIRST"), 0.5f, 0.8f, 0.25f, 0.1f, "","", "", "");
	pText->setProperty("Visible","false");
}

void CLANServerBrowsePhase::End()
{
	//if ((NextPhase != CJoiningGamePhase::Instance) && (NextPhase != CChooseMapPhase::Instance))
	//{
	//	MessageBox(0, "GASIM CLIENTA", 0, 0);
	//	CGameClient::Instance->Disconnect();
	//	CGameClient::Instance->ShutDown();
	//}

	CEGUI::WindowManager &Mng = CEGUI::WindowManager::getSingleton();

	Mng.getWindow("LANBrowse_LANGames")->destroy();
	Mng.getWindow("LANBrowse_Servers")->destroy();
	Mng.getWindow("LANBrowse_RefreshButton")->destroy();
	Mng.getWindow("LANBrowse_JoinButton")->destroy();
	Mng.getWindow("LANBrowse_BackButton")->destroy();
	Mng.getWindow("LANBrowse_CreateButton")->destroy();
	Mng.getWindow("LANBrowse_WarningText")->destroy();
}

void CLANServerBrowsePhase::Tick()
{
	if(CGameClient::Instance->GetDataType() == DT_SERVER)
	{
		CGameClient::Instance->ResetDataType();
		AddServer();
	}

	Time+=WeTimer::Instance()->GetDeltaTimeF();
	if(Time>2.0f)
	{
		CEGUI::EventArgs e;
		OnRefresh(e);
		//CGameClient::Instance->FindLANServers();
		Time=0.0f;
	}
	if(bRefreshServers)
	{
		RefreshServers();
		bRefreshServers = false;
	}

	
	if(CGame::Instance->InputMessage.Exit)
	{
		CEGUI::EventArgs e;
		OnBack(e);
	}
}

void CLANServerBrowsePhase::Render()
{
	CMenuPhase::Instance->RenderStdScreen();
}



void CLANServerBrowsePhase::CreateServerTable()
{
	CSmartList<String> ColumnNames;
	ColumnNames.PushBack("NUM");
	ColumnNames.PushBack("GAME NAME");
	ColumnNames.PushBack("MAP NAME");

	CEGUI::MultiColumnList *pMCL = CGUIHelper::CreateMultiList("LANBrowse_Servers", 0.05f, 0.1f, 0.4f, 0.38f, ColumnNames, false, true, "RootWindow");
	pMCL->subscribeEvent(CEGUI::MultiColumnList::EventSelectionChanged, CEGUI::Event::Subscriber(&OnServerTableSelect));
	pMCL->subscribeEvent(CEGUI::Window::EventMouseDoubleClick, CEGUI::Event::Subscriber(&OnServerTableDoubleClick));
}

/**********************************************************
			          BUTTON Callbacks
**********************************************************/
// Join Callback
bool CLANServerBrowsePhase::OnJoin(const CEGUI::EventArgs &)
{
	
	if (CLANServerBrowsePhase::Instance->bServerChoosen)
	{
		CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
		CMenuPhase::Instance->SetPhase(CJoiningGamePhase::Instance);
	}
	else
	{
		CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
		CEGUI::WindowManager::getSingleton().getWindow("LANBrowse_WarningText")->setProperty("Visible","true");
	}
	return true;
}

// Refresh Callback
bool CLANServerBrowsePhase::OnRefresh(const CEGUI::EventArgs &)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	//Clear table
	CEGUI::MultiColumnList *pMCL = static_cast<CEGUI::MultiColumnList *>(CEGUI::WindowManager::getSingleton().
		getWindow("LANBrowse_Servers"));
	unsigned int cnt_r = pMCL->getRowCount();
	unsigned int cnt_c = pMCL->getColumnCount();
	for(int i=0; i!=cnt_r; i++)
	{
		for(int j=0; j!=cnt_c; j++)
		{
			MyListItem *pTmp = static_cast<MyListItem *>(pMCL->getItemAtGridReference(CEGUI::MCLGridRef(i,j)));
			pTmp->setText("");
		}
	}
	pMCL->setText("");
	Games.Clear();
	//Find new servers
	CGameClient::Instance->FindLANServers();
	return true;
}

// Create Callback
bool CLANServerBrowsePhase::OnCreate(const CEGUI::EventArgs &)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
	CMenuPhase::Instance->SetPhase(CMultiRacePhase::Instance);
	CMultiRacePhase::Instance->MultiRaceState = MRS_CHOOSEMAP;
	CGame::Instance->ServerInfo.LAN = true;
	return true;
}

// Back Callback
bool CLANServerBrowsePhase::OnBack(const CEGUI::EventArgs &)
{
	CMenuPhase::Instance->PlayMenuSound(MenuSoundAltButton);
	CMenuPhase::Instance->SetPhase(CMainMenuPhase::Instance);
	CGameClient::Instance->Disconnect();
	CGameClient::Instance->ShutDown();	

	return true;
}

//////////////////////////////////////////////////////////////

/**************************************************************
					ServerTable Callback
**************************************************************/
bool CLANServerBrowsePhase::OnServerTableSelect(const CEGUI::EventArgs &)
{
	CEGUI::MultiColumnList *pMCL = static_cast<CEGUI::MultiColumnList *> 
		(CEGUI::WindowManager::getSingleton().getWindow("LANBrowse_Servers"));
	MyListItem *pItem = static_cast<MyListItem *>(pMCL->getFirstSelectedItem());
	if (pItem)
	{
		unsigned int to = pMCL->getItemRowIndex(pItem);
		Games.First();
		for(unsigned int i=0; i!=to; i++) Games.Next();
		CGame::Instance->ServerInfo.IP = Games.GetItem().IP.c_str();
		CGame::Instance->ServerInfo.Name = Games.GetItem().GameName.c_str();
		CGame::Instance->ServerInfo.Map = Games.GetItem().MapName.c_str();
		CLANServerBrowsePhase::Instance->bServerChoosen=true;
	}
	return true;
}

bool CLANServerBrowsePhase::OnServerTableDoubleClick(const CEGUI::EventArgs &)
{
	CEGUI::MultiColumnList *pMCL = static_cast<CEGUI::MultiColumnList *> 
		(CEGUI::WindowManager::getSingleton().getWindow("LANBrowse_Servers"));
	MyListItem *pItem = static_cast<MyListItem *>(pMCL->getFirstSelectedItem());
	if(pItem)
	{
		unsigned int to = pMCL->getItemRowIndex(pItem);
		Games.First();
		for(unsigned int i=0; i!=to; i++) Games.Next();
		CGame::Instance->ServerInfo.IP = Games.GetItem().IP.c_str();
		CGame::Instance->ServerInfo.Name = Games.GetItem().GameName.c_str();
		CGame::Instance->ServerInfo.Map = Games.GetItem().MapName.c_str();
		CMenuPhase::Instance->PlayMenuSound(MenuSoundStdButton);
		CMenuPhase::Instance->SetPhase(CJoiningGamePhase::Instance);
	}
	else
	{
		CMenuPhase::Instance->PlayMenuSound(MenuSoundDenied);
	}
	return true;
}

/////////////////////////////////////////////////////////////////////

void CLANServerBrowsePhase::AddServer()
{
	SServerGame New;
	const int *Data = CGameClient::Instance->GetData();
	New.GameName = *((char **)&Data[0]);
	New.MapName = *((char **)&Data[1]);
	New.IP = *((char **)&Data[2]);
	delete [] *((char **)&Data[0]);
	delete [] *((char **)&Data[1]);
	delete [] *((char **)&Data[2]);

	for(Games.First(); !Games.End(); Games.Next())
	{
		if(Games.GetItem()==New) return;
	}
	Games.PushBack(New);
	bRefreshServers=true;
}

void CLANServerBrowsePhase::RefreshServers()
{
	CEGUI::MultiColumnList *pMCL = static_cast<CEGUI::MultiColumnList *>(CEGUI::WindowManager::getSingleton().
		getWindow("LANBrowse_Servers"));
	unsigned int cnt_r = pMCL->getRowCount();
	unsigned int cnt_c = pMCL->getColumnCount();
	int add_row = Games.GetItemsCount()-cnt_r;

	unsigned ToAdd = cnt_r;
	for(;add_row>0;add_row--, ToAdd++)
	{
		pMCL->addRow();
		for(int i=0; i!=cnt_c; i++)
		{
			pMCL->setItem(new MyListItem("", "LANBrowse_Servers"),i,ToAdd);
		}
	}
	if(!Games.Empty())
	{
		MyListItem *pItem;
		int i;char buf[10];
		for(i=0,Games.First(); !Games.End(); Games.Next(),i++)
		{
			pItem = static_cast<MyListItem *>(pMCL->getItemAtGridReference(CEGUI::MCLGridRef(i,0)));
			pItem->setText(itoa(i+1,buf,10));
			pItem = static_cast<MyListItem *>(pMCL->getItemAtGridReference(CEGUI::MCLGridRef(i,1)));
			pItem->setText(Games.GetItem().GameName.c_str());
			pItem = static_cast<MyListItem *>(pMCL->getItemAtGridReference(CEGUI::MCLGridRef(i,2)));
			pItem->setText(Games.GetItem().MapName.c_str());
		}
		pMCL->setText("");
	}
}

