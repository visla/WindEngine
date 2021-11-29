/*

    CEditorGrid.h

	
	Autor: Nebojsa Vislavski
	Date: 20.12.2005.
	Version: 1.0


	This is grid used in editors

*/

#ifndef _CEDITORGRID_H
#define _CEDITORGRID_H

#include <WeActor.h>

class CEditorGrid: public WeActor
{
protected:

public:
	void Initialize(WeWorld* WorldToBeIn);
};

#endif