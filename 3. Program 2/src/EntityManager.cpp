#include "EntityManager.h"

EntityManager::EntityManager()
	: m_totalEntities(0)
{
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	// remove from vector and map all dead entity
}


void EntityManager::update()
{
	
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	// set up a entity
	// add to vector
	// return the entity
	
}

const EntityVec& EntityManager::getEntities()
{
	
}

const EntityVec& EntityManager::getEntities(const std::string & tag)
{
	
