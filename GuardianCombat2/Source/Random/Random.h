//==========================================================
//
//		Random�N���X
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
		static unsigned int seed_;				//�V�[�h�l
		static std::minstd_rand random_;	//�����_���f�o�C�X
	};
}

