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
	// delete character
	delete mainCharacter;
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

	// init infinite background sprite object
	infiniteBackground = CCSprite::create();
	infiniteBackground->setPosition(ccp(400.0f,200.0f));
	// load sprites for infinite background
	CCArray* imageFrames = CCArray::arrayWithCapacity(15);
	// load all 15 frames
	for (int p = 0; p < 15; p++)
	{
		CCString fileName;
		fileName.initWithFormat("Assets/infinite_background/infinite_background_%i.jpg",p); // build file name
		// create texture
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(fileName.getCString());
		// get texture size
		CCSize texSize = texture->getContentSize();
		CCRect texRect = CCRectMake(0,0,texSize.width,texSize.height);
		// create frame from texture
		CCSpriteFrame* frame = CCSpriteFrame::frameWithTexture(texture,texRect);
		// add to array
		imageFrames->addObject(frame);
	}
	
	// now create the animation and put in a infinite loop
	CCAnimation* anim = CCAnimation::animationWithSpriteFrames(imageFrames,0.03f);
	CCAnimate* animate = CCAnimate::actionWithAnimation(anim); // action
	CCRepeatForever* repeat = CCRepeatForever::actionWithAction(animate);

	infiniteBackground->runAction(repeat);
	// correct scale of the sprites since I made them too big
	infiniteBackground->setScale(0.8f);
	this->addChild(infiniteBackground);

	// put the update method to work
	this->scheduleUpdate();

	this->schedule(schedule_selector(MainGameScene::CreateBlockCallback),0.5f);

	// create Box2D stuff
	// init world
	box2DWorld = new b2World(b2Vec2(0.0f,0.0f));

	// create character
	mainCharacter = new Character(this,box2DWorld);

	// create batch 
	blocksBatch = CCSpriteBatchNode::batchNodeWithFile("Assets/block.png");
	this->addChild(blocksBatch);

	// define pointsmanager
	pointsManager = PointsManager::create();
	this->addChild(pointsManager);

	return true;
}

void MainGameScene::update(float dt)
{

	// update physics engine
	box2DWorld->Step(dt,10,10);

	// update game scene
	
	//update character
	mainCharacter->Update(dt);


	// update blocks
	//iterate through the list
	std::list<ColorBlock*>::iterator it;
	//lets just pretend that no more than 5 blocks will be deleted in the same frame

	std::list<ColorBlock*>::iterator toDelete[5];
	int amountToDelete = 0;
	for (it = blocksList.begin(); it != blocksList.end(); it++)
	{
		// only process if the block isn't dying
		if (!(*it)->GetDying())
		{
			// check if it's time to kill the block
			if ((*it)->getPositionY() > 650.0f || (*it)->getPositionY() < -85.0f ||
				(*it)->getPositionX() < -20.0f || (*it)->getPositionX() > 850.0f)
			{
				if(amountToDelete < 5)
				{
					toDelete[amountToDelete] = it;
					amountToDelete++;
				}
			}
			else
			{
				// dont process if the block is already connected with the character
				if (!(*it)->GetAttached())
				{
					// process colision detection
					b2ContactEdge* edge = (*it)->GetBody()->GetContactList();
					while (edge != NULL) // if == null, no more collisions
					{
						if (edge->contact->IsTouching())
						{
							// iterate through the list of contacts
							// if collided with character, don't delete
							if (((CCSprite*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->getTag() == CHARACTER_TAG ||
								((CCSprite*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->getTag() == CHARACTER_TAG)
							{
								// the block collided with character
								(*it)->AttachTo(mainCharacter->GetBody());
							}
							// check if it's a attached block, so attached to this one
							else if (((CCSprite*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->getTag() == BLOCK_TAG && 
								((CCSprite*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->getTag() == BLOCK_TAG)
							{
								// check which one is the IT
								// attached based on this
								if (edge->contact->GetFixtureA()->GetBody() == (*it)->GetBody())
								{
									// make sure it's already attached
									if(((ColorBlock*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->GetAttached() &&
										!((ColorBlock*)edge->contact->GetFixtureB()->GetBody()->GetUserData())->GetDying())
									{
										// attach on B fixture
										(*it)->AttachTo(edge->contact->GetFixtureB()->GetBody());
									}
								}
								else
								{
									if(((ColorBlock*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->GetAttached() &&
										!((ColorBlock*)edge->contact->GetFixtureA()->GetBody()->GetUserData())->GetDying())
									{
										// attach on A fixture
										(*it)->AttachTo(edge->contact->GetFixtureA()->GetBody());
									}
								}
							}
						}
						edge = edge->next; // go to the next colision
					}
				}
				// now process those which are already attached to the character body
				else
				{
					(*it)->BuildConnections(NULL, -1);
					// check if one of the attached blocks is on the death area
					if ((*it)->getPositionX() < -5.0f || (*it)->getPositionX() > 810.0f || 
						(*it)->getPositionY() > 610.0f || (*it)->getPositionY() < -5.0f)
					{
						// game over animation goes here
						// for now just call the next scene
						CCDirector::sharedDirector()->pushScene(CCTransitionProgressHorizontal::transitionWithDuration(1.0f,PointsScene::scene()));
					}
				}
			}
		}
		else
		{
			// check if it's time to be deleted
			if((*it)->getOpacity() == 0)
			{
				if(amountToDelete < 5)
				{
					toDelete[amountToDelete] = it;
					amountToDelete++;
				}
			}
		}
	}

	// delete dead ones
	for (int i = 0; i < amountToDelete; i++)
	{
		// remove from batch
		blocksBatch->removeChild((*toDelete[i]),true);
		// remove from local list
		blocksList.erase(toDelete[i]);
	}
}

void MainGameScene::CreateBlockCallback(float time)
{
	// create block on a random location
	ColorBlock* newBlock = ColorBlock::create("Assets/block.png");
	// unattached blocks have 50% opacity
	newBlock->setOpacity(127);

	// randomize position
	newBlock->setPositionX(rand() % 800);
	//newBlock->setPositionX(300.0f);
	// Y always in the botton
	newBlock->setPositionY(-80.0f);

	// add as a child to the batch
	blocksBatch->addChild(newBlock);

	newBlock->InitInWorld(box2DWorld);

	// add to local list
	blocksList.push_back(newBlock);

}
