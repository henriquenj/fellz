#include "WarningSign.h"

#include "MainGameScene.h"
USING_NS_CC;

WarningSign::WarningSign(std::list<ColorBlock*> *blocksList)
{
	// load the assets and put into node
	// all have 0 opacity at first

	leftCircularSignal = CCSprite::create("Assets/warning_sign_1.png");
	leftCircularSignal->setOpacity(0);
	this->addChild(leftCircularSignal);

	rightCircularSignal = CCSprite::create("Assets/warning_sign_1.png");
	rightCircularSignal->setOpacity(0);
	this->addChild(rightCircularSignal);

	topCircularSignal = CCSprite::create("Assets/warning_sign_1.png");
	topCircularSignal->setOpacity(0);
	this->addChild(topCircularSignal);

	botCircularSignal = CCSprite::create("Assets/warning_sign_1.png");
	botCircularSignal->setOpacity(0);
	this->addChild(botCircularSignal);

	leftSignal = CCSprite::create("Assets/warning_sign_2.png");
	leftSignal->setOpacity(0);
	this->addChild(leftSignal);

	rightSignal = CCSprite::create("Assets/warning_sign_2.png");
	rightSignal->setOpacity(0);
	this->addChild(rightSignal);

	botSignal = CCSprite::create("Assets/warning_sign_2.png");
	botSignal->setOpacity(0);
	this->addChild(botSignal);

	topSignal = CCSprite::create("Assets/warning_sign_2.png");
	topSignal->setOpacity(0);
	this->addChild(topSignal);

	// init with all false
	left = bot = right = top = false;

	this->scheduleUpdate();

	leftMost = botMost = 900;
	topMost = rightMost = -900;
	topBlock = leftBlock = rightBlock = botBlock = NULL;

	this->blocksList = blocksList;

}


WarningSign::~WarningSign(void)
{
}

WarningSign* WarningSign::create(std::list<ColorBlock*> *blocksList)
{
	WarningSign * pRet = new WarningSign(blocksList);
	pRet->autorelease();
	return pRet;
}

void WarningSign::update(float dt)
{
	// this funcion checks if the blocks still exists
	// and keep tracking of the ones in danger areas
	bool topExists, botExists, leftExists, rightExists;
	topExists = botExists = leftExists = rightExists = false;

	CCPoint location_c;
	const Character* character = ((MainGameScene*)getParent())->GetCharcter();
	location_c = character->GetPosition();

	// must go through all the list
	std::list<ColorBlock*>::iterator it;
	for (it = blocksList->begin(); it != blocksList->end(); it++)
	{
		if((*it)->GetAttached())
		{

			if ((*it) == topBlock)
			{
				// can process topblock
				topExists = true;
				continue;
			}
			if ((*it) == botBlock)
			{
				// can process topblock
				botExists = true;
				continue;
			}
			if ((*it) == leftBlock)
			{
				// can process topblock
				leftExists = true;
				continue;
			}
			if ((*it) == rightBlock)
			{
				// can process topblock
				rightExists = true;
				continue;
			}			
		}
	}

	// now we can check

	// top
	if (topExists == true)
	{
		if (topBlock->getPosition().y < 550.0f)
		{
			// out of danger
			top = false;
			topBlock = NULL;
		}
	}
	else if (top == true && topBlock == NULL) // this menas the character
	{
		if (location_c.y < 450.0f)
		{
			// out of danger
			top = false;
			topBlock = NULL;
		}
	}
	else 
	{
		// mark as out of danger zone
		// there's no more block
		top = false;
		topBlock = NULL;
	}


	//////////////////////////////////////////////////
	// bot
	if (botExists == true)
	{
		if (botBlock->getPosition().y > 50.0f)
		{
			// out of danger
			bot = false;
			botBlock = NULL;
		}
	}
	else if (bot == true && botBlock == NULL) // this menas the character
	{
		if (location_c.y > 100.0f)
		{
			// out of danger
			bot = false;
			botBlock = NULL;
		}
	}
	else 
	{
		// mark as out of danger zone
		// there's no more block
		bot = false;
		botBlock = NULL;
	}
	/////////////////////////////////////////////////////////////////////////

	if (top == false) 
	{
		topCircularSignal->setOpacity(0);
		topSignal->setOpacity(0);
	}
	if (bot == false)
	{
		botCircularSignal->setOpacity(0);
		botSignal->setOpacity(0);
	}
}

void WarningSign::ShowWarn(ColorBlock* block)
{
	CCPoint location;
	int aditional = 0; // to compute aditional coordenates for character sprite
	// it's the character
	if (block == NULL)
	{
		// get character object
		const Character* character = ((MainGameScene*)getParent())->GetCharcter();
		location = character->GetPosition();
		aditional = 50;
	}
	else
	{
		//it's a block
		location = block->getPosition();
	}

	//(*it)->getPositionX() < 30.0f || (*it)->getPositionX() > 700.0f || 
	//(*it)->getPositionY() > 500.0f || (*it)->getPositionY() < 30.0f

	//see which side is the problem
	if ((location.x - aditional) < 30.0f)
	{
		// left
		// check if it's the left most so far
		if ((location.x - aditional) < leftMost)
		{
			// ok, put left sprite in that location
			leftCircularSignal->setPosition(ccp(30.0f,location.y));
			leftSignal->setPosition(ccp(30.0f,location.y));
			// if it's the first time, change opacity
			if (left == false)
			{
				leftCircularSignal->setOpacity(255);
				leftSignal->setOpacity(255);
				left = true;
			}
			// update block reference
			leftBlock = block;
		}
	}
	else if ((location.x + aditional) > 700.0f)
	{
		// right
		// check if it's the right most so far
		if ((location.x + aditional) > rightMost)
		{
			// ok, put right sprite in that location
			rightCircularSignal->setPosition(ccp(700.0f,location.y));
			rightSignal->setPosition(ccp(700.0f,location.y));
			// if it's the first time, change opacity
			if (right == false)
			{
				rightCircularSignal->setOpacity(255);
				rightSignal->setOpacity(255);
				right = true;
			}
			// update block reference
			rightBlock = block;
		}
	}
	else if ((location.y + aditional) > 500.0f)
	{
		// top
		// check if it's the top most so far
		if ((location.y + aditional) > topMost)
		{
			// ok, put right sprite in that location
			topCircularSignal->setPosition(ccp(location.x,550.0f));
			topSignal->setPosition(ccp(location.x,550.0f));
			// if it's the first time, change opacity
			if (top == false)
			{
				topCircularSignal->setOpacity(255);
				topSignal->setOpacity(255);
				top = true;
			}
			// update block reference
			topBlock = block;
		}
	}
	else if ((location.y - aditional) < 100.0f)
	{
		// bot
		// check if it's the bot most so far
		if ((location.y - aditional) < botMost)
		{
			// ok, put bot sprite in that location
			botCircularSignal->setPosition(ccp(location.x,30.0f));
			botSignal->setPosition(ccp(location.x,30.0f));
			// if it's the first time, change opacity
			if (bot == false)
			{
				botCircularSignal->setOpacity(255);
				botSignal->setOpacity(255);
				bot = true;
			}
			// update block reference
			botBlock = block;
		}
	}
}