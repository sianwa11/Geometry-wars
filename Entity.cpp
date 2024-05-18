
#include "Entity.h"


Entity::Entity(int id, const std::string& tag)
	: m_id(id), m_tag(tag) // Initializing member variables in the constructor initializer list
{
	// Initialize other members as needed
}

bool Entity::isActive() const
{
	return m_active;
}

const std::string& Entity::getTag() const
{
	return m_tag; // Return member variable
}

const size_t Entity::id() const
{
	return m_id;
}

void Entity::destroy()
{
	// Perform destruction tasks here
	m_active = false;
}

