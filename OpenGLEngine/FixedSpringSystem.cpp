#include "FixedSpringSystem.h"
#include "ForceAccumulatorComponent.h"

namespace Reality
{
	FixedSpringSystem::FixedSpringSystem()
	{
		requireComponent<TransformComponent>();
		requireComponent<FixedSpringComponent>();
	}

	void FixedSpringSystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto& springTransform = e.getComponent<TransformComponent>();
			auto& spring = e.getComponent<FixedSpringComponent>();

			if (spring.connectedEntity.hasComponent<TransformComponent>() && spring.connectedEntity.hasComponent<ForceAccumulatorComponent>())
			{
				auto& transform = spring.connectedEntity.getComponent<TransformComponent>();
				auto& forceAcc = spring.connectedEntity.getComponent<ForceAccumulatorComponent>();

				Vector3 relativePos = transform.position - springTransform.position;
				float length = glm::length(relativePos);
				if (length > 0)
				{
					float deltaL = length - spring.restLength;
					Vector3 force = -glm::normalize(relativePos);
					force *= spring.springConstant * deltaL;
					forceAcc.AddForce(force);
					getWorld().data.renderUtil->DrawLine(springTransform.position, transform.position, deltaL > 0 ? Color::Red : Color::Green);
				}
			}
		}
	}
}
