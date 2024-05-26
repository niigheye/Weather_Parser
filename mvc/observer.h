#pragma once
#include <vector>

class Observer
{
public:
	virtual void Update() = 0;
};

class Observable
{
	std::vector<Observer*> observers;

public:
	void AddObserver(Observer* observer)
	{
		observers.push_back(observer);
	}

	void NotifyUpdate()
	{
		for (auto& observer : observers)
			observer->Update();
	}
};