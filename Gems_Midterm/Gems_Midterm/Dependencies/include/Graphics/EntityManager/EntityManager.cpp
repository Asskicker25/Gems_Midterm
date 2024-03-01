#include "EntityManager.h"
#include "../Renderer.h"

EntityManager& EntityManager::GetInstance()
{
	static EntityManager instance;
	return instance;
}

void EntityManager::AddEntity(const std::string& entityId, Entity* entity)
{
	listOfEntities[entityId] = entity;
	entity->entityId = entityId;
	//entity->entityManager = this;
}

void EntityManager::AddEntity(Entity* entity)
{
	std::unordered_map<std::string, Entity*>::iterator it;

	for (it = listOfEntities.begin(); it != listOfEntities.end(); it++)
	{
		if (it->second == entity) return;
	}

	std::string entityId = std::to_string((int)listOfEntities.size());
	entity->entityId = entityId;

	listOfEntities[entityId] = entity;
}

void EntityManager::RemoveEntity(const std::string& entityId)
{
	for (it = listOfEntities.begin(); it != listOfEntities.end(); ++it)
	{
		if (it->first == entityId)
		{
			Destroy(listOfEntities[entityId]);

			entityToRemove.push_back(entityId);

			return;
		}
	}
}

bool EntityManager::ShouldRemove(const std::string& entityId)
{
	for (const std::string& id : entityToRemove)
	{
		if (id == entityId)
		{
			return true;
		}
	}
	return false;
}

void EntityManager::Update(float deltaTime)
{
	for (const std::string& id : entityToRemove)
	{
		listOfEntities[id] = nullptr;

		delete listOfEntities[id];


		listOfEntities.erase(id);
	}

	entityToRemove.clear();

	try
	{
		for (std::pair<const std::string&, Entity*> item : listOfEntities)
		{
			if (item.second->isEnabled)
			{
				if (!item.second->isStartInvoked)
				{
					item.second->Start();
					item.second->isStartInvoked = true;
				}

				item.second->Update(deltaTime);

			}
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

}

void EntityManager::Render()
{
	try
	{
		for (std::pair<const std::string&, Entity*> item : listOfEntities)
		{

			item.second->Render();
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}
}

void EntityManager::Destroy(Entity* entity)
{
	entity->OnDestroy();
}

void EntityManager::Clear()
{
	std::unordered_map<std::string, Entity*>::iterator it;

	for (it = listOfEntities.begin(); it != listOfEntities.end(); ++it)
	{
		delete it->second;
	}

	listOfEntities.clear();
}

