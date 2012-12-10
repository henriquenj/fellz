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
	player2->Shutdown(100);
	RakNet::RakPeerInterface::DestroyInstance(player2);
}

bool WaitingConnectionScene::init()
{
	// super init first of all
	if (!CCLayer::init())
	{
		return false;
	}

	connectedSign = CCLabelBMFont::create("","Assets/badab.fnt");
	connectedSign->setPosition(ccp(400.0f,100.0f));
	this->addChild(connectedSign);

	// create instance here using global pointer
	// create instance of a client and try to connect
	player2 = RakNet::RakPeerInterface::GetInstance();
	RakNet::SocketDescriptor sd(CLIENT_PORT,0);
	player2->Startup(1, &sd, 1);
	RakNet::Packet* packet;
	// by default, this is true
	isServer = true;

	//start broadcast
	if (player2->Ping("255.255.255.255",SERVER_PORT,false))
	{
		Sleep(500);
		for (packet = player2->Receive();packet;player2->DeallocatePacket(packet),packet=player2->Receive())
		{
			//DefaultMessageIDTypes i = (DefaultMessageIDTypes)packet->data[0];
			if (packet->data[0] == ID_UNCONNECTED_PONG)
			{
				//try to connect
				player2->Connect(packet->systemAddress.ToString(false),SERVER_PORT,0,0);
				Sleep(200);
			}
			else if (packet->data[0] == ID_GAME_MESSAGE_1)
			{
				//reply
				RakNet::BitStream bsOut;
				bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				player2->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
				isServer = false;
				isConnected = true;
				// change warning
				connectedSign->setColor(ccc3(0,0,255));
				connectedSign->setString("Player Connected!");
				// copy player2 adress
				player2Adress = packet->systemAddress;
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
		player2->SetMaximumIncomingConnections(10);
		// now the update method will do the rest
		// change warning
		connectedSign->setColor(ccc3(255,0,0));
		connectedSign->setString("No Player Connected");
	}

	player2->SetTimeoutTime(3000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	// get instance for director
	CCDirector* director = CCDirector::sharedDirector();
	CCSize windowSize = director->getWinSize();

	//setup scene
	
	// only the server can start the game
	if (isServer)
	{
		//jump button
		CCMenuItemFont* jumpButton = CCMenuItemFont::create("Jump",this,menu_selector(WaitingConnectionScene::JumpCallback));
		// put the item on the middle of screen
		jumpButton->setPosition(windowSize.width/2.0f,windowSize.height/2.0f);
		// create menu to put button
		CCMenu* mainMenu = CCMenu::create(jumpButton, NULL);
		mainMenu->setPosition(CCPointZero);
		this->addChild(mainMenu);
	}
	else
	{
		CCLabelBMFont* awatingPlayerSign = CCLabelBMFont::create("","Assets/badab.fnt");
		awatingPlayerSign->setPosition(windowSize.width/2.0f,windowSize.height/2.0f);
		awatingPlayerSign->setString("Awating for the other player start the game...");
		this->addChild(awatingPlayerSign);		
	}

	// put update method to work
	this->scheduleUpdate();

	//init random seed
	srand(time(0));
	
	return true;
}


void WaitingConnectionScene::update(float dt)
{
	// keep checking for incomming connections

	RakNet::Packet* packet;
	for (packet = player2->Receive();packet;player2->DeallocatePacket(packet),packet=player2->Receive())
	{
		if (packet->data[0] == ID_NEW_INCOMING_CONNECTION)
		{
			//reply
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
			player2->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,packet->systemAddress,false);
		}
		else if (packet->data[0] == ID_GAME_MESSAGE_1)
		{
			// copy player2 adress
			player2Adress = packet->systemAddress;
			connectedSign->setColor(ccc3(0,0,255));
			connectedSign->setString("Player Connected!");
			isConnected = true;
		}
		else if (packet->data[0] == ID_GAME_START)
		{
			// start game
			CCDirector::sharedDirector()->pushScene(CCTransitionProgressOutIn::transitionWithDuration(1.0f,MainGameScene::scene()));
		}
		// player disconnected
		else if (packet->data[0] == ID_DISCONNECTION_NOTIFICATION || 
				packet->data[0] == ID_CONNECTION_LOST)
		{
			// change warning
			connectedSign->setColor(ccc3(255,0,0));
			connectedSign->setString("No Player Connected");
			isConnected = false;
		}
	}

}

void WaitingConnectionScene::JumpCallback(CCObject* pSender)
{
	if (isServer && isConnected)
	{
		// send signal to start the game
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_START);
		player2->Send(&bsOut,HIGH_PRIORITY,RELIABLE_ORDERED,0,player2Adress,false);
	}
	// calls next scene
	CCDirector::sharedDirector()->pushScene(CCTransitionProgressOutIn::transitionWithDuration(1.0f,MainGameScene::scene()));
}
