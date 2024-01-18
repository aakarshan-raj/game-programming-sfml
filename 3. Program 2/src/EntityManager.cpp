#include "EntityManager.h"
#include <iostream>

EntityManager::EntityManager()
	: m_totalEntities(0)
{
}

void EntityManager::removeDeadEntities(EntityVec &vec)
{
	vec.erase(std::remove_if(vec.begin(), vec.end(),
							 [](std::shared_ptr<Entity> e)
							 {
								 return e->m_active == false;
							 }),
			  vec.end());
}

void EntityManager::update()
{
	for (auto &e : m_toAdd)
	{
		m_entities.push_back(e);
	}

	for (auto &e : m_toAdd)
	{
		m_entityMap[e->m_tag].push_back(e);
	}
	removeDeadEntities(m_entities);
	for (auto &[key, value] : m_entityMap)
	{
		removeDeadEntities(value);
	}
	m_toAdd.clear();
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{
	std::shared_ptr<Entity> e = std::shared_ptr<Entity>(new Entity(m_totalEntities++,tag));

	m_toAdd.push_back(e);
	return e;
}

const EntityVec &EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec &EntityManager::getEntities(const std::string &tag)
{
	return m_entityMap[tag];
}

