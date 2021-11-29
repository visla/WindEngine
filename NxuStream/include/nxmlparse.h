//
//           nxmlparse
// 
// A simple parser for loading
// valid xml into a useful tree data structure.
// See the .cpp file for details.
//

#ifndef NX_XMLPARSE_H
#define NX_XMLPARSE_H

#include "array.h"
#include "nxstring.h"



class nxmlNode
{
public:
	class Attribute
	{
	  public:
		nxString key;
		nxString value;
		Attribute(const char *k):key(k){}
	};
	nxString tag;
	pxArray<Attribute*> attributes;  
	pxArray<nxmlNode*>  children;
	nxString body;
	nxmlNode *Child(const char *s){for(int i=0;i<children.count;i++)if(s==children[i]->tag)return children[i];return NULL;}
	Attribute *hasAttribute(const char *s){for(int i=0;i<attributes.count;i++)if(s==attributes[i]->key)return attributes[i];return NULL;}
	const Attribute *hasAttribute(const char *s)const{for(int i=0;i<attributes.count;i++)if(s==attributes[i]->key)return attributes[i];return NULL;}
	nxString &attribute(const char *s){Attribute *a=hasAttribute(s);if(a)return a->value;return (attributes.Add(new Attribute(s)))->value;}
	const nxString &attribute(const char *s)const;
	nxmlNode(const char *_tag):tag(_tag){}
	nxmlNode(){}
	~nxmlNode();
};


nxmlNode *nxmlParseFile(const char *filename);
void      nxmlSaveFile(nxmlNode *elem,const char *filename);
nxmlNode *nxmlParseFile(FILE* file);
void      nxmlSaveFile(nxmlNode *elem,FILE* file);



#endif
