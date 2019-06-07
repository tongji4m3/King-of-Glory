#include"CheckResult.h"
#include"MenuScene/WelcomeScene.h"
#include "SimpleAudioEngine.h"
#include"CombatResult.h"
#include"Data/Hero.h"
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

	//载入背景
	auto sprite = Sprite::create("loading_page/loading_background.png");
	if (sprite == nullptr)
	{
		return false;
	}
	else
	{
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		this->addChild(sprite, 0);
	}
	//添加返回主菜单按钮  
	auto return_to = MenuItemImage::create("loading_page/return.png", "loading_page/return_selected.png", CC_CALLBACK_1(CheckResult::ReturnToWelcome, this));
	auto menu = Menu::create(return_to, NULL);
	//按钮放置在屏幕正下方
	return_to->setPosition(Vec2(origin.x, origin.y - 400));
	this->addChild(menu, 1);

	//战绩展示
	std::string money_text;
	money_text += "MONEY: ";
	money_text += std::to_string(money_result);
	money_label = Label::create(money_text, "Arial", 40);
	money_label->setColor(Color3B(50, 100, 200));
	this->addChild(money_label);
	money_label->setPosition(Vec2(300, 800));

	std::string level_text;
	level_text += "LEVEL: ";
	level_text += std::to_string(level_result);
	level_label = Label::create(level_text, "Arial", 40);
	level_label->setColor(Color3B(50, 100, 200));
	this->addChild(level_label);
	level_label->setPosition(Vec2(300, 750));

	std::string experience_text;
	experience_text += "EXPERIENCE: ";
	experience_text += std::to_string(experience_result);
	experience_label = Label::create(experience_text, "Arial", 40);
	experience_label->setColor(Color3B(50, 100, 200));
	this->addChild(experience_label);
	experience_label->setPosition(Vec2(300, 700));

	std::string killed_hero_text;
	killed_hero_text += "KILLED_HERO: ";
	killed_hero_text += std::to_string(killed_hero_result);
	killed_hero_label = Label::create(killed_hero_text, "Arial", 40);
	killed_hero_label->setColor(Color3B(50, 100, 200));
	this->addChild(killed_hero_label);
	killed_hero_label->setPosition(Vec2(300, 650));

	std::string killed_soldier_text;
	killed_soldier_text += "KILLED_SOLDIER: ";
	killed_soldier_text += std::to_string(killed_soldier_result);
	killed_soldier_label = Label::create(killed_soldier_text, "Arial", 40);
	killed_soldier_label->setColor(Color3B(50, 100, 200));
	this->addChild(killed_soldier_label);
	killed_soldier_label->setPosition(Vec2(300, 600));

	std::string destroyed_tower_text;
	destroyed_tower_text += "DESTROYED_TOWER: ";
	destroyed_tower_text += std::to_string(destroyed_tower_result);
	destroyed_tower_label = Label::create(destroyed_tower_text, "Arial", 40);
	destroyed_tower_label->setColor(Color3B(50, 100, 200));
	this->addChild(destroyed_tower_label);
	destroyed_tower_label->setPosition(Vec2(300, 550));

	std::string be_killed_text;
	be_killed_text += "BE_KILLED: ";
	be_killed_text += std::to_string(be_killed_result);
	be_killed_label = Label::create(be_killed_text, "Arial", 40);
	be_killed_label->setColor(Color3B(50, 100, 200));
	this->addChild(be_killed_label);
	be_killed_label->setPosition(Vec2(300, 500));

	return true;
}
void CheckResult::ReturnToWelcome(Ref* pSender)
{
	Director::getInstance()->replaceScene(WelcomeScene::createScene());
}