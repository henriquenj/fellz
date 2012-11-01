#include "PowerUp.h"
#include "MainGameScene.h"

USING_NS_CC;

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

PowerUp::~PowerUp()
{
	if (body != NULL)
	{
		world->DestroyBody(body);
	}
}

void PowerUp::update(float dt)
{
	// power ups are handled by Box2D
	// so just update based on physics simulation
	this->setPosition(ccp(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO));
	this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));

	// check if is colliding with character
	for (b2ContactEdge* edge = body->GetContactList();edge;edge = edge->next)
	{
		if (edge->contact->IsTouching())
		{
			// find which fixture is
			b2Body* collidedBody;
			if (edge->contact->GetFixtureA()->GetBody() == body)
			{
				collidedBody = edge->contact->GetFixtureB()->GetBody();
			}
			else
			{
				collidedBody = edge->contact->GetFixtureA()->GetBody();
			}

			// now proceed
			if (((CCSprite*)collidedBody->GetUserData())->getTag() == CHARACTER_TAG)
			{
				((MainGameScene*)this->getParent())->GotPowerUp(this);
				edge = NULL;
				world->DestroyBody(body);
				body = NULL;
				this->unscheduleUpdate();
				break;
			}
		}
	}

	if (getPositionY() > 650.0f || getPositionY() < -85.0f ||
			getPositionX() < -20.0f || getPositionX() > 850.0f)
	{
		// delete block
		this->removeFromParentAndCleanup(true);
	}
}

PowerUp::PowerUp(b2World *world)
{
	this->world = world;
	this->scheduleUpdate();

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
	powerShape.m_radius = 16.0f / PTM_RATIO;
	
	// connect shape with body
	b2FixtureDef powerFixture;
	// make that fixture only collides with power up objects
	powerFixture.filter.maskBits = 0x0004;
	powerFixture.shape = &powerShape;
	powerFixture.density = 1.0f;
	powerFixture.friction = 0.2f;
	powerFixture.restitution = 0.8f;

	// add to body
	body->CreateFixture(&powerFixture);

	// make the blocks goes up
	body->SetLinearVelocity(b2Vec2(0.0,2.0f));
	body->SetAngularVelocity((rand() % 10) / 10.0f);
}

void PowerUp::setPosition(const cocos2d::CCPoint& pos)
{
	CCSprite::setPosition(pos);
	// must update body so this could work
	body->SetTransform(b2Vec2(this->getPosition().x / PTM_RATIO,
						this->getPosition().y / PTM_RATIO),
						body->GetAngle());

}