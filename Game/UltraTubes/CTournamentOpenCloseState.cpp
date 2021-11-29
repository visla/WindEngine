/*
    Autor: Nebojsa Vislavski
	Date: 27.3.2006.
	Version: 1.0
    

*/


#include <CTournamentOpenCloseState.h>



void CTournamentOpenCloseState::Serialize(WeStream &S)
{
	S.WriteString(TournamentName);
	S.WriteString(StateName);

}

void CTournamentOpenCloseState::Deserialize(WeStream &S)
{
	TournamentName = S.ReadString();
	StateName = S.ReadString();
}

