#pragma once
#include "ECSConfig.h"
#include "BungeeParticleComponent.h"

namespace Reality
{
	class BungeeParticleSystem : public ECSSystem
	{
	private:
		
	public:
		BungeeParticleSystem();
		void Update(float deltaTime);

	};
}
