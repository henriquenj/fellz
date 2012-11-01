#ifndef __MAIN_GAME_SCENE__
#define __MAIN_GAME_SCENE__


// this scene handle the game itself and should only be called when the other player is connected

#include <list>
#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "KeyboardInput.h"
#include "ColorBlock.h"
#include "Character.h"
#include "PointsScene.h"
#include "PointsManager.h"
#include "GLES-Render.h"
#include "PowerUp.h"
#include "BlockSpecial.h"

class MainGameScene : public cocos2d::CCLayer
{
public:

	// "constructor"
	static cocos2d::CCScene* scene();

	// "dealloc"
	virtual ~MainGameScene();

	// setup scene
	bool init();

	// implement the "static node()" method manually
	LAYER_CREATE_FUNC(MainGameScene);

	void update(float dt);

	// called by power up class to process this event
	void GotPowerUp (PowerUp* up);

	// Get character
	inline const Character* GetCharcter()const
	{
		return mainCharacter;
	}
	
private:
	// represents the main character
	Character* mainCharacter;
	// pointer to batch (supposedly improve performance)
	cocos2d::CCSpriteBatchNode* blocksBatch;
	// infinite background
	cocos2d::CCSprite* infiniteBackground;
	// list that hold points to all blocks
	std::list<ColorBlock*> blocksList;
	// pointer to Box2D world
	// hold all related Box2D stuff
	b2World* box2DWorld;
	// pointer to powerup implementation
	// there can be only one at a time
	PowerUpBase* powerup;

	// function callback of createblocks timer
	void CreateBlockCallback(float time);

	// function callback for deleting a power up
	void DeletePowerUpCallback(CCObject* up);

};


#endif // __MAIN_GAME_SCENE__
