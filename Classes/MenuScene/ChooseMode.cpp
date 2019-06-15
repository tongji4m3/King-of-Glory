#include"ChooseMode.h"
#include"RoomScene.h"
#include"ChooseHeroScene.h"

#include<string>

USING_NS_CC;

Scene* ChooseMode::createScene()
{
	auto scene = Scene::create();
	auto layer = ChooseMode::create();
	scene->addChild(layer);
	return scene;
}

bool ChooseMode::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//在视图的正中间竖向位置添加人机对战+开房间按钮  
	auto item1 = MenuItemImage::create("ChooseMode/man_vs_computer.png", "ChooseMode/man_vs_computer_selected.png", CC_CALLBACK_1(ChooseMode::onPushSceneChooseHero, this));
	auto item2 = MenuItemImage::create("ChooseMode/man_vs_man.png", "ChooseMode/man_vs_man_selected.png", CC_CALLBACK_1(ChooseMode::onPushSceneCreateRoom, this));
	auto item3 = MenuItemImage::create("ChooseMode/man_vs_man.png", "ChooseMode/man_vs_man_selected.png", CC_CALLBACK_1(ChooseMode::onPushSceneJoinRoom, this));

	auto menu = Menu::create(item1, item2, item3, NULL);
	menu->alignItemsVerticallyWithPadding(50);

	this->addChild(menu, 1);

	// 添加主界面背景
	auto sprite = Sprite::create("ChooseMode/loading_background.png");
	if (sprite == nullptr)
	{
		return false;
	}
	else
	{
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(sprite, 0);
	}

	return true;
}

//转场选择英雄界面  
void ChooseMode::onPushSceneChooseHero(Ref* pSender)
{
	Director::getInstance()->replaceScene(ChooseHeroScene::createScene());
}

std::string choose_mode;
//转场创建房间界面  
void ChooseMode::onPushSceneCreateRoom(Ref* pSender)
{
	choose_mode = "create";
	Director::getInstance()->replaceScene(RoomScene::createScene());
}
//转场加入房间界面
void ChooseMode::onPushSceneJoinRoom(Ref* pSender)
{
	choose_mode = "join";
	Director::getInstance()->replaceScene(RoomScene::createScene());
}