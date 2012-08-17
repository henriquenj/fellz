#ifndef __MAIN_GAME_SCENE__
#define __MAIN_GAME_SCENE__


// this scene handle the game itself and should only be called when the other player is connected

#include "cocos2d.h"

#include "KeyboardInput.h"
class MainGameScene : public cocos2d::CCLayer
{
public:

	// setup scene
	static cocos2d::CCScene* scene();

	// setup scene
	bool init();

	// implement the "static node()" method manually
	LAYER_CREATE_FUNC(MainGameScene);

	void update(float dt);
	
private:
	// represents the main character
	cocos2d::CCSprite* mainCharacter;
};


#endif // __MAIN_GAME_SCENE__
