#include "EntityManager.h"

EntityManager::EntityManager()
{

}

void EntityManager::update()
{
	// TODO: add entities from m_entittiesToAdd the proper location(s)
	//       - add them to the vector of all entities
	//       - add them to the vector inside the map, with the tag as key
	for (auto& e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
	}

	m_entitiesToAdd.clear();

	// remove dead entities from the vector of all entities
	removeDeadEntities(m_entities);

	// remove dead entities from each vector in the entity map
	// C++17 way of iterating through [key,value] pairs in a map
	for (auto& pair : m_entityMap)
	{
		const std::string& tag = pair.first;
		std::vector<Entity>& entityVec = pair.second;

		// Convert std::vector<Entity> to std::vector<std::shared_ptr<Entity>>
		std::vector<std::shared_ptr<Entity>> sharedPtrVec;
		for (auto& entity : entityVec)
		{
			sharedPtrVec.push_back(std::make_shared<Entity>(entity));
		}

		removeDeadEntities(sharedPtrVec);
	}

}
//void EntityManager::removeDeadEntities(std::vector<Entity>& vec)
void EntityManager::removeDeadEntities(std::vector<std::shared_ptr<Entity>>& vec)

{
	// TODO: remove all dead entities from the input vector
	//       this is called by the update() function
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

	m_entitiesToAdd.push_back(entity);

	return entity;
}


//std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
//{
//	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
//
//	 m_entitiesToAdd.push_back(entity);
//
//	 return entity;
//}

//const std::vector<Entity>& EntityManager::getEntities()
const std::vector<std::shared_ptr<Entity>>& EntityManager::getEntities()

{
	return m_entities;
}

const std::vector<Entity>& EntityManager::getEntities(const std::string& tag)
{
	// TODO: this is incorrect, return the correct vector from the map
	return m_entityMap[tag];
}
