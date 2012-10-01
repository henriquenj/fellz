#include "Character.h"

USING_NS_CC;

Character::Character(cocos2d::CCLayer* layer,b2World* world)
{
	// copy references
	this->layer = layer;
	this->world = world;

	// load asset and put in the right spot
	sprite = CCSprite::create("Assets/character.png");
	sprite->setPosition(ccp(400.0f,400.0f));
	sprite->setTag(CHARACTER_TAG);
	layer->addChild(sprite);

	// create body stuff here

	// descriptor
	b2BodyDef blockDef;
	blockDef.type = b2_staticBody;	
	// divide by the PTM ratio
	blockDef.position.Set(sprite->getPositionX() / PTM_RATIO, sprite->getPositionY() / PTM_RATIO);
	// define that this CCSprite is linked with this body
	blockDef.userData = sprite;
	// creates body with this descriptors
	characterBody = world->CreateBody(&blockDef);

	// now comes the shape
	b2PolygonShape blockShape;
	// create shape based on the size of the sprite
	blockShape.SetAsBox((sprite->boundingBox().size.width / PTM_RATIO) / 4.6,(sprite->boundingBox().size.height / PTM_RATIO) / 2);

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
	// grab rotation
	float rotation = sprite->getRotation();
	// to control if a player pressed a button
	bool pressed = false;

	// perform Input
	if(KeyboardInput::GetKey(CC_KEY_UP))
	{
		// go up
		sprite->setPosition(ccp(sprite->getPositionX(),sprite->getPositionY() + dt * 100.0f));
		pressed = true;
	}
	if (KeyboardInput::GetKey(CC_KEY_DOWN))
	{
		// go down
		sprite->setPosition(ccp(sprite->getPositionX(),sprite->getPositionY() - dt * 100.0f));
		pressed = true;
	}
	if (KeyboardInput::GetKey(CC_KEY_LEFT))
	{
		//go left
		sprite->setPosition(ccp(sprite->getPositionX() - dt * 100.0f,sprite->getPositionY()));
		// uodate rotation
		rotation -= dt * 10;
		pressed = true;
	}
	if (KeyboardInput::GetKey(CC_KEY_RIGHT))
	{
		// go right
		sprite->setPosition(ccp(sprite->getPositionX() + dt * 100.0f,sprite->getPositionY()));
		// uodate rotation
		rotation += dt * 10;
		pressed = true;
	}

	// make the sprite goes back to first positions after being rotate
	if (pressed == false)
	{
		if (rotation > 0.5f)
		{
			// tries to goes back to 0.0
			rotation -= dt * 10;
		}
		else if (rotation < -0.5f)
		{
			rotation += dt * 10;
		}
	}

	// put back rotation
	sprite->setRotation(rotation);

	// update body position based on CCSprite position
	characterBody->SetTransform(b2Vec2(sprite->getPosition().x / PTM_RATIO,
								sprite->getPosition().y / PTM_RATIO),
								-1 * CC_DEGREES_TO_RADIANS(sprite->getRotation()));


	// check if the character is on the death zone
	if (sprite->getPositionX() < 20.0f || sprite->getPositionX() > 750.0f || 
		sprite->getPositionY() > 550.0f || sprite->getPositionY() < 20.0f)
	{
		// game over animation goes here
		// for now just call the next scene
		CCDirector::sharedDirector()->pushScene(CCTransitionProgressHorizontal::transitionWithDuration(1.0f,PointsScene::scene()));
	}
}
