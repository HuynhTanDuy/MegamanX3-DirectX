#pragma once
#include "Boss1State.h"
#include "Boss1Data.h"
#include "Boss1BornState.h"

class Boss1ZiczacState : public Boss1State
{
public:
	Boss1ZiczacState(Boss1Data *Boss1Data);
	~Boss1ZiczacState();

	void Update(float dt);

	// void HandleKeyboard(std::map<int, bool> keys);

	virtual StateName GetState(); // lay state nhan vat

protected:

};

