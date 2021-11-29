/*
    WeNetworkMessageQueue.h

	Autor: Nebojsa Vislavski
	Date: 8.1.2006.
	Version: 1.0

	This is network message queue. Used for queing and processing of net msgs.

*/ 


#ifndef _WENETWORKMESSAGEQUEUE_H
#define _WENETWORKMESSAGEQUEUE_H

#include <WeNetworkMessage.h>
#include <SmartList.h>
#include <WindEngine.h>

const int MAX_NETMESSAGEQUEUE_SIZE = 100;

class WINDENGINE_API WeNetworkMessageQueue
{
protected:

	WeNetworkMessage	Messages[MAX_NETMESSAGEQUEUE_SIZE];
	int					Last;
	int					Pointer;

public:
	
						WeNetworkMessageQueue(): Last(0), Pointer(0) {}

	WeNetworkMessage*	GetMessageForWriting();  // this automaticly advances to next free msg. if overrun happens it will just rewrite last msg in queue
	void				NextMessageForWriting(); // this will go to next message for writing
	WeNetworkMessage*	GetMessageForReading(); // returns NULL if there is no more msgs
	void				Clear();
	void				BeginReading();	
        
};



#endif