
#ifndef __WAITING_CONNECTION_SCENE__
#define __WAITING_CONNECTION_SCENE__

#include "cocos2d.h"

#include "MainGameScene.h"
#include "RaknetGlobals.h"
// this scene just keeps checking if the other player is ready to play, like a lobby
class WaitingConnectionScene : public cocos2d::CCLayer
{
public:
	
	// setup scene
	virtual bool init();

	virtual ~WaitingConnectionScene();

	// act like a constructor
	static cocos2d::CCScene* scene();

	// jump menu item callback
	void JumpCallback(CCObject* pSender);

	// called each frame
	void update(float dt);

	 // implement the "static node()" method manually
	LAYER_CREATE_FUNC(WaitingConnectionScene);

private:
	// emits a warning when there's a connected peer
	cocos2d::CCLabelBMFont* connectedSign;
};

#endif // __WAITING_CONNECTION_SCENE__

