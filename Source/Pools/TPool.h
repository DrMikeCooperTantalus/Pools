#pragma once

#include "CoreMinimal.h"

// dynamic infinitely growable array that never reallocates a pointer
// works with allocated pages of a fixed size that hold multiple elements
// the T class has to implement a bool IsValid() which uses some internal data to say if the slot is free
//
// You add a new entry to the Pool using Add() first to get a pointer and then fill that pointer in with valid data
// or pass in a value to get added via T's copy constructor
// to remove an element set the object to invalid in whatever way you've defined
//
// You can iterate over all valid elements like so:
//   for (TPool<T, PageSize>::Iterator it = Pool.Begin(); it.IsValid(); it++)
//   {
//      T* item = *it;
//   }
template<typename T, int PageSize> class TPool
{
public:
	struct Iterator
	{
		// initialise to an invalid value
		Iterator() { Page = -1; Index = 0; Pool = nullptr; }
		int Page;
		int Index;
		TPool<T, PageSize>* Pool;
		
		bool IsValid() { return (**this) != nullptr; }
		
		void operator++ ()
		{
			// should never happen if the iterator came from Begin() first, but check anyway
			if (Pool == nullptr)
				return;

			// step over the current page, then all subsequent ones looking for a suitable next value
			do
			{
				while (Index < PageSize)
				{
					Index++;
					if (Pool->Pages[Page][Index].IsValid())
					{
						return;
					}
				}
				Page++;
				Index = 0;
			}
			while (Page < Pool->Pages.Num());
			
			// if we've hit this point, there are no valid entries ahead, so mark as done/not valid
			Page =-1;
			return;
		}

		// dereference gives us a T* pointer like STL iterators
		T* operator*()
		{
			if (Pool == nullptr)
				return nullptr;
			if (Page == -1)
				return nullptr;
			
			return &Pool->Pages[Page][Index];
		}
	};

	T* Add()
	{
		// find a free slot and return pointer to it
		for (int i=0; i< Pages.Num(); i++)
		{
			T* Page = Pages[i];
			for (int j=0; j<PageSize; j++)
			{
				if (Page[j].IsValid() == false)
					return &Page[j];
			}
		}
		// we've hit the end, need to add a new page 
		T* NewPage = new T[PageSize];
		Pages.Add(NewPage);
		// and return the first entry
		return &NewPage[0];
	}

	T* AddItem(T& item)
	{
		T* InPlaceItem = Add();
		*InPlaceItem = item;
		return InPlaceItem;
	}
	
	// find the first valid slot (and ref this pool)
	Iterator Begin()
	{
		Iterator it;
		it.Pool = this;
		for (it.Page=0; it.Page< Pages.Num(); ++it.Page)
		{
			T* Page = Pages[it.Page];
			for (it.Index=0; it.Index<PageSize; ++it.Index)
			{
				if (Page[it.Index].IsValid())
					return it;
			}
		}
		// return an invalid value
		it.Page = -1;
		return it;
	}

	void Clear()
	{
		// call destructor on all objects and clear
		for (int i=0; i<Pages.Num(); i++)
			delete[] Pages[i];
		Pages.Empty();
	}

	// accessor for the demo UI - shouldn't normally need access to this
	const TArray<T*>& GetPages() { return Pages; }
	
private:
	// these get created on the fly as big continuous blocks of memory
	TArray<T*> Pages;
	
};