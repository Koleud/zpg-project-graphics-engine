#include <vector>
#include <algorithm>
#include "Observer.h"

#pragma once
class Subject
{
private:
	std::vector<Observer*> observers; // Vector of pointers to Observer objects
public:
	virtual ~Subject() = default;

	void Attach(Observer* observer);
	void Detach(Observer* observer);

	void Notify();
};

