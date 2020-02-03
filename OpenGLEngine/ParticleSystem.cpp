#include "ParticleSystem.h"

namespace Reality
{
	ParticleSystem::ParticleSystem()
	{
		requireComponent<TransformComponent>();
		requireComponent<ParticleComponent>();
	}

	void ParticleSystem::Update(float deltaTime, float colorModifier)
	{
		for (auto e : getEntities())
		{
			auto& transform = e.getComponent<TransformComponent>();
			auto& particle = e.getComponent<ParticleComponent>();

			particle.velocity += particle.acceleration * deltaTime;
			transform.position += particle.velocity * deltaTime;

			if (DEBUG_LOG_LEVEL > 0)
			{
				particle.color.r = colorModifier * 0.05;
				particle.color.g = 1 - colorModifier * 0.05;
				getWorld().data.renderUtil->DrawSphere(transform.position, 1.0f, particle.color);
			}
			//getWorld().data.renderUtil->DrawLine(transform.position,Vector3(0,0,0), Color::Red);
		}
	}
}
