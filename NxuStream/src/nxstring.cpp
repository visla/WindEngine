//
//      nxString
//
// More convenient than using character arrays (char*). 
// Its your typical nxString class - nuff said.
//
//
//

#include <stdarg.h>
#include <assert.h>
#include <ctype.h>

#include "nxstring.h"




nxString::nxString()
{
	buf=NULL;
	size=length=0;
	prev=next=this;
}

nxString::nxString(const nxString &s)
{
	buf=NULL;
	size=length=0;
	prev=next=this;
	*this = s;
}

nxString &nxString::operator=(const nxString& s)
{
	if(next != this) 
	{
		prev->next=next;
		next->prev=prev;
		next=prev=this;
	}
	else if (buf) 
	{
        delete[] buf;
    }
	buf=NULL;
	size=length=0;
	if(s.buf==NULL || s[0]=='\0')
	{
		return *this;
	}
	buf    = s.buf;
	length = s.length;
	size   = s.size;
	prev   = &s;
	next   = s.next;
	s.next->prev = this;
	s.next = this;
	return *this;
}

nxString::nxString(const char *s)
{
	buf=NULL;
	next=prev=this;
	size=length=0;
	*this = s;
}


nxString &nxString::operator=(const char *s)
{
	if(s==*this) return *this;
	if(next != this) 
	{
		prev->next=next;
		next->prev=prev;
		next=prev=this;
		buf=NULL;
		size=length=0;
	}
	if(buf) buf[0]='\0';
	length=0;
	*this += s;
	return *this;
}

void nxString::resize(int _newsize)
{
	detach();  
	char *newbuf = new char[_newsize];
	for(int i=0;buf && i<size && i<_newsize;i++)
	{
		newbuf[i]=buf[i];
		if(buf[i]=='\0') break;  
	}
	if(buf) delete []buf;
	buf=newbuf;
	size=_newsize;
}

nxString::operator const char *() const{ 
	static char *rv="";
	return buf?buf:"\0";
} 


//----------------------------------------------
// Concatenating operators.  Note '+=' is not always a member.
//
nxString &nxString::operator+=(const char *b)
{
	if(!b) return *this;
	int blen = (int)strlen(b);
	if(blen==0) 
	{
		return *this;
	}
	detach();
	if(size < length+blen +1)
	{
		int newsize=16;
		while(newsize < length+blen +1) newsize *=2;
		assert( b-buf <0 || b-buf >=size);  // implement this case later, adding part of onesself so cant delete buf during resize.
		resize(newsize);
	}
	strcpy(buf+length,b);
	length+=blen;
	return *this;
}




nxString::nxString(const char *s,int slen)
{
	buf=NULL;
	next=prev=this;
	if(slen==0)
	{
		*this = "";
		return;
	}
	assert(slen>0);
	length = slen;
	size   = length+1;
	buf=new char[size];
	if(s) strncpy(buf,s,slen);
	else  buf[0]='\0';
	buf[length] = '\0';
	return;
}

void nxString::detach()
{
	if(this==next) 
	{
		if(!buf)
		{
			size=16;
			buf=new char[size];
			buf[0]='\0';
		}
		return;
	}
	if(size=0) size=16;
	buf=new char[size];
	strncpy(buf,(const char*)*next,size);
	prev->next=next;
	next->prev=prev;
	next=prev=this;
}

nxString::~nxString() 
{
	if(next != this) 
	{
		prev->next=next;
		next->prev=prev;
		next=prev=this;
		buf=NULL;
		size=length=0;
	}
    if (buf) {
        delete[] buf;
		buf=NULL;
		size=length=0;
    }
}



nxString operator+(const nxString &a,const nxString &b){
	nxString s(a);
	s += b;
	return s; 
}

//----------------------------------------------
// The obvious operators for comparing strings
// Dont you wish strcmp() was called strdiff().
//
int operator==(const nxString &a,const nxString &b){
	return (!stricmp(a,b));
}
int operator!=(const nxString &a,const nxString &b){
	return (stricmp(a,b)); 
}
int operator>(const nxString &a,const nxString &b){
	return (stricmp(a,b)>0);  
}
int operator<(const nxString &a,const nxString &b){
	return (stricmp(a,b)<0);  
}
int operator>=(const nxString &a,const nxString &b){
	return (stricmp(a,b)>=0); 
}
int operator<=(const nxString &a,const nxString &b){
	return (stricmp(a,b)<=0); 
}
int operator==(const nxString &a,const char *b){
	return (!stricmp(a,b));
}
int operator!=(const nxString &a,const char *b){
	return (stricmp(a,b));
}
int operator> (const nxString &a,const char *b){
	return (stricmp(a,b)>0);  
}
int operator< (const nxString &a,const char *b){
	return (stricmp(a,b)<0);  
}
int operator>=(const nxString &a,const char *b){
	return (stricmp(a,b)>=0); 
}
int operator<=(const nxString &a,const char *b){
	return (stricmp(a,b)<=0); 
}
int operator==(const char *a,const nxString &b){
	return (!stricmp(a,b));
}
int operator!=(const char *a,const nxString &b){
	return (stricmp(a,b));
}
int operator> (const char *a,const nxString &b){
	return (stricmp(a,b)>0);  
}
int operator< (const char *a,const nxString &b){
	return (stricmp(a,b)<0);  
}
int operator>=(const char *a,const nxString &b){
	return (stricmp(a,b)>=0); 
}
int operator<=(const char *a,const nxString &b){
	return (stricmp(a,b)<=0); 
}

nxString &nxString::sprintf(char *format,...) 
{
	detach();
	va_list vl;
	va_start(vl, format);
	if (buf) {	
		delete[] buf;
		buf=NULL;
	}
	int  n;
	char *tmp = NULL;
    int tmpsize = 128;
	while( (n = _vsnprintf((tmp=new char[tmpsize]), tmpsize-1, format, vl))<0) {
		delete []tmp;
		tmpsize *=2;
	}
    buf = new char[n+1];
	assert(buf);
	strcpy(buf,tmp);
	delete []tmp;
	va_end(vl);
	return *this;
}
	

