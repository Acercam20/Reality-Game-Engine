#pragma once
#include "ECSConfig.h"
#include "ParticleSystem.h"
#include "ParticleContactEvent.h"

namespace Reality
{
	class ParticleContactSystem : public ECSSystem
	{
	public:
		ParticleContactSystem();
		void Update(float deltaTime);
	private:
		float CalculateSeperatingVelocity(ParticleContactEvent& contact);
		void ResolveVelocity(ParticleContactEvent& contact, float deltaTime);
	};
}
