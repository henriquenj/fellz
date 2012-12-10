#include "ChangeScreenSpecial.h"

USING_NS_CC;
ChangeScreenSpecial::ChangeScreenSpecial(void)
{
	// put sprite on power up zone
	CCSprite* sprite = CCSprite::create("Assets/change_special.png");
	sprite->setPosition(ccp(730.0f,520.0f));
	
	this->addChild(sprite);
}
ChangeScreenSpecial* ChangeScreenSpecial::create()
{
	ChangeScreenSpecial * pRet = new ChangeScreenSpecial();
	pRet->autorelease();
	return pRet;
}

ChangeScreenSpecial::~ChangeScreenSpecial(void)
{
}

void ChangeScreenSpecial::Execute()
{
	active = false;
	// only perform the swap if it's connected AND the other game is still running
	if (isConnected)
	{
		if (!otherGameOver)
		{
			active = true;
		}
	}
}
