//#define STB_IMAGE_IMPLEMENTATION
#include "RenderingSystem.h"
#include "InputEventSystem.h"
#include "FPSControlSystem.h"
#include "RotateSystem.h"
#include "FireworksSystem.h"
#include "GravityForceSystem.h"
#include "DragForceSystem.h"
#include "FixedSpringSystem.h"
#include "PairedSpringSystem.h"
#include "MutualGravitySystem.h"
#include "SphereParticleSystem.h"
#include "ParticleContactSystem.h"
#include "BungeeParticleSystem.h"
#include "BuoyancySystem.h"

#include "ForceAccumulatorSystem.h"
#include "ParticleSystem.h"
#include "DynamicDirectionalLightSystem.h"
#include "DynamicPointLightSystem.h"
#include "DynamicSpotLightSystem.h"
#include <string>
#include <stdlib.h>     
#include <time.h>      

using namespace Reality;

void LoadShaders(ECSWorld& world);
void LoadModels(ECSWorld& world);
void SetupLights(ECSWorld& world);
void MakeABunchaObjects(ECSWorld& world);
void MakeFireworks(ECSWorld& world);
void Make3Particles(ECSWorld& world);
void MakeFixedSpring(ECSWorld& world);
void Make10Particles(ECSWorld& world);
void MakeABunchaSpheres(ECSWorld& world);
void MakeABungeeCord(ECSWorld& world);
void AttachABungeeCord(ECSWorld& world, int _bungeeCount, Vector3 _location);
void CreateBuoyancyObject(ECSWorld& world, float _waterDensity, Vector3 _location);
//void UpdateBuoyancyObject(ECSWorld& world);

ECSEntity *previousParticle;
int numOfBungee;
Vector3 location = Vector3(0, 0, 0);
float liquidDensity = 10.0f;

/*
	Change the following Booleans to change what simulation appears
	
	For the Bungee Cord Test:
	BuoyancyTest = false
	NBody = false

	For the Buoyancy Test:
	BuoyancyTest = true 
	NBody = false
	
	For the N-Body Test:
	BuoyancyTest = false
	NBody = true
*/

bool BuoyancyTest = false;
bool NBody = true;

