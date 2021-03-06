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

	otherGameOver = false;

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

	// pre load some assets
	CCTextureCache::sharedTextureCache()->addImage("Assets/BlockSpecial.png");
	CCTextureCache::sharedTextureCache()->addImage("Assets/particle.png");
	CCTextureCache::sharedTextureCache()->addImage("Assets/fire.png");
	CCTextureCache::sharedTextureCache()->addImage("Assets/change_special.png");
	CCTextureCache::sharedTextureCache()->addImage("Assets/transition.png");

	// put the update method to work
	this->scheduleUpdate();

	this->schedule(schedule_selector(MainGameScene::CreateBlockCallback),0.5f);

	// create Box2D stuff
	// init world
	box2DWorld = new b2World(b2Vec2(0.0f,0.0f));

	WarningSign* warning = WarningSign::create(box2DWorld);
	this->addChild(warning,30);

	// create character
	mainCharacter = new Character(this,box2DWorld);

	// create batch 
	blocksBatch = CCSpriteBatchNode::batchNodeWithFile("Assets/block.png");
	this->addChild(blocksBatch);

	// define pointsmanager
	pointsManager = PointsManager::create();
	this->addChild(pointsManager,500);

	// create power up panel
	CCSprite* powerpanel = CCSprite::create("Assets/powerup_panel.png");
	powerpanel->setPosition(ccp(730.0f,520.0f));
	this->addChild(powerpanel,700);

	// must init this with null
	powerup = NULL;

	// shows debug draw on debug builds
#ifdef _DEBUG
	 //create de debug draw
	//Box2DDebugDraw* debugDraw = Box2DDebugDraw::create(box2DWorld);
	//this->addChild(debugDraw,1000);
#endif // _DEBUG	




	return true;
}

