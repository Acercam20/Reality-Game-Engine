#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct SphereParticleComponent
	{
		SphereParticleComponent(float _radius = 1.0f) :radius(_radius)
		{

		}
		float radius;
	};
}
