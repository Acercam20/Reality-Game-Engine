#include "BungeeParticleSystem.h"
#include "BungeeParticleComponent.h"
#include "TransformComponent.h"
#include "ForceAccumulatorComponent.h"

namespace Reality
{
	BungeeParticleSystem::BungeeParticleSystem()
	{
		requireComponent<BungeeParticleComponent>();
	}

	void BungeeParticleSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto& bungeeCord = e.getComponent<BungeeParticleComponent>();

			if (bungeeCord.connectedEntityA.hasComponent<TransformComponent>() && bungeeCord.connectedEntityB.hasComponent<TransformComponent>())
			{
				auto& transformA = bungeeCord.connectedEntityA.getComponent<TransformComponent>();
				auto& transformB = bungeeCord.connectedEntityB.getComponent<TransformComponent>();

				//Vector3 force;
				Vector3 force = transformA.position - transformB.position;

				float length = glm::length(force);
				float deltaL = length - bungeeCord.restLength;

				if (length >= bungeeCord.restLength)
				{
					force *= bungeeCord.springConstant * deltaL;
					force = -glm::normalize(force);
					force *= length;

					if (bungeeCord.connectedEntityA.hasComponent<ForceAccumulatorComponent>())
					{
						bungeeCord.connectedEntityA.getComponent<ForceAccumulatorComponent>().AddForce(force);
					}
					if (bungeeCord.connectedEntityB.hasComponent<ForceAccumulatorComponent>())
					{
						bungeeCord.connectedEntityB.getComponent<ForceAccumulatorComponent>().AddForce(-force);
					}
				}
				getWorld().data.renderUtil->DrawLine(transformA.position, transformB.position, deltaL > 0 ? Color::Red : Color::Green);
			}
		}
	}
}