void MainGameScene::update(float dt)
{

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
					if ((*it)->getPositionX() < 0.0f || (*it)->getPositionX() > 800.0f || 
						(*it)->getPositionY() > 600.0f || (*it)->getPositionY() < 0.0f)
					{
						if (isConnected)
						{
							//send gameover signal through network
							RakNet::BitStream BsOut;
							BsOut.Write((RakNet::MessageID)ID_GAME_PLAYER_LOST);
							player2->Send(&BsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,player2Adress,false);
						}
						// game over animation goes here
						// for now just call the next scene
						//CCDirector::sharedDirector()->replaceScene(CCTransitionProgressHorizontal::transitionWithDuration(1.0f,PointsScene::scene()));
						CCDirector::sharedDirector()->pushScene(CCTransitionProgressHorizontal::transitionWithDuration(1.0f,PointsScene::scene()));
						// to prevent thousands of scenes being created
						this->unscheduleUpdate();
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

	// update physics engine
	box2DWorld->Step(dt,20,20);
	
	// update raknet
	if (isConnected)
	{
		// receive packets
		RakNet::Packet* packet;
		for (packet = player2->Receive();packet;player2->DeallocatePacket(packet),packet=player2->Receive())
		{
			// only process a packet that came from the other player
			if (packet->systemAddress == player2Adress)
			{
				// player disconnected
				if (packet->data[0] == ID_DISCONNECTION_NOTIFICATION || 
					packet->data[0] == ID_CONNECTION_LOST)
				{
					isConnected = false;
					// put some warning on the screen
					//create a text and make it goes up
					CCLabelBMFont* discWarning = CCLabelBMFont::create("Your opponent just disconnected","Assets/badab.fnt");
					discWarning->setColor(ccc3(255,0,0));
					discWarning->setPosition(ccp(400.0f,-20.0f));
					CCSequence* warningSequence = CCSequence::create(CCMoveTo::create(4.0f,ccp(400.0f,200.0f)),
						CCCallFuncO::create(this,callfuncO_selector(MainGameScene::DeleteDisctionnectionWarning),discWarning));
					discWarning->runAction(warningSequence);
					discWarning->runAction(CCFadeOut::create(4.0f));//fade
					this->addChild(discWarning,1000);
				}
				// other player just lost
				else if (packet->data[0] == ID_GAME_PLAYER_LOST)
				{
					otherGameOver = true;
				}
				else if (packet->data[0] == ID_GAME_NEW_POINTS)
				{
					// read new points from the other player
					int rs = 0;
					RakNet::BitStream bsIn(packet->data,packet->length,false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					bsIn.Read((char*)&rs,sizeof(int));
					pointsManager->SetP2Points(rs);
				}
				else if(packet->data[0] == ID_GAME_INIT_SWAP)
				{
					// other player will swap, prepare
					ChangeScreenSpecial* change = ChangeScreenSpecial::create();
					this->addChild(change,701);
					change->ExecuteActual();
					change->setTag(978);
				}
				else if(packet->data[0] == ID_GAME_SWAP_SCREENS)
				{
					// received information
					std::vector<BlockInformation> received;
					RakNet::BitStream bsIn(packet->data,packet->length,false);
					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
					// number of blocks within the package
					int size = 0;
					bsIn.Read((char*)&size,sizeof(int));
					BlockInformation* reci = new BlockInformation[size];
					bsIn.Read((char*)reci,sizeof(BlockInformation) * size);
					// put on the received vector
					for (int p = 0; p < size; p++)
					{
						received.push_back(reci[p]);
					}
					delete reci;
					// now change the blocks
					//delete all non-dying blocks
					std::list<ColorBlock*>::iterator it;
					std::vector<ColorBlock*> toDelete;
					for (it = blocksList.begin(); it != blocksList.end(); it++)
					{
						if (!(*it)->GetDying())
						{
							toDelete.push_back(*it);
						}
					}
					//delete them
					for (int c = 0; c < toDelete.size();c++)
					{
						blocksBatch->removeChild(toDelete[c],true);
						blocksList.remove(toDelete[c]);
					}
					//put them back

					for (int c = 0; c < received.size(); c++)
					{
						// create block on a random location
						ColorBlock* newBlock = ColorBlock::create("Assets/block.png");
						// unattached blocks have 50% opacity
						newBlock->setOpacity(127);

						newBlock->setPositionX(received[c].posX);
						newBlock->setPositionY(received[c].posY);

						// add as a child to the batch
						blocksBatch->addChild(newBlock);
						//mainScene->addChild(newBlock);

						newBlock->InitInWorld(box2DWorld);

						if (received[c].color == BLOCK_GREEN)
						{
							newBlock->setColor(ccc3(0,255,0));
						}
						else if (received[c].color == BLOCK_RED)
						{
							newBlock->setColor(ccc3(255,0,0));
						}
						else if (received[c].color == BLOCK_BLUE)
						{
							newBlock->setColor(ccc3(0,0,255));
						}
						// add to local list
						blocksList.push_back(newBlock);
					}
				}
			}
		}
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

void MainGameScene::GotPowerUp(PowerUp* up)
{
	// make the power up dissapear and then delete it
	CCSequence* sequence = CCSequence::create(CCScaleBy::create(0.5f,0.0f,0.0f),
				CCCallFuncO::create(this,callfuncO_selector(MainGameScene::DeletePowerUpCallback),up));
	up->runAction(sequence);
	if (powerup != NULL)
	{
		this->removeChild(powerup,true);
	}
	
	// that means it's a block special
	if (up->GetKind() < 3)
	{
		powerup = BlockSpecial::create(up->GetKind());
	}
	// change screen special
	else if (up->GetKind() == SPECIAL_CHANGE_SCREEN)
	{
		powerup = ChangeScreenSpecial::create();
		powerup->setTag(978);
	}
	this->addChild(powerup,701);
}

void MainGameScene::DeletePowerUpCallback(CCObject* up)
{
	// delete power up from the layer
	this->removeChild((PowerUp*)up,true);
}


void MainGameScene::DeleteDisctionnectionWarning(CCObject* warning)
{
	this->removeChild((CCLabelBMFont*)warning,true);
}