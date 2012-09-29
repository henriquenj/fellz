#include "PointsManager.h"

// define this here
PointsManager* pointsManager;

USING_NS_CC;


PointsManager* PointsManager::create(void)
{
	PointsManager * pRet = new PointsManager();
	pRet->autorelease();
	return pRet;
}

PointsManager::PointsManager()
{

}

PointsManager::~PointsManager()
{

}

