
#ifndef __WARNING_SIGN__
#define __WARNING_SIGN__

#include "cocos2d.h"
#include <list>
#include "ColorBlock.h"

// class that handles the warning signs on screen
class WarningSign : public cocos2d::CCNode
{
public:

	static WarningSign* create(b2World* world);
	WarningSign(b2World* world);
	virtual ~WarningSign(void);

	void update(float dt);

private:
	// four sprites of each kind to each corner of screen
	cocos2d::CCSprite* leftCircularSignal;
	cocos2d::CCSprite* rightCircularSignal;
	cocos2d::CCSprite* topCircularSignal;
	cocos2d::CCSprite* botCircularSignal;
	// excaclamtion signals
	cocos2d::CCSprite* leftSignal;
	cocos2d::CCSprite* rightSignal;
	cocos2d::CCSprite* topSignal;
	cocos2d::CCSprite* botSignal;

	// variables to control the most dangerous blocks
	int topMost, leftMost, rightMost, botMost;
	// for internal logic
	bool bot, top, left, right;

	// local reference
	b2World* world;
	// 4 bodies to act as sensors
	b2Body *left_b, *top_b, *right_b, *bot_b;
};


#endif // __WARNING_SIGN__