/*
   CActorTypesSupplier.h

   Autor: Nebojsa Vislavski
   Date: 19.1.2006.
   Version: 1.0

   This will tell anyone who wants to know actor types.

*/


#ifndef _CACTORTYPESSUPPLIER_H
#define _CACTORTYPESSUPPLIER_H


#include <CString.h>

#define MAX_ACTOR_TYPES		11

#define PICKUPTYPE_GLOBAL	10000


class CActorTypesSupplier
{   
private:
	static CActorTypesSupplier *_instance;

	struct SType
	{
		int Type;
		String Description;
	};

public:

    static CActorTypesSupplier *Instance();

   CActorTypesSupplier();

   SType Types[MAX_ACTOR_TYPES];

   
};


#endif
