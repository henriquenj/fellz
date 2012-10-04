
#ifndef __WARNING_SIGN__
#define __WARNING_SIGN__

#include "cocos2d.h"
#include <list>
#include "ColorBlock.h"

// class that handles the warning signs on screen
class WarningSign : public cocos2d::CCNode
{
public:

	static WarningSign* create(std::list<ColorBlock*> *blocksList);
	WarningSign(std::list<ColorBlock*> *blocksList);
	virtual ~WarningSign(void);

	void update(float dt);
	// call this to make WarningSign process the warning on the screen passing a block to watchout
	// send NULL case it is the character
	void ShowWarn(ColorBlock* block);

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

	// bool to control internal logic
	bool left, right, top, bot;
	// variables to control the most dangerous blocks
	int topMost, leftMost, rightMost, botMost;

	// local reference to list of blocks
	std::list<ColorBlock*> *blocksList;
	ColorBlock *topBlock, *leftBlock, *rightBlock, *botBlock;
};


#endif // __WARNING_SIGN__