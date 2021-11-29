#include <CDialog.h>
#include <CComponent.h>



#ifndef _CDIALOGSELECTION_H
#define _CDIALOGSELECTION_H


class CDialogSelection: public CDialog
{
protected:
	static CDialogSelection *_instance;

	void OnShow();
	void OnClick(DWORD ID);

public:
	static CDialogSelection *Instance();

	CCheckBox *Prefabs;
	CCheckBox *Lights;
	CCheckBox *PathNodes;
	CCheckBox *StartLocations;
	CCheckBox *ParticleSystems;

	bool Create(HWND hParent, bool Show = true);
};


#endif
