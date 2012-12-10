#ifndef __POINTS_SCENE__
#define __POINTS_SCENE__

#include "cocos2d.h"
#include "PointsManager.h"
// PointsScene class it is showed after the game's end
class PointsScene : public cocos2d::CCLayer
{
public:
	// setup scene
	// act as a constructor
	static cocos2d::CCScene* scene();

	// setup scene
	bool init();

	// "dealloc"
	virtual ~PointsScene(void);

	// implement the "static node()" method manually
	LAYER_CREATE_FUNC(PointsScene);

	// called each frame
	void update(float dt);

	// points increase callback
	void PointsIncreaseCallback(cocos2d::CCObject* obj);

private:
	int animationValue;
	cocos2d::CCLabelBMFont* player2Points;

};


#endif // __POINTS_SCENE__