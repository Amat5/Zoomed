#include "GameObject.h"

GameObject::GameObject(std::string m_objectName) : m_bStatic(false),
	isAlive(true),
	isVisible(true),
	isCollidable(true),
	isLightAffected(false),
	isFogAffected(false),
	makesNoise(false),
	boundingbox(0.0f,0.0f,0.0f, 0.0f,0.0f,0.0f),
	mass(0.0f),
	m_objectPosition(0,0,0),
	m_objectVelocity(0,0,0)
{
	this->setName(m_objectName);

	textures.push_back("Images//unknown.tga");
}

GameObject::~GameObject()
{
	//todo delete object
}

BoundingBox & GameObject::getBoundingBox()
{
	return this->boundingbox;
}

void GameObject::setStatic(bool status)
{
	this->m_bStatic = status;
}

bool GameObject::isStatic()
{
	return m_bStatic;
}

void GameObject::setPosition(Vector3 pos)
{
	this->m_objectPosition.x = pos.x;
	this->m_objectPosition.y = pos.y;
	this->m_objectPosition.z = pos.z;
}

Vector3 GameObject::getPosition()
{
	return this->m_objectPosition;
}

void GameObject::setVelocity(Vector3 vel)
{
	this->m_objectVelocity.x = vel.x;
	this->m_objectVelocity.y = vel.y;
	this->m_objectVelocity.z = vel.z;
}

Vector3 GameObject::getVelocity()
{
	return this->m_objectVelocity;
}