/*

    SmartList.h

	Author: Nebojsa Vislavski
	Date: 10.4.2005
	Version: 1.4 (STABLE)

	This Smart Lists are very similar to STL lists except SmartList have some capabilities 
	to hold max items and to rearrange items. 

	History:
	 1.2 - added InList function (returns true if something found and Current Item becomes found item
	       PushBackUnique and PushFrontUnique functions. 
     1.3 - added MoveNext, MovePrev function. Moving current item forward or backward in list
	 1.4 - fixed big bug in Empty method that was only shown when Release Builds are done
	 1.5 - fixes sum bug in InsertAfter and InsertBefore
		   

 */


#ifndef _SMARTLIST
#define _SMARTLIST


template <class T> class CSmartList;

template <class T>
class CSmartListIterator
{
friend class CSmartList<T>;
protected:
	CSmartList<T> *Current;
public:
	T    &GetItem() { return Current->GetItem(); } 
	bool operator == (const CSmartListIterator<T> &i2);
	bool operator != (const CSmartListIterator<T> &i2);
	CSmartListIterator<T> &operator++ ();

};

template <class T>
bool CSmartListIterator<T>::operator == (const CSmartListIterator<T> &i2)
{
	return Current == i2.Current;
}

template <class T>
bool CSmartListIterator<T>::operator != (const CSmartListIterator<T> &i2)
{
	return Current != i2.Current;
}


template <class T>
CSmartListIterator<T> &CSmartListIterator<T>::operator++ ()
{
   Current = Current->pNext;
   return *this;	
}


template <class T>
class CSmartList
{
friend class CSmartListIterator<T>;
protected:
    CSmartList<T> *pNext;
	CSmartList<T> *pPrev;
	mutable CSmartList<T> *pCurrent;
	

	T Item;	
	int    ItemsCount;
	bool   Circular;
	int    MaxItems;

	CSmartList(bool Sentinel, bool SentinelAgain); // just private constructor
public:



    CSmartList();
    CSmartList(bool Circular);  // if Circular true than list will not report if at begining or end becouse this is irrelevant
    ~CSmartList();

	T &operator [] (int i);

	CSmartList <T> &operator = (const CSmartList <T> &list);
	CSmartList <T> &operator += (const CSmartList <T> &list);

    bool Sentinel;
	void PushBack(const T &Item);
	void PushFront(const T &Item);
	void PushBackUnique(const T &Item);
	void PushFrontUnique(const T &Item);
	void MovePrev(); // swaps place with prev element if exists
	void MoveNext(); // swaps with next element if exists
	void InsertAfter(const T &Item); // inserts item after current element
	void InsertBefore(const T &Item); // inserts item before current element

	bool InList(const T &Item);

	bool End() const   { return pCurrent == this; };
	bool Begin() const { return pCurrent == this; };
	bool Empty() { return ItemsCount == 0; }

	// With iterators
	CSmartListIterator<T> FirstI() const;
	CSmartListIterator<T> EndI() const;

	void FirstI(CSmartListIterator<T> &I) const { I.Current = this->pNext; };
	bool EndI(CSmartListIterator<T> &I) const { return (I.Current == (CSmartList<T> *)this); };
	void NextI(CSmartListIterator<T> &I) const { I.Current = I.Current->pNext; }

	void Next() const;
	void Prev() const;
	void First() const;
	void Last() const ;
	void Delete(); // deletes current item and next item becomes current
	void PopBack(); // deletes last item
	void PopFront(); // deletes first item
	T    &GetItem() const;

	int  GetItemsCount() { return ItemsCount; }

	void Clear(); // Clears all items in list
	void SetCircular(bool circ){Circular=circ;}
	bool GetCircular() { return Circular; };

	void DropToBack(); // drops current item to be last item in list.
	void DropToFront(); // drops current item to be first item in list. 

	void SetMaxItems(int MaxItems) { this->MaxItems = MaxItems; };
	int  GetMaxItems() { return MaxItems; };
};


template <class T>
CSmartListIterator<T> CSmartList<T>::FirstI() const
{
	CSmartListIterator<T> I;
	I.Current = this->pNext;
	return I;	
}

template <class T>
CSmartListIterator<T> CSmartList<T>::EndI() const
{
	CSmartListIterator<T> I;
	I.Current = (CSmartList<T> *)this;
	return I;	
}





