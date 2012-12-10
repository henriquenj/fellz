

#ifndef __POINTS_MANAGER__
#define __POINTS_MANAGER__

#include "cocos2d.h"
#include "RaknetGlobals.h"

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
	// Call this function when a anihilation happaned, passing the number of blocks and the spot where it happened
	void AnihilationHappened(int numberOfBlocks, cocos2d::CCPoint spot);
	// function to be called every time a text must be deleted
	// receive the text to be deleted as parameter
	void DeleteTextCallback(cocos2d::CCObject* object);
	// Get number of points
	inline const int GetPoints()const
	{
		return points;
	}
	// Get player2 points
	inline const int GetP2Points()const
	{
		return points2;
	}
	inline void SetP2Points(int newPoints)
	{
		points2 = newPoints;
	}
private:
	// player points
	int points;
	// player 2 points
	int points2;
	cocos2d::CCString pointsToDisplay;// to be displayed on screen
	// label that show the points on the top right corner
	cocos2d::CCLabelBMFont* pointsLabel;
};

// hold reference to the only pointsManager in the game
extern PointsManager* pointsManager;


#endif // __POINTS_MANAGER__

