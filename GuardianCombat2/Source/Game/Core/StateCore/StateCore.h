#pragma once
class Core;

//		Core�̏�Ԃ��Ǘ�����
class StateCore
{
public:
	virtual ~StateCore(){}
	virtual void UpdateCore(Core* core) = 0;
};