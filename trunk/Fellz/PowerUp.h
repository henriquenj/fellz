
#ifndef __POWERUP_CLASS__
#define __POWERUP_CLASS__



#include "cocos2d.h"
#include "Box2D\Box2D.h"
// power up base class provide interface for the rest of power ups objects
// take care of sprite drawning, collision detection and power up usage
// subject to change
class PowerUp : cocos2d::CCSprite
{
public:

	static PowerUp* create(const char * file, b2World *world);

	PowerUp(b2World *world);

	// 'dealloc'
	virtual ~PowerUp(void);

private:
	// local reference to world object
	b2World* world;
};



#endif // __POWERUP_CLASS__