template <class T>
CSmartList <T> &CSmartList<T>::operator =(const CSmartList <T> &list) 
{
   if (&list == this) return *this;

   Clear();

   for (list.First(); !list.End(); list.Next())
   {	   
	   PushBack(list.GetItem());
   }
   return *this;
}



template <class T>
CSmartList <T> &CSmartList<T>::operator +=(const CSmartList <T> &list) 
{
   if (&list == this) return *this;

   for (list.First(); !list.End(); list.Next())
   {	   
	   PushBack(list.GetItem());
   }
   return *this;
}



template <class T>
T &CSmartList<T>::operator [](int i) 
{
	int j = 0;	
	for (First(); !End(); Next())
	{
		if (i == j) return GetItem();
		j++;
	}
	return GetItem();
}



template <class T>
void CSmartList<T>::PopBack()
{
  if (Empty()) return;
  
  if (pCurrent == this->pPrev) Next();

  CSmartList<T> *tmp;
  tmp = this->pPrev;
  this->pPrev = this->pPrev->pPrev;
  this->pPrev->pNext = this;
  delete tmp; 
}

template <class T>
void CSmartList<T>::PopFront()
{
  if (Empty()) return;
  
  if (pCurrent == this->pNext) Next();
  if (pCurrent == this->pNext) pCurrent = this;

  CSmartList<T> *tmp;

  tmp = this->pNext;
  this->pNext = this->pNext->pNext;
  this->pNext->pPrev = this;
  delete tmp; 
}





template <class T>
void CSmartList<T>::DropToBack()
{
      pCurrent->pNext->pPrev = pCurrent->pPrev;
	  pCurrent->pPrev->pNext = pCurrent->pNext;

      pCurrent->pPrev = this->pPrev;
      pCurrent->pNext = this;
      this->pPrev = pCurrent;
      pCurrent->pPrev->pNext = pCurrent;	  
}


template <class T>
void CSmartList<T>::DropToFront()
{
	  pCurrent->pNext->pPrev = pCurrent->pPrev;
	  pCurrent->pPrev->pNext = pCurrent->pNext;

      pCurrent->pPrev = this;
      pCurrent->pNext = this->pNext;
      this->pNext = pCurrent;
      pCurrent->pNext->pPrev = pCurrent;
}



template <class T>
void CSmartList<T>::Clear()
{
  First();
  while (!Empty()) Delete();  
}


template <class T>
void CSmartList<T>::Delete()
{
  CSmartList<T> *tmp;
  if (!Empty())
  { 
	  tmp = pCurrent->pNext;
	  pCurrent->pNext->pPrev = pCurrent->pPrev;
	  pCurrent->pPrev->pNext = pCurrent->pNext;
	  delete pCurrent;
	  pCurrent = tmp;
	  ItemsCount--;
  }
}



template <class T>
void CSmartList<T>::First() const
{
  pCurrent = this->pNext;
}

template <class T>
void CSmartList<T>::Last() const 
{
  pCurrent = this->pPrev;
}




template <class T>
T &CSmartList<T>::GetItem() const
{
	return pCurrent->Item;
}



template <class T>
void CSmartList<T>::Next() const
{
  if ((Circular) && (pCurrent->pNext == this)) pCurrent = pCurrent->pNext->pNext;
  else pCurrent = pCurrent->pNext;

}


template <class T>
void CSmartList<T>::Prev() const
{
  if ((Circular) && (pCurrent->pPrev == this)) pCurrent = pCurrent->pPrev->pPrev;	  
  else pCurrent = pCurrent->pPrev;
}






template <class T>
CSmartList<T>::CSmartList()
{
  pNext = this;
  pPrev = this;
  pCurrent = this;
  Circular = false;
  MaxItems = 20000000;
  ItemsCount = 0;
  Sentinel = true;
}

template <class T>
CSmartList<T>::CSmartList(bool Sentinel, bool SentinelAgain)
{
  pNext = this;
  pPrev = this;
  pCurrent = this;
  Circular = false;
  MaxItems = 20000000;
  ItemsCount = 0;
  Sentinel = false;
}


template <class T>
CSmartList<T>::CSmartList(bool Circular)
{
  pNext = this;
  pPrev = this;
  pCurrent = this;
  this->Circular = Circular;
  MaxItems = 20000000;
  ItemsCount = 0;
  Sentinel = true;
}




template <class T>
CSmartList<T>::~CSmartList()
{ 
  if (Sentinel == true) Clear();
}

