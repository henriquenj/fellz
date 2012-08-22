#ifndef __COLOR_BLOCK_CLASS__
#define __COLOR_BLOCK_CLASS__


#include "cocos2d.h"
#include "Box2D\Box2D.h"

// pixels per meter
#define PTM_RATIO 32

//Color block class hold references to Box2D bodys, its a CCSprite with Box2D
class ColorBlock : public cocos2d::CCSprite
{
public:
	ColorBlock();
	static ColorBlock* create(const char * file);
	// set world reference, only then the bodys are created
	void SetWorld(b2World* world);
	virtual ~ColorBlock(void);

private:
	// body
	b2Body* body;
	//world reference
	b2World* world;
};


#endif // __COLOR_BLOCK_CLASS__
