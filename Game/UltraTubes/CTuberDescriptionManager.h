/*
     CTuberDescriptionManager.h

	 Autor: Nebojsa Vislavski
	 Date: 15.3.2006.
	 Version: 1.0

*/

#ifndef _CTUBERDESCRIPTIONMANAGER_H
#define _CTUBERDESCRIPTIONMANAGER_H

#include <SmartList.h>
#include <CTuberDescription.h>

class CTuberDescriptionManager
{
protected:
	static CTuberDescriptionManager* _Instance;

public:
	static CTuberDescriptionManager* Instance();

	CSmartList <CTuberDescription> Descriptions;
	CTuberDescriptionManager();

	void	LoadDescriptions();
	void	SaveDescriptions();
	void	AddDescription(CTuberDescription &desc);
	void	RemoveDescription(CTuberDescription &desc);	


};

#endif

