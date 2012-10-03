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
	points = 0; // player start with 0 points

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

	// to put the update method to work
	this->scheduleUpdate();
}

PointsManager::~PointsManager()
{
	
}

void PointsManager::update(float dt)
{

}

void PointsManager::AnihilationHappened(int numberOfBlocks, cocos2d::CCPoint spot)
{
	// those formulas are experimental, must take a closer look later
	int multiplier = 1;
	if (numberOfBlocks > 3)	{multiplier = numberOfBlocks / 2;}
	// number of points that will be added depends upon the number of blocks that are being anihilated
	int addpoints = 10 * (numberOfBlocks - 2) * multiplier;
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
}

void PointsManager::DeleteTextCallback(cocos2d::CCObject* object)
{
	// remove text from node
	this->removeChild((CCNode*)object,true);
}