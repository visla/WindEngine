/*
     WeStream.cpp

	 Autor: Nebojsa Vislavski
	 Date: 13.9.2005
	 Version: 1.0

     This is main abstract stream. If you want to create stream inherit this.
*/ 


#include <WeStream.h>

WeStream::WeStream()
{
	IsOpened = false;
}