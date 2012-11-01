
#ifndef __POWER_UP_BASE__
#define __POWER_UP_BASE__

#include "cocos2d.h"
#include "KeyboardInput.h"

// Power up base class offers interface for all the power ups in the game
// executes the code of the power up, with gameplay changes
// add as child of MainGameScene
class PowerUpBase : public cocos2d::CCNode
{
public:
	// constructor
	PowerUpBase(void);
	// 'dealloc'
	~PowerUpBase(void);
	// called each frame
	void update(float dt);
	// check if power up is executing right now
	inline bool isActive()
	{
		return active;
	}

private:
	// called when the user press space key
	virtual void Execute() = 0;
protected:
	// tell if power up is executing right now
	bool active;
};



#endif // __POWER_UP_BASE__