#include "PowerUpBase.h"
#include "MainGameScene.h"

USING_NS_CC;

PowerUpBase::PowerUpBase(void)
{
	this->scheduleUpdate();
	active = false;
}


PowerUpBase::~PowerUpBase(void)
{
	
}

void PowerUpBase::update(float dt)
{
	// check if the user pressed the space button
	if (KeyboardInput::GetKey(CC_KEY_SPACE))
	{
		// executes the power up code
		this->Execute();
		active = true;
	}
}

void PowerUpBase::DeletePowerUpCallback()
{
	((MainGameScene*)this->getParent())->SetPoweUpToNull();
	this->getParent()->removeChild(this,true);
}