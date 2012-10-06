#include "WarningSign.h"

#include "MainGameScene.h"
USING_NS_CC;

WarningSign::WarningSign(b2World* world)
{
	
	// load the assets and put into node
	// all have 0 opacity at first
	// every sprite gets one action
	leftCircularSignal = CCSprite::create("Assets/warning_sign_1.png");
	leftCircularSignal->setOpacity(0);
	this->addChild(leftCircularSignal);
	CCRepeatForever* repeat1 = CCRepeatForever::actionWithAction(CCRotateBy::actionWithDuration(1.0f,360));
	leftCircularSignal->runAction(repeat1);

	rightCircularSignal = CCSprite::create("Assets/warning_sign_1.png");
	rightCircularSignal->setOpacity(0);
	this->addChild(rightCircularSignal);
	CCRepeatForever* repeat2 = CCRepeatForever::actionWithAction(CCRotateBy::actionWithDuration(1.0f,360));
	rightCircularSignal->runAction(repeat2);

	topCircularSignal = CCSprite::create("Assets/warning_sign_1.png");
	topCircularSignal->setOpacity(0);
	this->addChild(topCircularSignal);
	CCRepeatForever* repeat3 = CCRepeatForever::actionWithAction(CCRotateBy::actionWithDuration(1.0f,360));
	topCircularSignal->runAction(repeat3);

	botCircularSignal = CCSprite::create("Assets/warning_sign_1.png");
	botCircularSignal->setOpacity(0);
	this->addChild(botCircularSignal);
	CCRepeatForever* repeat4 = CCRepeatForever::actionWithAction(CCRotateBy::actionWithDuration(1.0f,360));
	botCircularSignal->runAction(repeat4);

	leftSignal = CCSprite::create("Assets/warning_sign_2.png");
	leftSignal->setOpacity(0);
	this->addChild(leftSignal);
	CCSequence* sequence1 = CCSequence::actionOneTwo(CCScaleTo::actionWithDuration(1.0f,1.5f),CCScaleTo::actionWithDuration(1.0f,0.5f));
	CCRepeatForever* repeat5 = CCRepeatForever::actionWithAction(sequence1);
	leftSignal->runAction(repeat5);

	rightSignal = CCSprite::create("Assets/warning_sign_2.png");
	rightSignal->setOpacity(0);
	this->addChild(rightSignal);
	CCSequence* sequence2 = CCSequence::actionOneTwo(CCScaleTo::actionWithDuration(1.0f,1.5f),CCScaleTo::actionWithDuration(1.0f,0.5f));
	CCRepeatForever* repeat6 = CCRepeatForever::actionWithAction(sequence2);
	rightSignal->runAction(repeat6);

	botSignal = CCSprite::create("Assets/warning_sign_2.png");
	botSignal->setOpacity(0);
	this->addChild(botSignal);
	CCSequence* sequence3 = CCSequence::actionOneTwo(CCScaleTo::actionWithDuration(1.0f,1.5f),CCScaleTo::actionWithDuration(1.0f,0.5f));
	CCRepeatForever* repeat7 = CCRepeatForever::actionWithAction(sequence3);
	botSignal->runAction(repeat7);

	topSignal = CCSprite::create("Assets/warning_sign_2.png");
	topSignal->setOpacity(0);
	this->addChild(topSignal);
	CCSequence* sequence4 = CCSequence::actionOneTwo(CCScaleTo::actionWithDuration(1.0f,1.5f),CCScaleTo::actionWithDuration(1.0f,0.5f));
	CCRepeatForever* repeat8 = CCRepeatForever::actionWithAction(sequence4);
	topSignal->runAction(repeat8);

	this->scheduleUpdate();

	leftMost = botMost = 900;
	topMost = rightMost = -900;

	this->world = world;

	// create sensors using box2d
	// body
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.userData = this;
	bodyDef.allowSleep = false;
	// create the shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox((100/PTM_RATIO)/2,(630/PTM_RATIO)/2,b2Vec2(1.0f,9.5f),0);
	// fixture
	b2FixtureDef fixture;
	fixture.shape = &boxShape;
	fixture.density = 1.0f;
	fixture.friction = 0.2f;
	fixture.restitution = 0.8f;
	fixture.isSensor = true;

	// left body
	left_b = world->CreateBody(&bodyDef);
	left_b->CreateFixture(&fixture);
	// right body
	boxShape.SetAsBox((100/PTM_RATIO)/2,(630/PTM_RATIO)/2,b2Vec2(24.0f,9.5f),0);
	right_b = world->CreateBody(&bodyDef);
	right_b->CreateFixture(&fixture);
	// top 
	boxShape.SetAsBox((100/PTM_RATIO)/2,(750/PTM_RATIO)/2,b2Vec2(12.5f,18.0f),CC_DEGREES_TO_RADIANS(90));
	top_b = world->CreateBody(&bodyDef);
	top_b->CreateFixture(&fixture);
	// bot
	boxShape.SetAsBox((100/PTM_RATIO)/2,(750/PTM_RATIO)/2,b2Vec2(12.5f,0.5f),CC_DEGREES_TO_RADIANS(90));
	bot_b = world->CreateBody(&bodyDef);
	bot_b->CreateFixture(&fixture);

	bot = top = left = right = false;
}


WarningSign::~WarningSign(void)
{
}

WarningSign* WarningSign::create(b2World* world)
{
	WarningSign * pRet = new WarningSign(world);
	pRet->autorelease();
	return pRet;
}

