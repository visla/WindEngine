/*
    WeNetworkMessageQueue.cpp

	Autor: Nebojsa Vislavski
	Date: 8.1.2006.
	Version: 1.0

	This is network message queue. Used for queing and processing of net msgs.

*/ 

#include <WeNetworkMessageQueue.h>


WeNetworkMessage *WeNetworkMessageQueue::GetMessageForWriting()
{
	WeNetworkMessage *msg;	
	msg = &Messages[Last];
	return msg;
}


void WeNetworkMessageQueue::NextMessageForWriting()
{
	Last++;
	if (Last == MAX_NETMESSAGEQUEUE_SIZE) Last = MAX_NETMESSAGEQUEUE_SIZE-1;	
}

WeNetworkMessage *WeNetworkMessageQueue::GetMessageForReading()
{
	if (Pointer >= Last) return NULL;	
	Pointer++;
	return &Messages[Pointer-1];
}


void WeNetworkMessageQueue::Clear()
{
	Pointer = 0;
	Last = 0;
}

void WeNetworkMessageQueue::BeginReading()
{
	Pointer = 0;
}

