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
	blockDef.type = b2_dynamicBody;	
	// divide by the PTM ratio
	blockDef.position.Set(sprite->getPositionX() / PTM_RATIO, sprite->getPositionY() / PTM_RATIO);
	// define that this CCSprite is linked with this body
	blockDef.userData = sprite;
	// creates body with this descriptors
	characterBody = world->CreateBody(&blockDef);

	// now comes the shape
	b2PolygonShape trunkShape;
	// create shape based on the size of the sprite
	trunkShape.SetAsBox((sprite->boundingBox().size.width / PTM_RATIO) / 7,(sprite->boundingBox().size.height / PTM_RATIO) / 2.4);

	// we gonna need to set the points on hand
	b2Vec2 vertex[4];
	vertex[0].x = -1.8f;
	vertex[0].y = 1.8f;
	vertex[1].x = -1.8f;
	vertex[1].y = 1.0f;
	vertex[2].x = 1.8f;
	vertex[2].y = 1.0f;
	vertex[3].x = 1.8f;
	vertex[3].y = 1.8f;
	// create other shape for arms
	b2PolygonShape arm;
	arm.Set(vertex,4);
	
	// connect shape with body
	b2FixtureDef trunkFixture;
	trunkFixture.filter.categoryBits = 0x0004;
	trunkFixture.shape = &trunkShape;
	trunkFixture.density = 1.0f;
	trunkFixture.friction = 0.2f;
	trunkFixture.restitution = 0.8f;

	b2FixtureDef armFixture;
	armFixture.filter.categoryBits = 0x0004;
	armFixture.shape = &arm;
	armFixture.density = 1.0f;
	armFixture.friction = 0.2f;
	armFixture.restitution = 0.8f;

	// add to body
	characterBody->CreateFixture(&armFixture);
	characterBody->CreateFixture(&trunkFixture);
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
	if (sprite->getPositionX() < 60.0f || sprite->getPositionX() > 740.0f || 
		sprite->getPositionY() > 530.0f || sprite->getPositionY() < 70.0f)
	{
		// game over animation goes here
		// for now just call the next scene
		CCDirector::sharedDirector()->pushScene(CCTransitionProgressHorizontal::transitionWithDuration(1.0f,PointsScene::scene()));
		// to prevent thousands of scenes being created
		CCDirector::sharedDirector()->getRunningScene()->unscheduleUpdate();
	}

}