template <class T>
void CSmartList<T>::PushBack(const T &Item)
{
  if (ItemsCount < MaxItems)
  {
    CSmartList<T> *tmp = new CSmartList<T>(false, false);
    tmp->Item = Item;

    tmp->pPrev = this->pPrev;
    tmp->pNext = this;
    this->pPrev = tmp;
    tmp->pPrev->pNext = tmp;

    pCurrent = tmp;
    ItemsCount++;
  }
}






template <class T>
void CSmartList<T>::PushFront(const T &Item)
{
  if (ItemsCount < MaxItems)
  {
    CSmartList<T> *tmp = new CSmartList<T>(false, false);
    tmp->Item = Item;
 
    tmp->pPrev = this;
    tmp->pNext = this->pNext;
    this->pNext = tmp;
    tmp->pNext->pPrev = tmp;

    pCurrent = tmp;
    ItemsCount++;
  }
}



template <class T>
void CSmartList<T>::PushFrontUnique(const T &Item)
{
  if (!InList(Item))
  {
    if (ItemsCount < MaxItems)
    {
      CSmartList<T> *tmp = new CSmartList<T>(false, false);
      tmp->Item = Item;
 
      tmp->pPrev = this;
      tmp->pNext = this->pNext;
      this->pNext = tmp;
      tmp->pNext->pPrev = tmp;

      pCurrent = tmp;
      ItemsCount++;
    }
  }
}




template <class T>
void CSmartList<T>::PushBackUnique(const T &Item)
{
  if (!InList(Item))
  if (ItemsCount < MaxItems)
  {
    CSmartList<T> *tmp = new CSmartList<T>(false, false);
    tmp->Item = Item;

    tmp->pPrev = this->pPrev;
    tmp->pNext = this;
    this->pPrev = tmp;
    tmp->pPrev->pNext = tmp;

    pCurrent = tmp;
    ItemsCount++;
  }
}



template <class T>
bool CSmartList<T>::InList(const T &Item)
{
  if (!Empty())
  {
    CSmartList<T> *tmp;	
	bool btmp;
    tmp = pCurrent;
	btmp = GetCircular();
    
	SetCircular(false);
    for (First(); !End(); Next())
	{
		if ((T)Item == GetItem()) {
          SetCircular(btmp);
		  return true;
		}
	}	
    SetCircular(btmp);
    pCurrent = tmp;   
  }
  return false;
}

template <class T>
void CSmartList<T>::MovePrev()
{
  if (!Empty())
  {
	  CSmartList<T> *tmp, *tmp2;
	  if (pCurrent->pPrev == this) return;
	  tmp = pCurrent->pNext;
	  tmp2 = pCurrent->pPrev;

	  pCurrent->pNext = tmp2;
	  pCurrent->pPrev = tmp2->pPrev;
	  tmp2->pNext = tmp;
	  tmp2->pPrev = pCurrent;
  }
}

template <class T>
void CSmartList<T>::MoveNext()
{
  if (!Empty())
  {
	  CSmartList<T> *tmp, *tmp2;
	  if (pCurrent->pNext == this) return;
	  tmp = pCurrent->pPrev;
	  tmp2 = pCurrent->pNext;

	  pCurrent->pNext = tmp2->pNext;
	  pCurrent->pPrev = tmp2;
	  tmp2->pNext = pCurrent;
	  tmp2->pPrev = tmp;    
  }
}

template <class T>
void CSmartList<T>::InsertAfter(const T &Item)
{
  if (Empty()) 
  { 
	  PushBack(Item);
  } else 
  {
    CSmartList<T> *tmp = new CSmartList<T>(false, false);
	tmp->Item = Item;

	tmp->pPrev = pCurrent;
	tmp->pNext = pCurrent->pNext;
	pCurrent->pNext->pPrev = tmp;
	pCurrent->pNext = tmp;	
	pCurrent = tmp;  
	ItemsCount++;
  }
  
}

template <class T>
void CSmartList<T>::InsertBefore(const T &Item)
{
  if (Empty()) 
  { 
	  PushFront(Item);
  } else 
  {
    CSmartList<T> *tmp = new CSmartList<T>(false, false);
	tmp->Item = Item;

	tmp->pNext = pCurrent;
	tmp->pPrev = pCurrent->pPrev;
	pCurrent->pPrev->pNext = tmp;
	pCurrent->pPrev = tmp;		
	pCurrent = tmp;  
	ItemsCount++;
  }
  
}



#endif