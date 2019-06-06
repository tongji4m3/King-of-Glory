#include"RoomScene.h"

USING_NS_CC;

Scene* RoomScene::createScene()
{
	auto scene = Scene::create();
	auto layer = RoomScene::create();
	scene->addChild(layer);
	return scene;
}

bool  RoomScene::init()
{
	if (!Layer::init()) 
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//载入背景
	auto sprite = Sprite::create("RoomScene/room.png");
	if (sprite == nullptr)
	{
		return false;
	}
	else
	{
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(sprite, 0);
	}

	//添加开始游戏按钮  
	auto item1 = MenuItemImage::create("RoomScene/startGame.png", "RoomScene/startGame_selected.png", CC_CALLBACK_1(RoomScene::RoomToGame, this));
	auto menu = Menu::create(item1, NULL);
	menu->alignItemsVerticallyWithPadding(50);
	//按钮放置在屏幕正中间
	item1->setPosition(Vec2(origin.x, origin.y - 300));
	this->addChild(menu, 1);

	return true;
}

void RoomScene::RoomToGame(Ref* pSender)
{
	Director::getInstance()->end();
}

