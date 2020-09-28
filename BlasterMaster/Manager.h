#pragma once
#include "useful_stuff.h"
template <typename T>
class Manager
{
public:
	Manager() {}
	virtual ~Manager() {}

	virtual void AddElement(T*) = 0;
};

template <typename A, typename ... B>
class Manager : public Manager<A>, public Manager<B>{};