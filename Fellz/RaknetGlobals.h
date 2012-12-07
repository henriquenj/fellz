// Raknet globals holds the global objects and related defines


#ifndef __RAKNET_GLOBALS__
#define __RAKNET_GLOBALS__

#include "RakPeerInterface.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

#define SERVER_PORT 59999
#define CLIENT_PORT 49999

// main interface
extern RakNet::RakPeerInterface* player2;
extern bool isServer;

enum GameMessages
{
	ID_GAME_MESSAGE_1=ID_USER_PACKET_ENUM+1
};

#endif // __RAKNET_GLOBALS__