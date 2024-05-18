#include "EntityManager.h"
#include <algorithm>

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
		m_entityMap[e->getTag()].push_back(e);
	}

	m_entitiesToAdd.clear();

	// remove dead entities from the vector of all entities
	removeDeadEntities(m_entities);

	// remove dead entities from each vector in the entity map
	// C++17 way of iterating through [key,value] pairs in a map
	for (auto& pair : m_entityMap)
	{
		const std::string& tag = pair.first;
		EntityVec& entityVec = pair.second;

		removeDeadEntities(entityVec);
	}

}
void EntityManager::removeDeadEntities(EntityVec& vec)

{
	// TODO: remove all dead entities from the input vector
	//       this is called by the update() function
	//       look into std::remove_if*


	auto newEnd = std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<Entity>& e) {
		return !e->isActive();
	});

	vec.erase(newEnd, vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	// Create a new Entity Object
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	// Store it in vector of all entities
	m_entitiesToAdd.push_back(entity);
	// Store it in map of tag->entityvector *
	//m_entityMap.insert(std::make_pair(tag, m_entities));
	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	// TODO: this is incorrect, return the correct vector from the map
	return m_entityMap[tag];
}
