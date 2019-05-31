#include"CheckResult.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace std;

Scene* CheckResult::createScene()
{
	auto scene = Scene::create();
	auto layer = CheckResult::create();
	scene->addChild(layer);
	return scene;
}

bool  CheckResult::init()
{
	if (!Layer::init()) {
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//ÔØÈë±³¾°
	auto sprite = Sprite::create("room.png");
	if (sprite == nullptr)
	{
		return false;
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}


	return true;

}