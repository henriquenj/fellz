

#ifndef __CHANGE_SCREEN_SPECIAL__
#define __CHANGE_SCREEN_SPECIAL__

#include "PowerUpBase.h"
#include "RaknetGlobals.h"

// the information that'll go through the network
struct BlockInformation
{
	float posX,posY;
	short color;
	//bool attached;
	//int index; //index of block
	//int attachedIndex; //index of the father block
	bool hasPowerUp;
};


// change screen special implements the functionality of the "swap level"  feature
class ChangeScreenSpecial : public PowerUpBase
{
public:
	// create method, copied from CCNode class
	static ChangeScreenSpecial* create();

	//constructor 
	ChangeScreenSpecial();
	// delloc
	~ChangeScreenSpecial(void);

	// actual execute
	// to be used by MainGameScene
	void ExecuteActual();

private:
	void Execute();
	void ExecuteTransferCallback();
	void Send(RakNet::BitStream *bsOut);
};	

#endif //__CHANGE_SCREEN_SPECIAL__