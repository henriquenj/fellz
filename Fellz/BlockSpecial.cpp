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
	std::list<ColorBlock*> *blocksList = ((MainGameScene*)this->getParent())->GetBlockList();

	// schedule the deletation with a little bit of difference in time
	float time = 0.2;

	//iterate through the list
	std::list<ColorBlock*>::iterator it;
	for (it = (*blocksList).begin();it != (*blocksList).end(); it++)
	{
		// if the block is the sabe kind of the special, anihilated
		if ((*it)->GetBlockColor() == kind)
		{
			CCSequence* sequence = CCSequence::create(CCDelayTime::create(time),CCCallFuncO::create(this,callfuncO_selector(BlockSpecial::DeleteBlockCallback),*it));
			this->runAction(sequence);
			time += 0.1;

		}
	}
}


void BlockSpecial::DeleteBlockCallback(CCObject* block)
{
	if (!((ColorBlock*)block)->GetDying())
	{
		// draw points
		pointsManager->AnihilationHappened(2,((ColorBlock*)block)->getPosition());
		// destroy block
		((ColorBlock*)block)->Destroy();
	}
}