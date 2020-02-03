#include "MutualGravitySystem.h"
#include "ParticleComponent.h"
#include "ForceAccumulatorComponent.h"
#include "TransformComponent.h"

namespace Reality
{
	MutualGravitySystem::MutualGravitySystem()
	{
		requireComponent<MutualGravityComponent>();
	}

	void MutualGravitySystem::Update(float deltaTime)
	{
		for (auto e : getEntities())
		{
			auto& mutualGrav = e.getComponent<MutualGravityComponent>();

			ECSEntity particleList[10] = { mutualGrav.connectedEntity1,mutualGrav.connectedEntity2 ,mutualGrav.connectedEntity3,
											mutualGrav.connectedEntity4 ,mutualGrav.connectedEntity5 ,mutualGrav.connectedEntity6,
											mutualGrav.connectedEntity7 ,mutualGrav.connectedEntity8 ,mutualGrav.connectedEntity9,
											mutualGrav.connectedEntity10 };

			for (int i = 0; i < 10; i++)
			{
				Vector3 force = Vector3(0, 0, 0);
				for (int j = 0; j < 10; j++)
				{
					if (i != j)
					{
						auto& transformA = particleList[i].getComponent<TransformComponent>();
						auto& transformB = particleList[j].getComponent<TransformComponent>();
						auto& forceAccA = particleList[i].getComponent<ForceAccumulatorComponent>();
						auto& forceAccB = particleList[j].getComponent<ForceAccumulatorComponent>();
						Vector3 relativePos = transformA.position - transformB.position;
						float length = glm::length(relativePos);

						if (length < 30)
						{		
							length = 30;
						}
						force = -glm::normalize(relativePos);
						double temp = mutualGrav.gravityVal / ((forceAccA.inverseMass * forceAccB.inverseMass) * pow(length, 2));
						force *= temp;
						particleList[i].getComponent<ForceAccumulatorComponent>().AddForce(force);
						getWorld().data.renderUtil->DrawLine(transformA.position, transformB.position, Color::Magenta);
					}
				}
			}
		}
	}
}
