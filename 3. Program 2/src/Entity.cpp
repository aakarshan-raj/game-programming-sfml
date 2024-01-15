#include "Entity.h"

Entity::Entity(size_t id, const std::string& tag)
	: m_id(id),
	m_tag(tag)
{
}

void Entity::destroy()
{
}

bool Entity::isActive() const
{
}

const std::string&  Entity::tag() const
{
}

const size_t Entity::id() const
{
}