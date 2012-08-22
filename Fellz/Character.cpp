#include "Character.h"

USING_NS_CC;

Character::Character(cocos2d::CCLayer* layer,b2World* world)
{
	// copy references
	this->layer = layer;
	this->world = world;

	// load asset and put in the right spot
	sprite = CCSprite::create("Assets/meat_boy_by_barakaldo-d3apehs.png");
	sprite->setPosition(ccp(400.0f,500.0f));
	layer->addChild(sprite);


	// create body stuff here

	// descriptor
	b2BodyDef blockDef;
	blockDef.type = b2_staticBody;
	// divide by the PTM ratio
	blockDef.position.Set(sprite->getPositionX() / PTM_RATIO, sprite->getPositionY() / PTM_RATIO);
	// define that this CCSprite is linked with this body
	blockDef.userData = this;
	// creates body with this descriptors
	characterBody = world->CreateBody(&blockDef);

	// now comes the shape
	b2PolygonShape blockShape;
	// create shape based on the size of the sprite
	blockShape.SetAsBox((sprite->boundingBox().size.width / PTM_RATIO) / 2,(sprite->boundingBox().size.height / PTM_RATIO) / 2);

	// connect shape with body
	b2FixtureDef blockFixture;
	blockFixture.shape = &blockShape;
	blockFixture.density = 1.0f;
	blockFixture.friction = 0.2f;
	blockFixture.restitution = 0.8f;

	// add to body
	characterBody->CreateFixture(&blockFixture);
}


Character::~Character(void)
{
	// delete the body here
	world->DestroyBody(characterBody);
}

void Character::Update(float dt)
{
	// perform Input
	if(KeyboardInput::GetKey(CC_KEY_UP))
	{
		// go up
		sprite->setPosition(ccp(sprite->getPositionX(),sprite->getPositionY() + dt * 100.0f));
	}
	if (KeyboardInput::GetKey(CC_KEY_DOWN))
	{
		// go down
		sprite->setPosition(ccp(sprite->getPositionX(),sprite->getPositionY() - dt * 100.0f));
	}
	if (KeyboardInput::GetKey(CC_KEY_LEFT))
	{
		//go left
		sprite->setPosition(ccp(sprite->getPositionX() - dt * 100.0f,sprite->getPositionY()));
	}
	if (KeyboardInput::GetKey(CC_KEY_RIGHT))
	{
		// go right
		sprite->setPosition(ccp(sprite->getPositionX() + dt * 100.0f,sprite->getPositionY()));
	}


	// update body position based on CCSprite position
	characterBody->SetTransform(b2Vec2(sprite->getPosition().x / PTM_RATIO,
								sprite->getPosition().y / PTM_RATIO),
								characterBody->GetAngle());
}
