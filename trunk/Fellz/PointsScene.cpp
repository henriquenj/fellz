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
	
	this->addChild(pointsText);
	this->addChild(numbersText);
	
	return true;
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