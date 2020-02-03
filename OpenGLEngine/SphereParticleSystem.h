#pragma once
#include "ECSConfig.h"
#include "SphereParticleComponent.h"
#include "TransformComponent.h"

namespace Reality
{
	class SphereParticleSystem : public ECSSystem
	{
	public:
		SphereParticleSystem();
		void Update(float deltaTime);
	private:
		ECSEntity wallDummy;
		bool createWall = false;
	};
}
