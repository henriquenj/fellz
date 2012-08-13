#include "WaitingConnectionScene.h"


USING_NS_CC;


CCScene* WaitingConnectionScene::scene()
{

	// alloc scene 


	// 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
	WaitingConnectionScene *layer = WaitingConnectionScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

bool WaitingConnectionScene::init()
{
	// super init first of all
	if (!CCLayer::init())
	{
		return false;
	}

	//setup scene


	return true;
}
