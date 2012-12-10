#include "PointsScene.h"

USING_NS_CC;

CCScene* PointsScene::scene()
{
	// alloc scene 
	
	// 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
	PointsScene *layer = PointsScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

PointsScene::~PointsScene()
{
	// nothing for now
}

bool PointsScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	animationValue = 0;
	// create texts
	CCLabelBMFont *pointsText = CCLabelBMFont::create("Your Points: ","Assets/badab.fnt");
	pointsText->setColor(ccc3(255,255,255));
	pointsText->setPosition(ccp(300.0f,-300.0f));
	CCMoveTo* moveTo = CCMoveTo::actionWithDuration(1.0f,ccp(300.0f,300.0f));
	pointsText->runAction(moveTo);

	CCLabelBMFont *numbersText = CCLabelBMFont::create("0","Assets/badab.fnt");
	numbersText->setPosition(ccp(400.0f,-300.0f));
	numbersText->setColor(ccc3(255,255,255));
	CCMoveTo* moveTo2 = CCMoveTo::actionWithDuration(1.0f,ccp(400.0f,300.0f));
	// this action will increase the points counter until it hit the current points number
	// create a action with delay
	CCSequence* sequence = CCSequence::create(CCDelayTime::actionWithDuration(0.05f),
		CCCallFuncO::create(this,callfuncO_selector(PointsScene::PointsIncreaseCallback),numbersText));
	// initial movement
	CCSequence* sequence2 = CCSequence::create(moveTo2,CCRepeat::actionWithAction(sequence,9999));
	// put action on the object
	numbersText->runAction(sequence2);

	// player 2 points string
	CCLabelBMFont* player2PointsLabel = CCLabelBMFont::create("Your Oponent Points: ","Assets/badab.fnt");
	player2PointsLabel->setColor(ccc3(255,255,255));
	player2PointsLabel->setPosition(ccp(250.0f,-360.0f));
	CCMoveTo* moveTo3 = CCMoveTo::actionWithDuration(1.0f,ccp(250.0f,240.0f));
	player2PointsLabel->runAction(moveTo3);

	// number
	CCString player2PString;
	player2PString.initWithFormat("%d",pointsManager->GetP2Points());
	player2Points = CCLabelBMFont::create(player2PString.getCString(),"Assets/badab.fnt");
	player2Points->setColor(ccc3(255,255,255));
	player2Points->setPosition(ccp(400.0f,-360.0f));
	CCMoveTo* moveTo4 = CCMoveTo::actionWithDuration(1.0f,ccp(400.0f,240.0f));
	player2Points->runAction(moveTo4);
	
	this->addChild(pointsText);
	this->addChild(numbersText);
	this->addChild(player2PointsLabel);
	this->addChild(player2Points);

	// put update method to work
	this->scheduleUpdate();
	
	return true;
}

void PointsScene::update(float dt)
{
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
				// other player just lost
				if (packet->data[0] == ID_GAME_PLAYER_LOST)
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
					CCString pointsString;
					pointsString.initWithFormat("%d",rs);
					player2Points->setString(pointsString.getCString());
					pointsManager->SetP2Points(rs);
				}
			}
		}
	}
}
void PointsScene::PointsIncreaseCallback(cocos2d::CCObject* obj)
{
	if (animationValue+5 > pointsManager->GetPoints())
	{
		animationValue = pointsManager->GetPoints();
		this->stopAllActions();
	}
	else{animationValue+=5;}

	// prepare string
	CCString numberString;
	numberString.initWithFormat("%d",animationValue);
	// send to object
	((CCLabelBMFont*)obj)->setString(numberString.getCString());

}