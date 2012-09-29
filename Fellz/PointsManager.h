

#ifndef __POINTS_MANAGER__
#define __POINTS_MANAGER__

#include "cocos2d.h"

// points manager class take of everything inside the points managment, it's a child of MainGameScene
// and also a global variable
// I'll still see if subclassing the CCNode class is the best approach
class PointsManager : public cocos2d::CCNode
{
public:
	// create method, copied from CCNode class
	static PointsManager* create();

	// constructor
	PointsManager();

	// dealloc
	virtual ~PointsManager();
};

// hold reference to the only pointsManager in the game
extern PointsManager* pointsManager;


#endif // __POINTS_MANAGER__

