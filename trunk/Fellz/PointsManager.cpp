#include "PointsManager.h"

// define this here
PointsManager* pointsManager;

USING_NS_CC;


PointsManager* PointsManager::create(void)
{
	PointsManager * pRet = new PointsManager();
	pRet->autorelease();
	return pRet;
}

PointsManager::PointsManager()
{
	points = 0; // players start with 0 points
	points2 = 0;
	// setup label
	pointsLabel = CCLabelBMFont::create("0","Assets/badab.fnt");
	pointsLabel->setPositionX(760.0f);
	pointsLabel->setPositionY(30.0f);
	pointsLabel->setColor(ccc3(0,0,0));
	this->addChild(pointsLabel);

	// create label that write "points" on the screen
	CCLabelBMFont* pointsLabelWord = CCLabelBMFont::create("Points:","Assets/badab.fnt");
	pointsLabelWord->setColor(ccc3(0,0,0));
	pointsLabelWord->setPositionY(30.0f);
	pointsLabelWord->setPositionX(680.0f);
	this->addChild(pointsLabelWord);
}

PointsManager::~PointsManager()
{
	
}

void PointsManager::AnihilationHappened(int numberOfBlocks, cocos2d::CCPoint spot)
{
	// number of points that will be added depends upon the number of blocks that are being anihilated
	int addpoints = 0;
	// in case it's a power up
	if (numberOfBlocks == 1) {addpoints = 2;}
	// normal anihilation
	else if (numberOfBlocks == 3){addpoints = 10;}
	// 4-block anihilation
	else if (numberOfBlocks == 4){addpoints = 30;}
	// bigger anihiltion
	else if (numberOfBlocks == 5){addpoints = 60;}
	
	points += addpoints;
	// update number of points
	pointsToDisplay.initWithFormat("%i",points);
	pointsLabel->setCString(pointsToDisplay.getCString());


	// create a flying number on the screen
	CCString flyingNumberString;
	flyingNumberString.initWithFormat("%i",addpoints);
	CCLabelBMFont* flyingNumber = CCLabelBMFont::create(flyingNumberString.getCString(),"Assets/badab.fnt");
	flyingNumber->setColor(ccc3(0,0,0));
	flyingNumber->setPosition(spot);
	//put some nice actions on the number
	CCAction* sequence = CCSequence::create(CCDelayTime::actionWithDuration(1.0),
											  CCFadeOut::actionWithDuration(1.0f),
											  CCCallFuncO::actionWithTarget(this,callfuncO_selector(PointsManager::DeleteTextCallback),flyingNumber),
											  NULL);
	// add to node
	flyingNumber->runAction(sequence);
	flyingNumber->runAction(CCMoveBy::actionWithDuration(2.0f,ccp(0.0f,120.0f)));
	this->addChild(flyingNumber);

	if (isConnected)
	{
		// send update points to other player
		RakNet::BitStream BsOut;
		BsOut.Write((RakNet::MessageID)ID_GAME_NEW_POINTS);
		BsOut.Write((const char*)&points,sizeof(int));
		player2->Send(&BsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,player2Adress,false);
	}
}

void PointsManager::DeleteTextCallback(cocos2d::CCObject* object)
{
	// remove text from node
	this->removeChild((CCNode*)object,true);
}