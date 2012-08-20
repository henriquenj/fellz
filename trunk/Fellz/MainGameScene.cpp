#include "MainGameScene.h"

USING_NS_CC;

CCScene* MainGameScene::scene()
{
	// alloc scene 
	
	// 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
	MainGameScene *layer = MainGameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

MainGameScene::~MainGameScene()
{

}

bool MainGameScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	// load asset and put in the right spot
	mainCharacter = CCSprite::create("Assets/meat_boy_by_barakaldo-d3apehs.png");
	mainCharacter->setPosition(ccp(400.0f,500.0f));
	this->addChild(mainCharacter);

	// put the update method to work
	this->scheduleUpdate();

	this->schedule(schedule_selector(MainGameScene::CreateBlockCallback),0.1f);
	
	return true;
}

void MainGameScene::update(float dt)
{
	// update game scene

	if(KeyboardInput::GetKey(CC_KEY_UP))
	{
		// go up
		mainCharacter->setPosition(ccp(mainCharacter->getPositionX(),mainCharacter->getPositionY() + dt * 100.0f));
	}
	if (KeyboardInput::GetKey(CC_KEY_DOWN))
	{
		// go down
		mainCharacter->setPosition(ccp(mainCharacter->getPositionX(),mainCharacter->getPositionY() - dt * 100.0f));
	}
	if (KeyboardInput::GetKey(CC_KEY_LEFT))
	{
		//go left
		mainCharacter->setPosition(ccp(mainCharacter->getPositionX() - dt * 100.0f,mainCharacter->getPositionY()));
	}
	if (KeyboardInput::GetKey(CC_KEY_RIGHT))
	{
		// go right
		mainCharacter->setPosition(ccp(mainCharacter->getPositionX() + dt * 100.0f,mainCharacter->getPositionY()));
	}

	// update blocks, make them go up

	
	//iterate through the list
	std::list<CCSprite*>::iterator it;
	//lets just pretend that no more than 5 blocks will be deleted in the same frame
	std::list<CCSprite*>::iterator toDelete[5];
	int amountToDelete = 0;
	for (it = blocksList.begin(); it != blocksList.end(); it++)
	{
		(*it)->setPositionY((*it)->getPositionY() + dt * 100.0f);
		// check if it's time to kill the block
		if ((*it)->getPositionY() > 650.0f)
		{
			toDelete[amountToDelete] = it;
			if(amountToDelete < 5)
			{
				amountToDelete++;
			}
		}
	}

	// delete dead ones
	for (int i = 0; i < amountToDelete; i++)
	{
		// remove from layer
		this->removeChild((*toDelete[i]),true);
		// remove from local list
		blocksList.erase(toDelete[i]);
	}
	
}

void MainGameScene::CreateBlockCallback(float time)
{

	// create block on a random location
	CCSprite* newBlock = CCSprite::create("Assets/red_block.png");

	// randomize position
	newBlock->setPositionX(rand() % 800);
	// Y always in the botton
	newBlock->setPositionY(-80.0f);

	// add as a child to this layer
	this->addChild(newBlock);

	// add to local list
	blocksList.push_back(newBlock);
}
