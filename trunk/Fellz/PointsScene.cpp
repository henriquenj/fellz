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
	return true;
}
