/*
    CStartLocation.h.

	Autor: Nebojsa Vislavski
	Date: 4.4.2006.
	Version: 1.0

*/


#ifndef _CSTARTLOCATION_H
#define _CSTARTLOCATION_H

#include <WeFileStream.h>
#include <WeVector.h>
#include <WeActor.h>


class CTuber;

class CStartLocation: public WeActor
{
protected:

public:
	int			TeamNumber;
	CTuber*		Tuber; // koji je tuber napravljen na ovoj startnoj lokaciji	

				CStartLocation();

    void		Initialize();
	void		Serialize(WeFileStream &FS);
	void		Deserialize(WeFileStream &FS);
	void		Clone(WeActor *Destination, bool DeepCopy = false);

};



#endif