void WarningSign::update(float dt)
{
	// every bool is false at the end of frame
	bot = top = left = right = false;
	leftMost = botMost = 900;
	topMost = rightMost = -900;
	// check for collisions on each block
	b2Body* collidedBody;
	CCPoint point;


	/////////////////////////////////////////////////////////////////////////////////////////
	// bot
	for (b2ContactEdge* edge = bot_b->GetContactList(); edge != NULL; edge = edge->next)
	{
		if (edge->contact->IsTouching())
		{
			if(edge->contact->GetFixtureA()->GetBody() == bot_b)
			{
				collidedBody = edge->contact->GetFixtureB()->GetBody();
			}
			else{collidedBody = edge->contact->GetFixtureA()->GetBody();}
			// check if it's a block or the character
			if(((CCSprite*)collidedBody->GetUserData())->getTag() == BLOCK_TAG)
			{
				if(!((ColorBlock*)collidedBody->GetUserData())->GetAttached())
				{
					continue;
				}
			}
			else if(((CCSprite*)collidedBody->GetUserData())->getTag() == CHARACTER_TAG){}
			else {continue;}

			point = ((CCSprite*)collidedBody->GetUserData())->getPosition();
			// check if it's the bot most so far
			if (point.y < botMost)
			{

				botMost = point.y;
				// put sprite on the spot
				botCircularSignal->setOpacity(255);
				point.y = 30.0f;
				botCircularSignal->setPosition(point);
				botSignal->setOpacity(255);
				botSignal->setPosition(point);
				bot = true;
			}
		}
	}
	if (bot == false)
	{
		botCircularSignal->setOpacity(0);
		botSignal->setOpacity(0);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// top
	for (b2ContactEdge* edge = top_b->GetContactList(); edge != NULL; edge = edge->next)
	{
		if (edge->contact->IsTouching())
		{
			if(edge->contact->GetFixtureA()->GetBody() == top_b)
			{
				collidedBody = edge->contact->GetFixtureB()->GetBody();
			}
			else{collidedBody = edge->contact->GetFixtureA()->GetBody();}
			// check if it's a block or the character
			if(((CCSprite*)collidedBody->GetUserData())->getTag() == BLOCK_TAG)
			{
				if(!((ColorBlock*)collidedBody->GetUserData())->GetAttached())
				{
					continue;
				}
			}
			else if(((CCSprite*)collidedBody->GetUserData())->getTag() == CHARACTER_TAG){}
			else {continue;}

			point = ((CCSprite*)collidedBody->GetUserData())->getPosition();
			// check if it's the bot most so far
			if (point.y > topMost)
			{

				topMost = point.y;
				// put sprite on the spot
				topCircularSignal->setOpacity(255);
				point.y = 560.0f;
				topCircularSignal->setPosition(point);
				topSignal->setOpacity(255);
				topSignal->setPosition(point);
				top = true;
			}
		}
	}
	if (top == false)
	{
		topCircularSignal->setOpacity(0);
		topSignal->setOpacity(0);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// left
	for (b2ContactEdge* edge = left_b->GetContactList(); edge != NULL; edge = edge->next)
	{
		if (edge->contact->IsTouching())
		{
			if(edge->contact->GetFixtureA()->GetBody() == left_b)
			{
				collidedBody = edge->contact->GetFixtureB()->GetBody();
			}
			else{collidedBody = edge->contact->GetFixtureA()->GetBody();}
			// check if it's a block or the character
			if(((CCSprite*)collidedBody->GetUserData())->getTag() == BLOCK_TAG)
			{
				if(!((ColorBlock*)collidedBody->GetUserData())->GetAttached())
				{
					continue;
				}
			}
			else if(((CCSprite*)collidedBody->GetUserData())->getTag() == CHARACTER_TAG){}
			else {continue;}

			point = ((CCSprite*)collidedBody->GetUserData())->getPosition();
			// check if it's the bot most so far
			if (point.x < leftMost)
			{

				leftMost = point.x;
				// put sprite on the spot
				leftCircularSignal->setOpacity(255);
				point.x = 30.0f;
				leftCircularSignal->setPosition(point);
				leftSignal->setOpacity(255);
				leftSignal->setPosition(point);
				left = true;
			}
		}
	}
	if (left == false)
	{
		leftCircularSignal->setOpacity(0);
		leftSignal->setOpacity(0);
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	// right
	for (b2ContactEdge* edge = right_b->GetContactList(); edge != NULL; edge = edge->next)
	{
		if (edge->contact->IsTouching())
		{
			if(edge->contact->GetFixtureA()->GetBody() == right_b)
			{
				collidedBody = edge->contact->GetFixtureB()->GetBody();
			}
			else{collidedBody = edge->contact->GetFixtureA()->GetBody();}
			// check if it's a block or the character
			if(((CCSprite*)collidedBody->GetUserData())->getTag() == BLOCK_TAG)
			{
				if(!((ColorBlock*)collidedBody->GetUserData())->GetAttached())
				{
					continue;
				}
			}
			else if(((CCSprite*)collidedBody->GetUserData())->getTag() == CHARACTER_TAG){}
			else {continue;}

			point = ((CCSprite*)collidedBody->GetUserData())->getPosition();
			// check if it's the bot most so far
			if (point.x > rightMost)
			{

				rightMost = point.x;
				// put sprite on the spot
				rightCircularSignal->setOpacity(255);
				point.x = 770.0f;
				rightCircularSignal->setPosition(point);
				rightSignal->setOpacity(255);
				rightSignal->setPosition(point);
				right = true;
			}
		}
	}
	if (right == false)
	{
		rightCircularSignal->setOpacity(0);
		rightSignal->setOpacity(0);
	}
}