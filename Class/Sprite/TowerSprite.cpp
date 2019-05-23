#include"TowerSprite.h"
#include<string>
#include<vector>

//4座塔，0,1是蓝方，2，3是红方
TowerSprite::TowerSprite(int id) 
{
	hero.attack = 60;
	hero.HP = 800;
	hero.attack_distance = 500;
	hero.survival = true;

	//给红蓝方不同颜色的血条
	if (id == 0 || id == 1)
	{
		blood = Sprite::create("blood_bar&blue_bar/bule_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		hero.red = false;
	}
	else
	{
		blood = Sprite::create("blood_bar&blue_bar/red_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		hero.red = true;
	}

	//微调血条的位置
	switch (id)
	{
	case 0:
		blood->setPosition(Vec2(100, 250));
		break;
	case 1:
		blood->setPosition(Vec2(20, 150));
		break;
	case 2:
		blood->setPosition(Vec2(100, 300));
		break;
	case 3:
		blood->setPosition(Vec2(150, 300));
		break;
	default:
		break;
	}
}

TowerSprite* TowerSprite::create(int id)
{
	TowerSprite* towerSprite = new TowerSprite(id);
	towerSprite->init(id);
	towerSprite->autorelease();
	return towerSprite;
}
bool TowerSprite::init(int id)
{
	std::vector<std::string> tower_image =
	{
		"defending_tower/basement_blue.png",
		"defending_tower/tower_blue.png",
		"defending_tower/tower_red.png",
		"defending_tower/basement_red.png"
	};

	//添加防御塔
	Sprite::initWithFile(tower_image[id]);
	return true;
}


