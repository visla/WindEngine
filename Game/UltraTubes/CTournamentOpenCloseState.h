/*
    Autor: Nebojsa Vislavski
	Date: 27.3.2006.
	Version: 1.0

     

*/

#ifndef _CTOURNAMENTOPENCLOSESTATE_H
#define _CTOURNAMENTOPENCLOSESTATE_H

#include <CString.h>
#include <WeStream.h>

class CTournamentOpenCloseState
{
protected:
public:
	String			StateName;
	String			TournamentName;

	void			Serialize(WeStream &S);
	void			Deserialize(WeStream &S);


};

#endif