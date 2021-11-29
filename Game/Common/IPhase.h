/*
    IPhase.h

    Author: Nebojsa Vislavski
	Date: 8.6.2005.
	Version: 1.0
	 
    Phase is main interface in all game applications that has some functionality


*/

#pragma once

enum PHASES 
{
  PHASE_INITIALIZE_GAME,
  PHASE_TERMINATED, 
  PHASE_APPSTART, 
  PHASE_APPEND,
  PHASE_INITIALIZE,
  PHASE_DEINITIALIZE,
  PHASE_MAINLOOP,
  PHASE_CREATEDIALOGS

};


class IPhase
{
protected:
   bool bStarted;
   PHASES Phase;


    void SetStarted(bool Started) { bStarted = Started; };
public:
    IPhase(void);
	virtual ~IPhase(void);

	bool GetStarted() { return bStarted; }; 

	virtual PHASES Start() = 0;  // Every phase needs to return next phase identifier. 
	void Stop() { bStarted = false; };

};
