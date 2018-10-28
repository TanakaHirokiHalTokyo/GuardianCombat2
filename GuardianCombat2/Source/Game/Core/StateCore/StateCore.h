#pragma once
class Core;

//		Core‚Ìó‘Ô‚ğŠÇ—‚·‚é
class StateCore
{
public:
	virtual ~StateCore(){}
	virtual void UpdateCore(Core* core) = 0;
};