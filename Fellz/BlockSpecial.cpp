#include "BlockSpecial.h"

USING_NS_CC;

BlockSpecial::BlockSpecial(int kind) //: PowerUpBase()
{
	this->kind = kind;
	// put sprite on power up zone
	CCSprite* sprite = CCSprite::create("Assets/redSpecial.png");
	sprite->setPosition(ccp(730.0f,520.0f));
	this->addChild(sprite);
}


BlockSpecial::~BlockSpecial(void)
{
}

BlockSpecial* BlockSpecial::create(int kind)
{
	BlockSpecial * pRet = new BlockSpecial(kind);
	pRet->autorelease();
	return pRet;
}

void BlockSpecial::Execute()
{
		
}
