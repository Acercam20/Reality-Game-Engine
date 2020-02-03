#include "SphereParticleSystem.h"
#include "ParticleContactEvent.h"

namespace Reality
{
	SphereParticleSystem::SphereParticleSystem()
	{
		requireComponent<TransformComponent>();
		requireComponent<SphereParticleComponent>();
	}

	void SphereParticleSystem::Update(float deltaTime)
	{
		if (!createWall)
		{
			wallDummy = getWorld().createEntity();
		}
		getWorld().data.renderUtil->DrawCube(Vector3(0, 0, 0), Vector3(20, 20, 20), Vector3(0, 0, 0), Color::Red);
		for (auto e : getEntities())
		{
			auto& transform = e.getComponent<TransformComponent>();
			auto& sphere = e.getComponent<SphereParticleComponent>();
			if (DEBUG_LOG_LEVEL > 0)
			{
				getWorld().data.renderUtil->DrawSphere(transform.position,sphere.radius,Color::Orange);
			}

			//COllision with Y
			if (abs(transform.position.y) + sphere.radius >= 10)
			{
				Vector3 normal = Vector3(0, transform.position.y > 0 ? -1 : 1, 0);
				float penetration = abs(transform.position.y) + sphere.radius - 10;
				getWorld().getEventManager().emitEvent<ParticleContactEvent>(e,wallDummy,1.0f,normal,penetration);
				getWorld().data.renderUtil->DrawLine(transform.position - normal * sphere.radius, transform.position - normal * (sphere.radius - penetration), Color::Red);
			}

			//COllision with X
			if (abs(transform.position.x) + sphere.radius >= 10)
			{
				Vector3 normal = Vector3(transform.position.x > 0 ? -1 : 1,0, 0);
				float penetration = abs(transform.position.x) + sphere.radius - 10;
				getWorld().getEventManager().emitEvent<ParticleContactEvent>(e, wallDummy, 1.0f, normal, penetration);
				getWorld().data.renderUtil->DrawLine(transform.position - normal * sphere.radius, transform.position - normal * (sphere.radius - penetration), Color::Red);
			}

			//COllision with Z
			if (abs(transform.position.z) + sphere.radius >= 10)
			{
				Vector3 normal = Vector3(0,0, transform.position.z > 0 ? -1 : 1);
				float penetration = abs(transform.position.z) + sphere.radius - 10;
				getWorld().getEventManager().emitEvent<ParticleContactEvent>(e, wallDummy, 1.0f, normal, penetration);
				getWorld().data.renderUtil->DrawLine(transform.position - normal * sphere.radius, transform.position - normal * (sphere.radius - penetration), Color::Red);
			}
		}
	}
}
