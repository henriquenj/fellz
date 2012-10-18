#include "PowerUp.h"

USING_NS_CC;

PowerUp* PowerUp::create(const char * file,b2World *world, CCPoint &position)
{
	// I hope that I made the corret init sequence

	PowerUp *pobSprite = new PowerUp(world,position);
    if (pobSprite && pobSprite->initWithFile(file))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

PowerUp::~PowerUp()
{

}

void PowerUp::update(float dt)
{
	// power ups are handled by Box2D
	// so just update based on physics simulation
	this->setPosition(ccp(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO));
	this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
}

PowerUp::PowerUp(b2World *world,CCPoint &position)
{
	this->world = world;
	this->scheduleUpdate();

	this->setPosition(position);

	// descriptor
	b2BodyDef powerDef;
	powerDef.type = b2_dynamicBody;
	powerDef.allowSleep = false;
	// divide by the PTM ratio
	powerDef.position.Set(this->getPositionX() / PTM_RATIO, this->getPositionY() / PTM_RATIO);
	// define that this CCSprite is linked with this body
	powerDef.userData = this;
	// set random rotation for the block
	powerDef.angle = CC_DEGREES_TO_RADIANS(rand() % 360);
	// creates body with this descriptors
	body = world->CreateBody(&powerDef);

	// now comes the shape
	b2CircleShape powerShape;
	// create shape based on the size of the sprite
	powerShape.m_radius = 32.0f / PTM_RATIO;
	
	// connect shape with body
	b2FixtureDef powerFixture;
	powerFixture.shape = &powerShape;
	powerFixture.density = 1.0f;
	powerFixture.friction = 0.2f;
	powerFixture.restitution = 0.8f;

	// add to body
	body->CreateFixture(&powerFixture);

	// make the blocks goes up
	body->SetLinearVelocity(b2Vec2(0.0,3.0f));
}

