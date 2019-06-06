#include "ChooseHeroScene.h"
#include"../GameScene/MainScene.h"
USING_NS_CC;


Scene* ChooseHeroScene::createScene()
{
	auto scene = Scene::create();
	auto layer = ChooseHeroScene::create();
	scene->addChild(layer);
	return scene;
}

bool ChooseHeroScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto hero_num1 = MenuItemImage::create("characters/daji/daji.png", "characters/daji/daji.png", this, menu_selector(ChooseHeroScene::chooseHero1));
	auto hero_num2 = MenuItemImage::create("characters/yase/yase.png", "characters/yase/yase.png", this, menu_selector(ChooseHeroScene::chooseHero2));
	auto hero_num3 = MenuItemImage::create("characters/houyi/houyi.png", "characters/houyi/houyi.png", this, menu_selector(ChooseHeroScene::chooseHero3));
	auto m = Menu::create(hero_num1, hero_num2, hero_num3, 0);
	this->addChild(m);
	m->alignItemsHorizontallyWithPadding(100);

	//ÔØÈë±³¾°
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

int id;
void ChooseHeroScene::chooseHero1(Ref* pSender)
{
	id = 0;
	Director::getInstance()->replaceScene(MainScene::createScene());
}
void ChooseHeroScene::chooseHero2(Ref* pSender)
{
	id = 1;
	Director::getInstance()->replaceScene(MainScene::createScene());
}
void ChooseHeroScene::chooseHero3(Ref* pSender)
{
	id = 2;
	Director::getInstance()->replaceScene(MainScene::createScene());
}