#include "OnlineChooseHero.h"
#include"../GameScene/OnlineScene.h"
#include"../NetWork/MessageCode.h"
#include<string>

USING_NS_CC;

//谁进来就代表哪个玩家
static Client* temp_client = nullptr;

Scene* OnlineChooseHero::createScene(Client * client)
{
	temp_client = client;
	auto scene = Scene::create();
	auto layer = OnlineChooseHero::create();
	scene->addChild(layer);
	return scene;
}

bool OnlineChooseHero::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto hero_num1 = MenuItemImage::create("characters/daji/daji.png", "characters/daji/daji.png", this, menu_selector(OnlineChooseHero::chooseHero1));
	auto hero_num2 = MenuItemImage::create("characters/yase/yase.png", "characters/yase/yase.png", this, menu_selector(OnlineChooseHero::chooseHero2));
	auto hero_num3 = MenuItemImage::create("characters/houyi/houyi.png", "characters/houyi/houyi.png", this, menu_selector(OnlineChooseHero::chooseHero3));
	auto m = Menu::create(hero_num1, hero_num2, hero_num3, 0);
	this->addChild(m);
	m->alignItemsHorizontallyWithPadding(100);

	//载入背景
	auto sprite = Sprite::create("characters/choose_hero_bg.jpg");
	if (sprite == nullptr)
	{
		return false;
	}
	else
	{
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(sprite, -1);
	}

	return true;
}

int online_id;
void OnlineChooseHero::chooseHero1(Ref* pSender)
{
	online_id = 0;
	Director::getInstance()->replaceScene(OnlineScene::createScene(temp_client));
	std::string hero_type = AI_HERO_TYPE + std::to_string(online_id);
	temp_client->send_message(hero_type);
}
void OnlineChooseHero::chooseHero2(Ref* pSender)
{
	online_id = 1;
	Director::getInstance()->replaceScene(OnlineScene::createScene(temp_client));
	std::string hero_type = AI_HERO_TYPE + std::to_string(online_id);
	temp_client->send_message(hero_type);
}
void OnlineChooseHero::chooseHero3(Ref* pSender)
{
	online_id = 2;
	Director::getInstance()->replaceScene(OnlineScene::createScene(temp_client));
	std::string hero_type = AI_HERO_TYPE + std::to_string(online_id);
	temp_client->send_message(hero_type);
}