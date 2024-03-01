#pragma once

#include "../Object.h"
#include "../Transform.h"

class EntityManager;

class Entity : public Object
{

public:

	virtual ~Entity() {};
	virtual void InitializeEntity(Entity* entity);
	virtual void Start() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
	virtual void OnDestroy() = 0;

	virtual void OnPropertyDraw();
	virtual void OnSceneDraw();

	virtual void Destroy();

	Transform transform;

	std::string entityId;
	std::string tag = "Untagged";
	std::string name = "UnNamed";

	bool isStartInvoked = false;

};

