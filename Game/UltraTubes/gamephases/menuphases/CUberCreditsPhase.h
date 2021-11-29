/*
	Filename:	CUberCreditsPhase.h
	Author:		Goran Bogdanovic
*/


#ifndef CUBERCREDITSPHASE_H
#define CUBERCREDITSPHASE_H


#include <CAbstractPhase.h>
#include <CString.h>

#define DEFAULT_DURATION	8.0f

class CUberText;
class WeTexture;


enum DudeCoordinate
{
	DudeCoordinateX,
	DudeCoordinateY
};

enum DudeElements
{
	DudeElementsName,
	DudeElementsInfo,
	DudeElementsPicture
};

enum DudeStage
{
	DudeStageName,
	DudeStageInfo,
	DudeStagePortrait
};


class CUberCreditsDude
{
protected:
	CUberText*					Name;
	CUberText*					Info;
	WeTexture*					Picture;

	String						NameText;
	String						InfoText;
	float						TotalDuration;
	int							ChosenPreset;

	DudeStage					CurrentStage;

	bool						Completed;
	float						Timer;
	float						TimerPicture;
	float						PictureTimeShow;
	float						PictureTimeStay;
	float						PictureTimeHide;
	float						PictureAlpha;

	static float				WOffset;
	static float				HOffset;

	float						GetCoord(DudeCoordinate Coord, DudeElements Elem, int Preset);

public:
	CUberCreditsDude(String DudesName, String DudesInfo, String Portrait, float Duration = DEFAULT_DURATION);
	~CUberCreditsDude();

	void						Tick();
	void						Render();

	float						GetTimer();
	bool						IsComplete();

};


class CUberCreditsPhase : public CAbstractPhase
{
protected:
	int							DudesCount;

	String						DudeNames[20];
	String						DudeInfos[20];
	String						DudePorts[20];

	int							CurrentDudeIndex;
	CUberCreditsDude*			CurrentDude;

public:
	static CUberCreditsPhase*	Instance;

	CUberCreditsPhase();
	~CUberCreditsPhase();

	void						Start();
	void						End();
	void						Tick();
	void						Render();

};


#endif