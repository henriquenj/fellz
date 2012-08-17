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
	
}
