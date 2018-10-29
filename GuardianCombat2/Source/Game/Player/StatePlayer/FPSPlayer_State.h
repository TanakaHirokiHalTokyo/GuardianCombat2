#pragma once

class FPSPlayer;

class FPSPlayerState
{
public:
	virtual void Act(FPSPlayer* player) = 0;			//é¿çsä÷êî
};