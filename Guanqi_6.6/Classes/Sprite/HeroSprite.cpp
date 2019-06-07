#include"HeroSprite.h"
#include<string>
#include<vector>

HeroSprite::HeroSprite(int id)
{
	//经验等级数值
	int attack = 50;
	int HP = 500;
	int experience = 100;
	hero.attack.push_back(attack);
	hero.HP.push_back(HP);
	hero.experience.push_back(experience);

	hero.attack_distance = 400;
	hero.survival = true;

	for (int level = 1; level < 21; level++) {
		attack += 10;
		HP += 100;
		experience += 25;
		hero.attack.push_back(attack);
		hero.HP.push_back(HP);
		hero.experience.push_back(experience);
	}

	//给红蓝方不同颜色的血条
	if (id>=0 && id<=2)
	{
		blood = Sprite::create("blood_bar&blue_bar/bule_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		blood->setPosition(Vec2(50, 200));
		hero.red = false;
	}
	else
	{
		blood = Sprite::create("blood_bar&blue_bar/red_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		blood->setPosition(Vec2(50, 300));
		hero.red = true;
	}
	
	
}

HeroSprite* HeroSprite::create(int id)
{
	HeroSprite* heroSprite = new HeroSprite(id);
	//为了在这里不区分红蓝
	if (id > 2)
	{
		id -= 3;
	}
	heroSprite->init(id);
	heroSprite->autorelease();
	return heroSprite;
}
bool HeroSprite::init(int id)
{
	std::vector<std::string> hero_image =
	{
		"characters/daji/daji_down.png",
		"characters/yase/yase_down.png",
		"characters/houyi/houyi_down.png"
	};

	//添加英雄
	Sprite::initWithFile(hero_image[id]);
	return true;
}
