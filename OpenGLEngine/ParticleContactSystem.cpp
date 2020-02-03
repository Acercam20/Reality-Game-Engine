#include "ParticleContactSystem.h"
#include "ParticleComponent.h"
#include "ForceAccumulatorComponent.h"

namespace Reality
{
	ParticleContactSystem::ParticleContactSystem()
	{

	}

	void ParticleContactSystem::Update(float deltaTime)
	{
		auto contactEvents = getWorld().getEventManager().getEvents<ParticleContactEvent>();
		for (auto& contact : contactEvents)
		{
			ResolveVelocity(contact, deltaTime);
		}
	}
	float ParticleContactSystem::CalculateSeperatingVelocity(ParticleContactEvent & contact)
	{
		Vector3 velocityA = contact.entityA.hasComponent<ParticleComponent>() ? contact.entityA.getComponent<ParticleComponent>().velocity : Vector3(0, 0, 0);
		Vector3 velocityB = contact.entityB.hasComponent<ParticleComponent>() ? contact.entityB.getComponent<ParticleComponent>().velocity : Vector3(0, 0, 0);

		Vector3 relativeVelocity = velocityA - velocityB;

		return glm::dot(relativeVelocity, contact.normal);
	}
	void ParticleContactSystem::ResolveVelocity(ParticleContactEvent & contact, float deltaTime)
	{
		float initialSeperatingVelocity = CalculateSeperatingVelocity(contact);

		if (initialSeperatingVelocity >= 0)
		{
			return;
		}

		float finalSeperatingVelocity = -contact.restitution * initialSeperatingVelocity;
		float deltaVelocity = finalSeperatingVelocity - initialSeperatingVelocity;

		float invMA = contact.entityA.hasComponent<ForceAccumulatorComponent>() ? contact.entityA.getComponent<ForceAccumulatorComponent>().inverseMass : 0;
		float invMB = contact.entityB.hasComponent<ForceAccumulatorComponent>() ? contact.entityB.getComponent<ForceAccumulatorComponent>().inverseMass : 0;

		float totalInverseMass = invMA + invMB;

		if (totalInverseMass <= 0)
		{
			return;
		}

		float impulse = deltaVelocity / totalInverseMass;
		Vector3 impulsePerUnitMass = impulse * contact.normal;

		if (contact.entityA.hasComponent<ParticleComponent>())
		{
			contact.entityA.getComponent<ParticleComponent>().velocity += impulsePerUnitMass * invMA;
		}
		if (contact.entityB.hasComponent<ParticleComponent>())
		{
			contact.entityB.getComponent<ParticleComponent>().velocity -= impulsePerUnitMass * invMB;
		}
	}
}
