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

bool MainGameScene::init()
{
	if (!CCLayer::init())
	{
		return false;
	}


	return true;
}
