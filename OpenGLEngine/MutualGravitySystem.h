#pragma once
#include "ECSConfig.h"
#include "MutualGravityComponent.h"

namespace Reality
{
	class MutualGravitySystem : public ECSSystem
	{
	public:
		MutualGravitySystem();
		void Update(float deltaTime);
	};
}
