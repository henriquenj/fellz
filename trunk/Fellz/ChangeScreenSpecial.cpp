#include "ChangeScreenSpecial.h"
#include "MainGameScene.h"

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
			// send signal to other application
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_GAME_INIT_SWAP);
			player2->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,player2Adress,false);
			// do the rest
			this->ExecuteActual();
		}
	}

	if (!active)
	{
		// delete from maingamescene
		this->DeletePowerUpCallback();
	}
}
void ChangeScreenSpecial::ExecuteTransferCallback()
{
	// first pack everything on a nice C++ array
	std::vector<BlockInformation> toSend;
	MainGameScene *mainScene = (MainGameScene*)this->getParent()->getParent();
	std::list<ColorBlock*>::iterator it;
	BlockInformation infTemp;
	for (it = mainScene->blocksList.begin(); it != mainScene->blocksList.end(); it++)
	{
		// dont transfere the one which are dying
		if (!(*it)->GetDying())
		{
			// copy values
			//TODO: make the program transfer also the connectivity of the blocks
			infTemp.posX = (*it)->getPositionX();
			infTemp.posY = (*it)->getPositionY();
			infTemp.color = (*it)->GetBlockColor();
			infTemp.hasPowerUp = (*it)->hasPowerUp;
			//add to vector
			toSend.push_back(infTemp);
		}
	}
	// now put them on a C++ array
	BlockInformation* toSendContent = new BlockInformation[toSend.size()];
	// copy
	for (int p = 0; p < toSend.size();p++)
	{
		toSendContent[p] = toSend[p];
	}
	//write on the buffer
	RakNet::BitStream bufferOut;
	bufferOut.Write((RakNet::MessageID)ID_GAME_SWAP_SCREENS);
	int size = toSend.size();
	bufferOut.Write((const char*)&size,sizeof(int));
	bufferOut.Write((const char*)toSendContent,sizeof(BlockInformation) * toSend.size());
	
	// if it's a server, send first
	if (isServer)
	{
		((ChangeScreenSpecial*)(this->getParent()))->Send(&bufferOut);
		((ChangeScreenSpecial*)(this->getParent()))->Receive();
	}
	// if it's a client, recieve first
	else
	{
		((ChangeScreenSpecial*)(this->getParent()))->Receive();
		((ChangeScreenSpecial*)(this->getParent()))->Send(&bufferOut);
	}
	delete toSendContent;
	// now change the blocks
	//delete all non-dying blocks
	std::vector<ColorBlock*> toDelete;
	for (it = mainScene->blocksList.begin(); it != mainScene->blocksList.end(); it++)
	{
		if (!(*it)->GetDying())
		{
			toDelete.push_back(*it);
		}
	}
	//delete them
	for (int c = 0; c < toDelete.size();c++)
	{
		mainScene->blocksBatch->removeChild(toDelete[c],true);
		mainScene->blocksList.remove(toDelete[c]);
	}
	//put them back
	
	for (int c = 0; c < ((ChangeScreenSpecial*)(this->getParent()))->received.size(); c++)
	{
		// create block on a random location
		ColorBlock* newBlock = ColorBlock::create("Assets/block.png");
		// unattached blocks have 50% opacity
		newBlock->setOpacity(127);

		float x,y;
		x = ((ChangeScreenSpecial*)(this->getParent()))->received[c].posX;
		y = ((ChangeScreenSpecial*)(this->getParent()))->received[c].posY;
		newBlock->setPositionX(x);
		newBlock->setPositionY(y);

		// add as a child to the batch
		mainScene->blocksBatch->addChild(newBlock);
		//mainScene->addChild(newBlock);

		newBlock->InitInWorld(mainScene->box2DWorld);
		
		if (((ChangeScreenSpecial*)(this->getParent()))->received[c].color == BLOCK_GREEN)
		{
			newBlock->setColor(ccc3(0,255,0));
		}
		else if (((ChangeScreenSpecial*)(this->getParent()))->received[c].color == BLOCK_RED)
		{
			newBlock->setColor(ccc3(255,0,0));
		}
		else if (((ChangeScreenSpecial*)(this->getParent()))->received[c].color == BLOCK_BLUE)
		{
			newBlock->setColor(ccc3(0,0,255));
		}
		// add to local list
		mainScene->blocksList.push_back(newBlock);
	}

	//delete power up
	((ChangeScreenSpecial*)(this->getParent()))->DeletePowerUpCallback();
}

void ChangeScreenSpecial::Receive()
{
	RakNet::Packet* packet;
	for (packet = player2->Receive();packet;player2->DeallocatePacket(packet),packet=player2->Receive())
	{
		// only process packages from player2
		if (packet->systemAddress == player2Adress)
		{
			if (packet->data[0] == ID_GAME_SWAP_SCREENS)
			{
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
			}
		}
	}
}

void ChangeScreenSpecial::Send(RakNet::BitStream *bsOut)
{
	//send immediatly
	player2->Send(bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,player2Adress,false);
}

void ChangeScreenSpecial::ExecuteActual()
{
	// put black on everything
	CCSize windowsSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite* black = CCSprite::create("Assets/transition.png");
	black->setPosition(ccp(windowsSize.width/2,windowsSize.height/2));
	black->setScale(0.1f);
	CCScaleTo *scale = CCScaleTo::actionWithDuration(1.0f,1.0f,1.0f);
	CCSequence *sequence = CCSequence::create(scale,CCCallFunc::create(black,callfunc_selector(ChangeScreenSpecial::ExecuteTransferCallback)));
	this->addChild(black);
	black->runAction(sequence);
}
