#ifndef __COLOR_BLOCK_CLASS__
#define __COLOR_BLOCK_CLASS__


#include "cocos2d.h"
#include "Box2D\Box2D.h"

// pixels per meter
#define PTM_RATIO 32

#define BLOCK_TAG 999
//Color block class hold references to Box2D bodys, its a CCSprite with Box2D
class ColorBlock : public cocos2d::CCSprite
{
public:
	ColorBlock();
	static ColorBlock* create(const char * file);
	// update callback, called every frame
	void update(float dt);
	// set world reference and init block
	void InitInWorld(b2World* world);
	virtual ~ColorBlock(void);

	// Attach the block to a body (probably the character)
	void AttachTo(b2Body* toAttach);

	inline b2Body* GetBody()const
	{
		return body;
	}
	inline bool GetAttached()const
	{
		return attached;
	}

private:
	// body
	b2Body* body;
	//world reference
	b2World* world;
	// bool that controls if the block id attached to the main character
	bool attached;
};


#endif // __COLOR_BLOCK_CLASS__