int main()
{
	ECSWorld world;

	// Init and Load
	world.data.InitRendering();
	//LoadAssets(world);
	
	world.data.renderUtil->camera.Position = Vector3(0, 40.0f, 50.0f);
	world.data.renderUtil->SetFOV(60);
	// Create entities

	// Make a player controller
	auto e = world.createEntity();
	e.addComponent<FPSControlComponent>();

	SetupLights(world);
	//MakeABunchaObjects(world);
	//MakeFireworks(world);
	//Make3Particles(world);
	//MakeFixedSpring(world);
	if (NBody)
		Make10Particles(world);
	//MakeABunchaSpheres(world);
	if (BuoyancyTest)
		CreateBuoyancyObject(world, liquidDensity, location);
	else if (!NBody)
		MakeABungeeCord(world);

	// Create Systems
	world.getSystemManager().addSystem<RenderingSystem>();
	world.getSystemManager().addSystem<InputEventSystem>();
	world.getSystemManager().addSystem<FPSControlSystem>();
	world.getSystemManager().addSystem<RotateSystem>();
	world.getSystemManager().addSystem<FireworksSystem>();
	world.getSystemManager().addSystem<GravityForceSystem>();
	world.getSystemManager().addSystem<DragForceSystem>();
	world.getSystemManager().addSystem<FixedSpringSystem>();
	world.getSystemManager().addSystem<PairedSpringSystem>();
	world.getSystemManager().addSystem<MutualGravitySystem>();
	world.getSystemManager().addSystem<SphereParticleSystem>();
	world.getSystemManager().addSystem<ParticleContactSystem>();
	world.getSystemManager().addSystem<ForceAccumulatorSystem>();
	world.getSystemManager().addSystem<ParticleSystem>();
	world.getSystemManager().addSystem<DynamicDirectionalLightSystem>();
	world.getSystemManager().addSystem<DynamicPointLightSystem>();
	world.getSystemManager().addSystem<DynamicSpotLightSystem>();
	world.getSystemManager().addSystem<BungeeParticleSystem>();
	world.getSystemManager().addSystem<BuoyancySystem>();


	float time = glfwGetTime();
	float stepTime = glfwGetTime();
	float deltaTime = 0;
	float elapsedDeltaTime = 0;
	float logicDelta = 0;
	float debugDelta = 0;

	LoadShaders(world);
	bool shadersLoaded = false;
	bool modelsLoadStarted = false;
	bool pressed = false;

	// game loop
	// -----------
	while (!glfwWindowShouldClose(world.data.renderUtil->window->glfwWindow))
	{
		float current = glfwGetTime();
		deltaTime = current - time;
		deltaTime = 1 / 60.0f;
		time = glfwGetTime();

		world.update();

		// Poll OpenGl events
		glfwPollEvents();

		world.data.renderUtil->ClearDisplay(world.data.renderUtil->window->glfwWindow);

		// Load
		if (!shadersLoaded)
		{
			shadersLoaded = world.data.assetLoader->ShadersLoaded();
		}
		if(shadersLoaded && !modelsLoadStarted)
		{
			LoadModels(world);
			modelsLoadStarted = true;
		}
		// Update View
		world.data.renderUtil->UpdateViewMatrix();
		// Process Input
		world.getSystemManager().getSystem<InputEventSystem>().Update(deltaTime);

		// Game Logic Update
		world.getSystemManager().getSystem<FPSControlSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RotateSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<FireworksSystem>().Update(deltaTime);

		//world.getSystemManager().getSystem<SphereParticleSystem>().Update(deltaTime);

		// Update Transform

		// Physics
		//float fixedDeltaTime = glfwGetKey(world.data.renderUtil->window->glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS ? 1 / 60.0f : 0;		
		float fixedDeltaTime = 1 / 60.0f;
		// Force Generator
		world.getSystemManager().getSystem<GravityForceSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<DragForceSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<FixedSpringSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<PairedSpringSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<BungeeParticleSystem>().Update(fixedDeltaTime);
		world.getSystemManager().getSystem<BuoyancySystem>().Update(fixedDeltaTime, liquidDensity);

		world.getSystemManager().getSystem<MutualGravitySystem>().Update(fixedDeltaTime);

		// Force Accumulator
		world.getSystemManager().getSystem<ForceAccumulatorSystem>().Update(fixedDeltaTime);

		// Contact Resolution
		world.getSystemManager().getSystem<ParticleContactSystem>().Update(fixedDeltaTime);

		// Integrator
		if (BuoyancyTest)
		world.getSystemManager().getSystem<ParticleSystem>().Update(fixedDeltaTime, liquidDensity);
		else
		world.getSystemManager().getSystem<ParticleSystem>().Update(fixedDeltaTime, 1);

		// Rendering Update
		world.getSystemManager().getSystem<DynamicDirectionalLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<DynamicPointLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<DynamicSpotLightSystem>().Update(deltaTime);
		world.getSystemManager().getSystem<RenderingSystem>().Update(deltaTime);

		elapsedDeltaTime = glfwGetTime() - time;
		logicDelta = elapsedDeltaTime - world.data.renderUtil->GetRenderDelta();
		stepTime = glfwGetTime();

		// Debug
		if (DEBUG_LOG_LEVEL > 0)
		{
			world.data.renderUtil->RenderText("FPS : " + std::to_string((int)round(1.0f / deltaTime)), 1810.0f, 1060.0f, 0.5f, Color(0, 1, 1, 1));
		}
		if (DEBUG_LOG_LEVEL > 1)
		{
			int logic = (int)round(logicDelta * 100.0f / deltaTime);
			std::string logicString = logic < 10 ? " " + std::to_string(logic) : std::to_string(logic);
			int render = (int)round(world.data.renderUtil->GetRenderDelta() * 100.0f / deltaTime);
			std::string renderString = logic < 10 ? " " + std::to_string(render) : std::to_string(render);
			int debug = (int)round(debugDelta * 100.0f / deltaTime);
			std::string debugString = logic < 10 ? " " + std::to_string(debug) : std::to_string(debug);
			
			world.data.renderUtil->RenderText("Logic : " + logicString + "%" +
				//+ " | Physics : " + std::to_string((int)round(physicsDelta * 100.0f / deltaTime)) + "%" +
				+ " | Rendering : " + renderString + "%" +
				+ " | Debug : " + debugString + "%"
				, 1680.0f, 1040.0f, 0.25f, Color(0, 1, 1, 1));
		}
		if (DEBUG_LOG_LEVEL > 2)
		{
			world.data.renderUtil->RenderText("Draw Calls : " + std::to_string(world.data.renderUtil->GetDrawCalls())
				+ " | Verts : " + std::to_string(world.data.renderUtil->GetVerts())
				+ " | Tris : " + std::to_string(world.data.renderUtil->GetTris())
				+ " | Lines : " + std::to_string(world.data.renderUtil->GetLines())
				, 1610.0f, 1020.0f, 0.25f, Color(0, 1, 1, 1));
		}

		// Update debug delta
		debugDelta = glfwGetTime() - stepTime;
		stepTime = glfwGetTime();

		world.data.renderUtil->SwapBuffers(world.data.renderUtil->window->glfwWindow);

		// Show FPS in console
		//std::cout << "FPS : " << 1.0f / deltaTime << std::endl;
		if (!NBody)
		{
			if (glfwGetKey(world.data.renderUtil->window->glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
			{

				if (pressed && !BuoyancyTest)
				{
					location = world.data.renderUtil->camera.Position;
					location += world.data.renderUtil->camera.Front;
					AttachABungeeCord(world, numOfBungee, location);
				}
				else if (pressed && BuoyancyTest)
				{
					location = world.data.renderUtil->camera.Position;
					location += world.data.renderUtil->camera.Front;
					//location.y = 0;
					CreateBuoyancyObject(world, liquidDensity, location);
				}
				pressed = false;
			}

			if (glfwGetKey(world.data.renderUtil->window->glfwWindow, GLFW_KEY_SPACE) == GLFW_RELEASE)
			{
				pressed = true;
			}

			if (glfwGetKey(world.data.renderUtil->window->glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
			{
				liquidDensity += 0.1f;
				std::cout << liquidDensity << std::endl;
			}
			if (glfwGetKey(world.data.renderUtil->window->glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
			{
				liquidDensity -= 0.1f;
				std::cout << liquidDensity << std::endl;
			}
		}
		
}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

void LoadShaders(ECSWorld& world)
{
	world.data.assetLoader->StartShaderLoading({ {"Shaders/Lighting_Maps.vs", "Shaders/Lighting_Maps.fs"} });
}
void LoadModels(ECSWorld& world)
{
	world.data.assetLoader->StartModelLoading({
		//ModelData("Resources/Models/snowy-mountain-terrain/SnowyMountainMesh.obj"),
		ModelData("Resources/Models/Sponza-master/sponza.obj"),
		ModelData("Resources/Models/nanosuit/nanosuit.obj"),
		ModelData("Resources/Models/supermarine-spitfire/spitfire.fbx",
			{{"spitfire_d.png"}})
		});
}

void MakeABunchaObjects(ECSWorld& world)
{
	auto castle = world.createEntity();
	castle.addComponent<TransformComponent>(Vector3(0, -3.0f, 0.0f), Vector3(0.1f, 0.1f, 0.1f), Vector3(0, 270, 0));
	// Add mesh
	castle.addComponent<ModelComponent>("Resources/Models/Sponza-master/sponza.obj");

	//auto flight = world.createEntity();
	//flight.addComponent<TransformComponent>(Vector3(0, 30, -50), Vector3(0.1f, 0.1f, 0.1f), Vector3(270, 0, 0));
	//// Add mesh
	//flight.addComponent<ModelComponent>("Resources/Models/supermarine-spitfire/spitfire.fbx");
	//flight.addComponent<RotateComponent>(Vector3(0, 90, 0));
	//flight.addComponent<ParticleComponent>(Vector3(0, 30, 0));
	//flight.addComponent<ForceAccumulatorComponent>();
	//flight.addComponent<GravityForceComponent>();

}

void MakeFireworks(ECSWorld & world)
{
	for (int i = 0; i < 3; i++)
	{
		auto fireworks = world.createEntity();
		fireworks.addComponent<TransformComponent>(Vector3(-100 + 100 * i, 30 + RANDOM_FLOAT(-10, 10), -50));
		fireworks.addComponent<ParticleComponent>(Vector3(0, 100, 0));
		fireworks.addComponent<ForceAccumulatorComponent>();
		fireworks.addComponent<GravityForceComponent>();
		fireworks.addComponent<FireworksComponent>(6, 3, 3 + RANDOM_FLOAT(-1, 1));
	}
	
}

void Make3Particles(ECSWorld & world)
{
	auto particle1 = world.createEntity();
	particle1.addComponent<TransformComponent>(Vector3(-50, 30, -50));
	particle1.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle1.addComponent<ForceAccumulatorComponent>();
	particle1.addComponent<GravityForceComponent>();
	particle1.addComponent<DragForceComponent>(0,0);

	auto particle2 = world.createEntity();
	particle2.addComponent<TransformComponent>(Vector3(0, 30, -50));
	particle2.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle2.addComponent<ForceAccumulatorComponent>();
	particle2.addComponent<GravityForceComponent>();
	particle2.addComponent<DragForceComponent>(1,0);

	auto particle3 = world.createEntity();
	particle3.addComponent<TransformComponent>(Vector3(50, 30, -50));
	particle3.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle3.addComponent<ForceAccumulatorComponent>();
	particle3.addComponent<GravityForceComponent>();
	particle3.addComponent<DragForceComponent>(1,1);
}

void MakeFixedSpring(ECSWorld & world)
{

	auto particle1 = world.createEntity();
	particle1.addComponent<TransformComponent>(Vector3(0, 40, -50));
	particle1.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle1.addComponent<ForceAccumulatorComponent>();
	particle1.addComponent<GravityForceComponent>();

	auto particle2 = world.createEntity();
	particle2.addComponent<TransformComponent>(Vector3(-10, 20, -50));
	particle2.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle2.addComponent<ForceAccumulatorComponent>();
	particle2.addComponent<GravityForceComponent>();

	auto spring1 = world.createEntity();
	spring1.addComponent<TransformComponent>(Vector3(10, 60, -50));
	spring1.addComponent<FixedSpringComponent>(50, 20, particle1);

	auto spring2 = world.createEntity();
	spring2.addComponent<TransformComponent>(Vector3(-10, 60, -50));
	spring2.addComponent<FixedSpringComponent>(50, 20, particle1);

	auto pairedSpring = world.createEntity();
	pairedSpring.addComponent<PairedSpringComponent>(50, 15, particle1, particle2);
}

void Make10Particles(ECSWorld & world)
{
	auto particle1 = world.createEntity();
	particle1.addComponent<TransformComponent>(Vector3(30, 0, -50));
	particle1.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle1.addComponent<ForceAccumulatorComponent>(100);

	auto particle2 = world.createEntity();
	particle2.addComponent<TransformComponent>(Vector3(20, -10, -50));
	particle2.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle2.addComponent<ForceAccumulatorComponent>(200);

	auto particle3 = world.createEntity();
	particle3.addComponent<TransformComponent>(Vector3(10, -20, -50));
	particle3.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle3.addComponent<ForceAccumulatorComponent>(300);

	auto particle4 = world.createEntity();
	particle4.addComponent<TransformComponent>(Vector3(0, -30, -50));
	particle4.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle4.addComponent<ForceAccumulatorComponent>(400);

	auto particle5 = world.createEntity();
	particle5.addComponent<TransformComponent>(Vector3(-10, -20, -50));
	particle5.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle5.addComponent<ForceAccumulatorComponent>(500);

	auto particle6 = world.createEntity();
	particle6.addComponent<TransformComponent>(Vector3(-30, 0, -50));
	particle6.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle6.addComponent<ForceAccumulatorComponent>(600);

	auto particle7 = world.createEntity();
	particle7.addComponent<TransformComponent>(Vector3(-20, 10, -50));
	particle7.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle7.addComponent<ForceAccumulatorComponent>(700);

	auto particle8 = world.createEntity();
	particle8.addComponent<TransformComponent>(Vector3(-10, 20, -50));
	particle8.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle8.addComponent<ForceAccumulatorComponent>(800);

	auto particle9 = world.createEntity();
	particle9.addComponent<TransformComponent>(Vector3(10, 20, -50));
	particle9.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle9.addComponent<ForceAccumulatorComponent>(900);

	auto particle10 = world.createEntity();
	particle10.addComponent<TransformComponent>(Vector3(20, 10, -50));
	particle10.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle10.addComponent<ForceAccumulatorComponent>(1000);

	auto mutualGrav = world.createEntity();
	mutualGrav.addComponent<MutualGravityComponent>(0.05, 
		particle1, particle2, particle3,
		particle4, particle5, particle6,
		particle7, particle8, particle9,
		particle10);

}

void MakeABunchaSpheres(ECSWorld & world)
{
	auto sphere = world.createEntity();
	sphere.addComponent<TransformComponent>(Vector3(0,0,0));
	sphere.addComponent<ParticleComponent>(Vector3(RANDOM_FLOAT(-40, 40), RANDOM_FLOAT(-40, 40), RANDOM_FLOAT(-40, 40)));
	sphere.addComponent<ForceAccumulatorComponent>(1.0f);
	sphere.addComponent<GravityForceComponent>();
	sphere.addComponent<SphereParticleComponent>(3);

	//auto cube = world.createEntity();
}

void MakeABungeeCord(ECSWorld& world)
{
	auto particle1 = world.createEntity();
	particle1.addComponent<TransformComponent>(Vector3(0, 0, 0));
	particle1.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	//particle1.addComponent<ForceAccumulatorComponent>();
	//particle1.addComponent<GravityForceComponent>();

	auto particle2 = world.createEntity();
	particle2.addComponent<TransformComponent>(Vector3(0, -5, 0));
	particle2.addComponent<ParticleComponent>(Vector3(0, 0, 0));
	particle2.addComponent<ForceAccumulatorComponent>();
	particle2.addComponent<GravityForceComponent>();

	auto pairedSpring = world.createEntity();
	pairedSpring.addComponent<BungeeParticleComponent>(50, 15, particle1, particle2);
	pairedSpring.addComponent<TransformComponent>(Vector3(0, 0, 0));
	pairedSpring.addComponent<GravityForceComponent>();
	numOfBungee = 0;
	previousParticle = &pairedSpring;
	particle2.tag(to_string(numOfBungee));

}

void AttachABungeeCord(ECSWorld& world, int _numOfBungee, Vector3 _location)
{

	auto previousBungee = world.getEntity(to_string(_numOfBungee));

	auto particle = world.createEntity();
	particle.addComponent<TransformComponent>(_location);
	particle.addComponent<ParticleComponent>(_location);
	particle.addComponent<ForceAccumulatorComponent>();
	particle.addComponent<GravityForceComponent>();

	auto spring = world.createEntity();
	spring.addComponent<TransformComponent>(Vector3(world.getEntity(to_string(_numOfBungee)).getComponent<TransformComponent>().position)); //
	spring.addComponent<BungeeParticleComponent>(10.0f, 10.0f, previousBungee, particle);
	spring.addComponent<GravityForceComponent>();
	
	//previousParticle = &spring;
	numOfBungee++;
	particle.tag(to_string(numOfBungee));
}

void CreateBuoyancyObject(ECSWorld& world, float _waterDensity, Vector3 _location)
{
	auto particle = world.createEntity();
	particle.addComponent<TransformComponent>(_location);
	particle.addComponent<ParticleComponent>(_location);
	particle.addComponent<ForceAccumulatorComponent>();
	particle.addComponent<BuoyancyComponent>(4.0f, 1.0f, 3.0f, liquidDensity, particle);
}


void SetupLights(ECSWorld& world)
{
	auto l = world.createEntity();
	l.addComponent<TransformComponent>(Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(90, 0, 0));
	l.addComponent<DynamicDirectionalLightComponent>(Color(0.0, 0.1, 0.1), Color(0.0, 0.1, 0.1), Color(0.0, 0.1, 0.1));

	// Lanterns
	auto pl1 = world.createEntity();
	pl1.addComponent<TransformComponent>(Vector3(22, 14, 48.5f));
	pl1.addComponent<DynamicPointLightComponent>(100.0f, Color(0.1, 0, 0), Color(1.0f, 0.0f, 0.0f), Color(1.0f, 0.0f, 0.0f));
	auto hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(23, 15, 48.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22, 13.5f, 50.5f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(21, 12.5f, 47.5f));

	auto pl2 = world.createEntity();
	pl2.addComponent<TransformComponent>(Vector3(-14.5f, 14, 49.0f));
	pl2.addComponent<DynamicPointLightComponent>(100.0f, Color(0, 0, 0.1f), Color(0.0f, 0.0f, 1.0f), Color(0.0f, 0.0f, 1.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f + 1, 14 - 1, 49.0f - 1));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 0.5f, 14 + 1, 49.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f, 14 - 1, 49.0f + 1));
	
	auto pl3 = world.createEntity();
	pl3.addComponent<TransformComponent>(Vector3(22, 14, -62.0f));
	pl3.addComponent<DynamicPointLightComponent>(100.0f, Color(0, 0.1f, 0), Color(0.0f, 1.0f, 0.0f), Color(0.0f, 1.0f, 0.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22 - 1, 14 - 1, -62.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22, 14 + 0.5f, -62.0f - 1));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(22 + 1, 14, -62.0f + 0.5f));

	auto pl4 = world.createEntity();
	pl4.addComponent<TransformComponent>(Vector3(-14.5f, 14, -61.5f));
	pl4.addComponent<DynamicPointLightComponent>(100.0f, Color(0.1, 0.05, 0), Color(1.0f, 0.55f, 0.0f), Color(1.0f, 0.55f, 0.0f));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 1, 14, -61.5f -1));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f - 0.25f, 14 - 0.5f, -61.5f + 1));
	hook = world.createEntity();
	hook.addComponent<TransformComponent>(Vector3(-14.5f + 0.5f, 14+ 1, -61.5f + 1));

	// Spears
	std::vector<Color> cols = { Color(1,0,0), Color(0,1,0), Color(0,0,1), Color(0.7f,0.55f,0) };
	for (int i = 1; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			pl1 = world.createEntity();
			pl1.addComponent<TransformComponent>(Vector3((i % 2 == 0 ? 8 : -1), 85, 49.5f - 37 * j), Vector3(1, 1, 1), Vector3(180, 0, 0));
			pl1.addComponent<DynamicSpotLightComponent>(10.0f, 100, Color(0, 0, 0), cols[3 - j], cols[3 - j], 5);
		}
	}
}