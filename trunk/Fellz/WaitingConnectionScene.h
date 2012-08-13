
#ifndef __WAITING_CONNECTION_SCENE__
#define __WAITING_CONNECTION_SCENE__


// this scene just keeps checking if the other player is ready to play, like a lobby
#include "cocos2d.h"

class WaitingConnectionScene : public cocos2d::CCLayer
{
public:
	
	// setup scene
	virtual bool init();

	// act like a constructor
	static cocos2d::CCScene* scene();

	 // implement the "static node()" method manually
	LAYER_CREATE_FUNC(WaitingConnectionScene);
};


#endif // __WAITING_CONNECTION_SCENE__

