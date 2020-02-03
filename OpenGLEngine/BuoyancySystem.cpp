#include "BuoyancySystem.h"
#include "BuoyancyComponent.h"
#include "TransformComponent.h"
#include "ForceAccumulatorComponent.h"
#include "ParticleComponent.h"


namespace Reality
{
	BuoyancySystem::BuoyancySystem()
	{
		requireComponent<BuoyancyComponent>();
	}

	void BuoyancySystem::Update(float deltaTime, float waterDensity)
	{
		for (auto e : getEntities())
		{
			auto& buoyancyParticle = e.getComponent<BuoyancyComponent>();
			
			if (buoyancyParticle.floatingEntity.hasComponent<TransformComponent>())
			{
				float depth = buoyancyParticle.floatingEntity.getComponent<TransformComponent>().position.y;

				if (depth >= buoyancyParticle.waterHeight + buoyancyParticle.maxDepth)
				{
					return;
				}

				Vector3 force = Vector3(0, 0, 0);

				if (depth <= buoyancyParticle.waterHeight - buoyancyParticle.maxDepth)
				{
					force.y = waterDensity * buoyancyParticle.volume;
					if (buoyancyParticle.floatingEntity.hasComponent<ForceAccumulatorComponent>())
					{
						buoyancyParticle.floatingEntity.getComponent<ForceAccumulatorComponent>().AddForce(force);
					}
					return;
				}
				else
				{
					force.y = waterDensity * buoyancyParticle.volume * (depth - buoyancyParticle.maxDepth - buoyancyParticle.waterHeight) / 2 * buoyancyParticle.maxDepth;
					if (buoyancyParticle.floatingEntity.hasComponent<ForceAccumulatorComponent>())
					{
						buoyancyParticle.floatingEntity.getComponent<ForceAccumulatorComponent>().AddForce(force);
					}
					return;
				}
				buoyancyParticle.floatingEntity.getComponent<ParticleComponent>().color = Color(0, 0, 0, 0);
			}
		}
	}
}
