#include"HeroSprite.h"
#include<string>
#include<vector>

//读取妲己亚瑟后羿初始物攻数据                  
int HeroSprite::get_hero_atk(int id)
{
	std::vector<int>hero_atk = { 170,172,188 };
	return hero_atk[id % 3];
}

//读取妲己亚瑟后羿初始防御力                  
int HeroSprite::get_hero_def(int id)
{
	std::vector<int>hero_def = { 86,109,86 };
	return hero_def[id % 3];
}

//读取妲己亚瑟后羿初始蓝                  
int HeroSprite::get_hero_mp(int id)
{
	std::vector<int>hero_mp = { 560,500,500 };
	return hero_mp[id % 3];
}

//读取妲己亚瑟后羿初始法强                 
int HeroSprite::get_hero_ap(int id)
{
	std::vector<int>hero_ap = { 17,2,0 };
	return hero_ap[id % 3];
}

//读取妲己亚瑟后羿初始法抗                  
int HeroSprite::get_hero_ap_def(int id)
{
	std::vector<int>hero_ap_def = { 8,8,8 };
	return hero_ap_def[id % 3];
}

//读取妲己亚瑟后羿初始生命值
int HeroSprite::get_hero_hp(int id)
{
	std::vector<int>hero_hp = { 3383,3776,3182 };
	return hero_hp[id % 3];
}

//读取妲己亚瑟后羿攻击距离
int HeroSprite::get_hero_attack_distance(int id)
{
	std::vector<int>hero_attack_distance = { 350,150,450 };
	return hero_attack_distance[id % 3];
}

HeroSprite::HeroSprite(int id)
{
	/*
	每一次升级所需要的经验值不断增加
	每一次升级加成的生命值与攻击力不断增加
	例如：升一级提升的攻击力:10 20 30 40 50 60...
	*/
	int attack_add = 10;
	double HP_add = 100;
	int experience_add = 25;
	for (int i = 0; i < 100; ++i)
	{
		hero.level_add_attack.push_back(attack_add);
		hero.level_add_HP.push_back(HP_add);
		hero.level_add_experience.push_back(experience_add);
		attack_add += 10;
		HP_add += 100;
		experience_add += 25;
	}

	hero.hero_exp = 0;
	hero.level = 1;

	hero.attack = get_hero_atk(id);
	hero.HP = get_hero_hp(id);
	hero.FullHP = get_hero_hp(id);
	hero.defect = get_hero_def(id);
	hero.MP = get_hero_def(id);
	hero.FullMP = get_hero_def(id);
	hero.ap = get_hero_ap(id);
	hero.ap_def = get_hero_ap_def(id);
	hero.attack_distance = get_hero_attack_distance(id);
	hero.rehp = 20;
	hero.money = 500;
	hero.move_speed = 500;
	hero.survival = true;

	//给红蓝方不同颜色的血条
	if (id >= 0 && id <= 2)
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
		blood->setPosition(Vec2(50, 200));
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

int HeroSprite::myrand()
{
	srand((unsigned)time(NULL));
	int ai_id = rand() % 3 + 3;

	if (ai_id == 4)//亚瑟由于攻击距离短，ai会有bug
	{
		++ai_id;
	}

	return ai_id;
}



