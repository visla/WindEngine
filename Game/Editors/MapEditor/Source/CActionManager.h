/*

    CActionManager.h

	Autor: Nebojsa Vislavski
	Date: 26.12.2005.
	Version: 1.0

	This is action manager used to record actions in editor

*/

#ifndef _CACTIONMANAGER_H
#define _CACTIONMANAGER_H

#include <SmartList.h>
#include <WeActor.h>
#include <CString.h>



class CActionManager
{
public:

	class CAction
	{
	public:
	    class CActorPart
		{
		public:
			WeVector LocationV;
			WeVector XAxisV;
			WeVector YAxisV;
			WeVector ZAxisV;
			WeActor *Actor;
		};
		CSmartList <CActorPart *> ActionParts;
		String Name;
	};

protected:
	static CActionManager *_instance;

	CAction *CurrentAction;
	bool	 MakingAction;

public:
	static CActionManager *Instance();

	CActionManager();

	
	void RecordAction(WeActor *actor);
	void RemoveRecordedAction(WeActor *actor);
	void StartRecording(StringParam Name);						// starts recording of object
	void StopRecording();						// fills actions file with data
	void LoadActions();
	void PlayAction(StringParam Name);			// this will play action from selected object
	void PlayCurrentAction();					// this will play current action
	bool Recording() { return MakingAction; }   // if recording is on
	void DeleteAction(StringParam Name);
	
	CSmartList <CAction *> Actions;

};

#endif