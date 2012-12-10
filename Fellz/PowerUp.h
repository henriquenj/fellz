
#ifndef __POWERUP_CLASS__
#define __POWERUP_CLASS__



#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "ColorBlock.h"
#include "Character.h"


// defines that set each type of power up
#define SPECIAL_RED 0
#define SPECIAL_BLUE 1
#define SPECIAL_GREEN 2
#define SPECIAL_CHANGE_SCREEN 3

// power up base class provide interface for the rest of power ups objects
// it doens't make any gameplay change
// take care of sprite drawning and collision detection 
class PowerUp : public cocos2d::CCSprite
{
public:

	static PowerUp* create(const char * file, b2World *world);

	PowerUp(b2World *world);
	
	// 'dealloc'
	virtual ~PowerUp(void);

	// called each frame
	void update(float dt);

	// set the type of power up according the defines
	inline void SetKind(const unsigned short kind)
	{
		this->kind = kind;
	}
	// get the type of power up according the defines
	inline const unsigned short GetKind()const
	{
		return kind;
	}

	void setPosition(const cocos2d::CCPoint& pos);

private:
	// local reference to world object
	b2World* world;
	// reference to body
	b2Body* body;
	// kind of power up
	unsigned short kind;
};



#endif // __POWERUP_CLASS__