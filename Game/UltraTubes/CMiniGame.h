/*
   CMiniGame.h

   Autor: Goran Bogdanovic
   Date: 3.4.2006.
   Version: 1.0
*/


#ifndef _CMiniGame_H
#define _CMiniGame_H

#include <WeTexture.h>

class CMiniGame
{
public:
	bool				Finished;
	WeTexture			*Background;

						CMiniGame();
						~CMiniGame();

	virtual void		Tick();
	virtual void		Render();

};


#endif