//==========================================================
//
//		Randomクラス
//
//==========================================================
#pragma once

#include <random>

namespace GC2
{
	class Random
	{
	public:
		static float RangeFloat(float min,float max);
	private:
		static unsigned int seed_;				//シード値
		static std::minstd_rand random_;	//ランダムデバイス
	};
}

