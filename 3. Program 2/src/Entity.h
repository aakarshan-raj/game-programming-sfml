#pragma once

#include "Components.h"
#include <memory>

class Entity
{
private:
	friend class EntityManager;

	size_t			m_id		{ 0 };
	std::string     m_tag 	= "default";
	bool			m_active	{ true };

	Entity(size_t id, const std::string& tag);

public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CScore>		cScore;
	std::shared_ptr<CInput>		cInput;
	std::shared_ptr<CLifeSpan>	cLifespan;
	std::shared_ptr<CShape>		cShape;

public:
	bool isActive() const;
	const std::string& tag() const;
	const size_t id() const;
	void destroy();
};

