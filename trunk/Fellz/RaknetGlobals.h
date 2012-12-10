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
// remote adrress struct used by raknet
extern RakNet::SystemAddress player2Adress;
extern bool isServer;
extern bool isConnected;
// controls if the other player has lost the game
extern bool otherGameOver;

enum GameMessages
{
	ID_GAME_MESSAGE_1=ID_USER_PACKET_ENUM+1,
	ID_GAME_START, // signal to make the player2 start the game
	ID_GAME_CHANGE_SCENARIO, // signal to make the players swap scenarios
	ID_GAME_PLAYER_LOST, // signal to warn that the player lost
	ID_GAME_NEW_POINTS // signal to send the new amount of points
};

#endif // __RAKNET_GLOBALS__