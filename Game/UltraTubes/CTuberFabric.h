/*
      File: CTuberFabric.h
	  Author: Tomislav Kukic
	  Date: 23.03.2006
	  Version: 1.0
*/




#ifndef _CTuberFabric_h_
#define _CTuberFabric_h_


#include <CTuber.h>
#include <CTuberDescription.h>

class CProfile;

class CTuberFabric
{
public:
	static CTuberFabric *Instance;


	// Ovo ce da napravi tuber-a ali mu nece podesiti Lokaciju, niti ga dodati u svet
	// ovo ce samo da napravi tuber-a i da mu podesi parametre iz tuberdesc-a
	CTuber* CreateTuber(CTuberDescription  &TuberDesc, CProfile *Owner);
};




#endif
