#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct DragForceComponent
	{
		DragForceComponent(float kOne = 1.0f,float kTwo = 1.0f) : k1(kOne),k2(kTwo)
		{

		}
		float k1;
		float k2;
	};
}
