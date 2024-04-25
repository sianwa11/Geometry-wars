#include "Entity.h"
#include<iostream>
#include<vector>
#include<map>
#include<memory>


class EntityManager
{
	//std::vector<Entity> m_entities;
	std::vector<std::shared_ptr<Entity>> m_entities;

	//std::vector<Entity> m_entitiesToAdd;
	std::vector<std::shared_ptr<Entity>> m_entitiesToAdd;

	std::map<std::string, std::vector<Entity>> m_entityMap;
	//size_t    m_totalEntities = 0;
	int    m_totalEntities = 0;


	//void removeDeadEntities(std::vector<Entity>& vec);
	void removeDeadEntities(std::vector<std::shared_ptr<Entity>>& vec);


public:
	EntityManager();

	void update();

	std::shared_ptr<Entity> addEntity(const std::string& tag);

	//const std::vector<Entity>& getEntities();
	const std::vector<std::shared_ptr<Entity>>& getEntities();


	const std::vector<Entity>& getEntities(const std::string& tag);
};