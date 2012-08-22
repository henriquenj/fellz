#ifndef __CHARACTER_CLASS__
#define __CHARACTER_CLASS__

#include "cocos2d.h"
#include "Box2D\Box2D.h"

#include "KeyboardInput.h"
#include "ColorBlock.h"
// class that stores the concept of a character, processing input and other things
class Character
{
public:
	// must hold a pointer to the layer and the Box2D world
	Character(cocos2d::CCLayer* layer, b2World* world);
	~Character(void);
	void Update(float dt);

private:
	cocos2d::CCLayer* layer;
	cocos2d::CCSprite* sprite;
	b2Body* characterBody;
	b2World* world;
	
};


#endif // __CHARACTER_CLASS__