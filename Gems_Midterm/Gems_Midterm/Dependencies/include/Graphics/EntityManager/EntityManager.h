#pragma once

#include <unordered_map>
#include <string>
#include "Entity.h"

class EntityManager
{
private:
	std::vector<std::string> entityToRemove;
	std::unordered_map<std::string, Entity*>::iterator it;

public:
	std::unordered_map<std::string, Entity*> listOfEntities;

	static EntityManager& GetInstance();

	void AddEntity(const std::string& entityId, Entity* entity);
	void AddEntity(Entity* entity);
	void RemoveEntity(const std::string& entityId);
	bool ShouldRemove(const std::string& entityId);

	void Update(float deltaTime);
	void Render();
	void Destroy(Entity* entity);
	void Clear();
};

