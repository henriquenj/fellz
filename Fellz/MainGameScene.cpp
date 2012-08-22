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
	// delete the other bodies before deleting world
	this->removeAllChildrenWithCleanup(true);
	// delete Box2D world
	delete box2DWorld;
}

bool MainGameScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	// put the update method to work
	this->scheduleUpdate();

	this->schedule(schedule_selector(MainGameScene::CreateBlockCallback),1.0f);


	// create Box2D stuff
	// init world
	box2DWorld = new b2World(b2Vec2(0.0f,0.0f)); // this gravity will make the objects go up

	// create character
	mainCharacter = new Character(this,box2DWorld);
	
	return true;
}

void MainGameScene::update(float dt)
{
	// update game scene
	mainCharacter->Update(dt);

	// update physics engine
	box2DWorld->Step(dt,10,10);

	// update blocks
	//iterate through the list
	std::list<ColorBlock*>::iterator it;
	//lets just pretend that no more than 5 blocks will be deleted in the same frame
	std::list<ColorBlock*>::iterator toDelete[5];
	int amountToDelete = 0;
	for (it = blocksList.begin(); it != blocksList.end(); it++)
	{
		// check if it's time to kill the block
		if ((*it)->getPositionY() > 650.0f || (*it)->getPositionY() < -85.0f)
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
	ColorBlock* newBlock = ColorBlock::create("Assets/red_block.png");

	// randomize position
	newBlock->setPositionX(rand() % 800);
	// Y always in the botton
	newBlock->setPositionY(-80.0f);

	// add as a child to this layer
	this->addChild(newBlock);

	newBlock->InitInWorld(box2DWorld);

	// add to local list
	blocksList.push_back(newBlock);
}
