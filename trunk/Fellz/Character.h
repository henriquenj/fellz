#ifndef __CHARACTER_CLASS__
#define __CHARACTER_CLASS__


#define CHARACTER_TAG 99 // to be used in the processing of collisions

#include "cocos2d.h"
#include "Box2D\Box2D.h"

#include "KeyboardInput.h"
#include "ColorBlock.h"
#include "PointsScene.h"
#include "WarningSign.h"
// class that stores the concept of a character, processing input and other things
class Character
{
public:
	// must hold a pointer to the layer and the Box2D world
	Character(cocos2d::CCLayer* layer, b2World* world,WarningSign* warning);
	~Character(void);
	void Update(float dt);
	inline b2Body* GetBody()const
	{
		return characterBody;
	}
	inline const cocos2d::CCPoint& GetPosition()const
	{
		return sprite->getPosition();
	}
private:
	cocos2d::CCLayer* layer;
	cocos2d::CCSprite* sprite;
	b2Body* characterBody;
	b2World* world;
	WarningSign* warning;
	
};


#endif // __CHARACTER_CLASS__