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
	bool active_t = false;
	// only perform the swap if it's connected AND the other game is still running
	if (isConnected)
	{
		if (!otherGameOver)
		{
			active_t = true;
			// send signal to other application
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_GAME_INIT_SWAP);
			player2->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,player2Adress,false);
			// do the rest
			this->ExecuteActual();
		}
	}

	if (!active_t)
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
	((ChangeScreenSpecial*)(this->getParent()))->Send(&bufferOut);

	delete toSendContent;
	//delete power up
	((ChangeScreenSpecial*)(this->getParent()))->DeletePowerUpCallback();
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
