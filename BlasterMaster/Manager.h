#pragma once
#include "useful_stuff.h"
template <typename T>
class Manager
{
public:
	virtual void AddElement(T*) = 0;
	virtual void RemoveElement(T*) = 0;

	Manager() {}
	virtual ~Manager() {}
};

template <typename T>
class Managed
{
protected:
	Manager<T>* manager;
public:
	virtual Manager<T>* GetManager() {
		return manager;// LeSon add
	}
	virtual void SetManager(Manager<T>* manager) 
	{
		this->manager = manager;
	}
	Managed() {}
	virtual ~Managed() {}
};