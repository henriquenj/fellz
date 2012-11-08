#include "BlockSpecial.h"

USING_NS_CC;

BlockSpecial::BlockSpecial(int kind) //: PowerUpBase()
{
	this->kind = kind;
	// put sprite on power up zone
	CCSprite* sprite = CCSprite::create("Assets/BlockSpecial.png");
	// put the proper color
	if (kind == SPECIAL_BLUE){sprite->setColor(ccc3(0,0,255));}
	else if (kind == SPECIAL_GREEN){sprite->setColor(ccc3(0,255,0));}
	else if (kind == SPECIAL_RED){sprite->setColor(ccc3(255,0,0));}
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
	float time = 0.2f;

	//iterate through the list
	std::list<ColorBlock*>::iterator it;
	for (it = (*blocksList).begin();it != (*blocksList).end(); it++)
	{
		// if the block is the sabe kind of the special, anihilated
		if ((*it)->GetBlockColor() == kind)
		{
			CCSequence* sequence = CCSequence::create(CCDelayTime::create(time),CCCallFuncO::create(this,callfuncO_selector(BlockSpecial::DeleteBlockCallback),*it));
			this->runAction(sequence);
			time += 0.1f;
		}
	}

	// schedule the powerup deletation when the processing have finished
	CCSequence* deletation = CCSequence::create(CCDelayTime::create(time),CCCallFunc::create(this,callfunc_selector(PowerUpBase::DeletePowerUpCallback)));
	this->runAction(deletation);
}


void BlockSpecial::DeleteBlockCallback(CCObject* block)
{
	if (!((ColorBlock*)block)->GetDying())
	{
		// draw points
		pointsManager->AnihilationHappened(1,((ColorBlock*)block)->getPosition());
		// destroy block
		((ColorBlock*)block)->Destroy();
		// put some particles
		// get to the mains game scene
		MainGameScene* mainScene = (MainGameScene*)pointsManager->getParent();
		// create particle system
		CCParticleSystem* system = CCParticleFire::create();
		system->setTexture(CCTextureCache::sharedTextureCache()->addImage("Assets/fire.png"));
		system->setAutoRemoveOnFinish(true);
		system->setPosition(((ColorBlock*)block)->getPosition());
		system->setDuration(0.3f);
		system->setLife(0.5f);
		mainScene->addChild(system);
	}
}