#include "PowerUp.h"

PowerUp* PowerUp::create(const char * file,b2World *world)
{
	// I hope that I made the corret init sequence

	PowerUp *pobSprite = new PowerUp(world);
    if (pobSprite && pobSprite->initWithFile(file))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}


PowerUp::PowerUp(b2World *world)
{
	this->world = world;

	//TODO: make the rest of box2d stuff here
}

PowerUp::~PowerUp()
{

}