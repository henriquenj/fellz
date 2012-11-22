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

WaitingConnectionScene::~WaitingConnectionScene()
{
	
}

bool WaitingConnectionScene::init()
{
	// super init first of all
	if (!CCLayer::init())
	{
		return false;
	}

	//#define MAX_CLIENTS 30
	//#define SERVER_PORT 6000
	//RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	//RakNet::SocketDescriptor sd(SERVER_PORT,0);
	//peer->Startup(MAX_CLIENTS, &sd, 1);

	// get instance for director
	CCDirector* director = CCDirector::sharedDirector();
	CCSize windowSize = director->getWinSize();

	//setup scene
	
	//jump button
	CCMenuItemFont* jumpButton = CCMenuItemFont::create("Jump",this,menu_selector(WaitingConnectionScene::JumpCallback));
	// put the item on the middle of screen
	jumpButton->setPosition(windowSize.width/2.0f,windowSize.height/2.0f);
	// create menu to put button
	CCMenu* mainMenu = CCMenu::create(jumpButton, NULL);
	mainMenu->setPosition(CCPointZero);
	this->addChild(mainMenu);

	//init random seed
	srand(time(0));
	
	
	return true;
}

void WaitingConnectionScene::JumpCallback(CCObject* pSender)
{
	// calls next scene
	CCDirector::sharedDirector()->pushScene(CCTransitionProgressOutIn::transitionWithDuration(1.0f,MainGameScene::scene()));
}
