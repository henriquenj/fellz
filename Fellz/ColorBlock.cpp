#include "ColorBlock.h"



ColorBlock::ColorBlock()
{
	body = NULL;
	world = NULL;
	attachedBody = NULL;
}

ColorBlock* ColorBlock::create(const char * file)
{
	// I hope that I made the corret init sequence

	ColorBlock *pobSprite = new ColorBlock();
    if (pobSprite && pobSprite->initWithFile(file))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

void ColorBlock::update(float dt)
{
	// update sprite's position based on body from Box2D
	this->setPosition(ccp(body->GetPosition().x * PTM_RATIO, body->GetPosition().y * PTM_RATIO));
	this->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));

}
void ColorBlock::InitInWorld(b2World* world)
{

	// put the update method to work;
	this->scheduleUpdate();

	// hold reference
	this->world = world;

	// create body stuff here

	// descriptor
	b2BodyDef blockDef;
	blockDef.type = b2_dynamicBody;
	blockDef.allowSleep = false;
	// divide by the PTM ratio
	blockDef.position.Set(this->getPositionX() / PTM_RATIO, this->getPositionY() / PTM_RATIO);
	// define that this CCSprite is linked with this body
	blockDef.userData = this;
	// creates body with this descriptors
	body = world->CreateBody(&blockDef);

	// now comes the shape
	b2PolygonShape blockShape;
	// create shape based on the size of the sprite
	blockShape.SetAsBox((this->boundingBox().size.width / PTM_RATIO)/2,(this->boundingBox().size.height / PTM_RATIO)/2);

	// connect shape with body
	b2FixtureDef blockFixture;
	blockFixture.shape = &blockShape;
	blockFixture.density = 1.0f;
	blockFixture.friction = 0.2f;
	blockFixture.restitution = 0.8f;

	// add to body
	body->CreateFixture(&blockFixture);

	// make the blocks goes up
	body->SetLinearVelocity(b2Vec2(0.0,3.0));

	attached = false;
	// set tag
	this->setTag(BLOCK_TAG);
}

ColorBlock::~ColorBlock(void)
{
	if (body != NULL)
	{
		world->DestroyBody(body);
	}
}

void ColorBlock::AttachTo(b2Body* toAttach)
{
	// set this flag with true
	attached = true;
	// copy reference
	attachedBody = toAttach;

	//// define joint
	//b2DistanceJointDef jointDef;
	//jointDef.Initialize(body,toAttach,b2Vec2(this->getPositionX() / PTM_RATIO,this->getPositionY() / PTM_RATIO),toAttach->GetPosition() );
	//// add to world
	//world->CreateJoint(&jointDef);


	b2Vec2 worldCoordsAnchorPoint = body->GetWorldPoint(b2Vec2(0.6f,0));
	// define joint
	b2WeldJointDef jointDef;
	jointDef.bodyA = toAttach;
	jointDef.bodyB = body;
	jointDef.localAnchorB = body->GetLocalPoint(worldCoordsAnchorPoint);
	jointDef.localAnchorA = toAttach->GetLocalPoint(worldCoordsAnchorPoint);
	jointDef.referenceAngle = body->GetAngle() - toAttach->GetAngle();
	// add to world
	world->CreateJoint(&jointDef);

	
}
