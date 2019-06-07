#include"SoldierSprite.h"
#include<string>
#include<vector>

SoldierSprite::SoldierSprite(int id)
{
	soldier.attack = 45;
	soldier.HP = 300;
	soldier.attack_distance = 100;
	soldier.survival = true;

	//给红蓝方不同颜色的血条
	if (id == 0 || id == 1)
	{
		blood = Sprite::create("blood_bar&blue_bar/bule_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		blood->setPosition(Vec2(50, 200));
		soldier.red = false;
	}
	else
	{
		blood = Sprite::create("blood_bar&blue_bar/red_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		blood->setPosition(Vec2(50, 200));
		soldier.red = true;
	}

	//炮车攻击范围大
	if (id == 1 || id == 3)
	{
		soldier.attack_distance = 300;
	}
}

SoldierSprite* SoldierSprite::create(int id)
{
	SoldierSprite* soldierSprite = new SoldierSprite(id);
	soldierSprite->init(id);
	soldierSprite->autorelease();
	return soldierSprite;
}
bool SoldierSprite::init(int id)
{
	std::vector<std::string> soldier_image =
	{
		"soldiers\\soldier1_blue.png"  ,
		"soldiers\\soldier2_blue.png"  ,
		"soldiers\\soldier1_red.png"   ,
		"soldiers\\soldier2_red.png"
	};

	//添加小兵
	Sprite::initWithFile(soldier_image[id]);
	return true;
}


