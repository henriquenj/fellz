#ifndef __MAIN_GAME_SCENE__
#define __MAIN_GAME_SCENE__


// this scene handle the game itself and should only be called when the other player is connected

#include <list>
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "KeyboardInput.h"
#include "ColorBlock.h"

class MainGameScene : public cocos2d::CCLayer
{
public:

	// setup scene
	static cocos2d::CCScene* scene();

	// "dealloc"
	virtual ~MainGameScene();

	// setup scene
	bool init();

	// implement the "static node()" method manually
	LAYER_CREATE_FUNC(MainGameScene);

	void update(float dt);

	// function callback of createblocks timer
	void CreateBlockCallback(float time);
	
private:
	// represents the main character
	cocos2d::CCSprite* mainCharacter;
	// list that hold points to all blocks
	std::list<ColorBlock*> blocksList;
	// pointer to Box2D world
	// hold all related Box2D stuff
	b2World* box2DWorld;
};


#endif // __MAIN_GAME_SCENE__
