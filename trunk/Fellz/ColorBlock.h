#ifndef __COLOR_BLOCK_CLASS__
#define __COLOR_BLOCK_CLASS__


#include "cocos2d.h"
#include "Box2D\Box2D.h"

// pixels per meter
#define PTM_RATIO 32

// color defines
#define BLOCK_RED 0
#define BLOCK_BLUE 1
#define BLOCK_GREEN 2


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
	// to be used to destroy a block when there's two or more of the same kind nearby
	void Destroy();

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
	inline short GetBlockColor()const
	{
		return cubeColor;
	}
	inline bool GetDying()const
	{
		return this->isDying;
	}

private:
	// body
	b2Body* body;
	//world reference
	b2World* world;
	// bool that controls if the block id attached to the main character
	bool attached;
	b2Body* attachedBody; // pointer to attached body
	// stores cube color, could varies from 0 to 2
	short cubeColor;
	// bool that stores if a block is dying
	bool isDying;
};


#endif // __COLOR_BLOCK_CLASS__
