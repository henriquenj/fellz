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
	// destroy raknet object
	RakNet::RakPeerInterface::DestroyInstance(player2);
}

bool WaitingConnectionScene::init()
{
	// super init first of all
	if (!CCLayer::init())
	{
		return false;
	}

	// create instance here using global pointer

	// create instance of a client and try to connect
	player2 = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd(CLIENT_PORT,0);
	player2->Startup(1, &sd, 1);
	RakNet::Packet* packet;

	// by default, this is true
	isServer = true;
	bool connected = false;
	bool isFellz = false;
	// first, attempts to connect with the other player
	// only localhost for now
	if (player2->Connect("localhost",SERVER_PORT,0,0) == RakNet::CONNECTION_ATTEMPT_STARTED)
	{
		for (packet = player2->Receive();packet;player2->DeallocatePacket(packet),packet=player2->Receive())
		{
			if(packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
			{
				connected = true;
			}
			else if (packet->data[0] == ID_GAME_MESSAGE_1 && connected)
			{
				isFellz = true;
				isServer = false;
			}
		}
	}

	// if didn't find anything, create a server peer
	if (isServer)
	{
		//first delete old one
		RakNet::RakPeerInterface::DestroyInstance(player2);
		//create a new
		player2 = RakNet::RakPeerInterface::GetInstance();
		RakNet::SocketDescriptor sd_server(SERVER_PORT,0);
		player2->Startup(10,&sd_server,1);
		// now the update method will do the rest
	}

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

	// put update method to work
	this->scheduleUpdate();
	
	return true;
}


void WaitingConnectionScene::update(float dt)
{
	bool connected = false;
	bool isFellz = false;
	// keep checking for incomming connections
	if (isServer)
	{
		RakNet::Packet* packet;
		for (packet = player2->Receive();packet;player2->DeallocatePacket(packet),packet=player2->Receive())
		{
			if (packet->data[0] == ID_CONNECTION_REQUEST_ACCEPTED)
			{
				// ok, someone answered
				// let's see who is it
				//reply
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				player2->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
				connected = true;
			}
			else if (packet->data[0] == ID_GAME_MESSAGE_1 && connected)
			{
				isFellz = true;
			}
		}
	}
}

void WaitingConnectionScene::JumpCallback(CCObject* pSender)
{
	// calls next scene
	CCDirector::sharedDirector()->pushScene(CCTransitionProgressOutIn::transitionWithDuration(1.0f,MainGameScene::scene()));
}
