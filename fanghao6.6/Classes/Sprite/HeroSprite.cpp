#include"HeroSprite.h"
#include<string>
#include<vector>

//读取妲己亚瑟后羿初始物攻数据                   //改↓
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
}                                             //改↑          


HeroSprite::HeroSprite(int id)//改
{
	hero.attack = get_hero_atk(id);//改
	hero.HP = get_hero_hp(id);//改
	hero.FullHP = get_hero_hp(id);//改
	hero.defect = get_hero_def(id);//改
	hero.MP = get_hero_def(id);//改
	hero.FullMP = get_hero_def(id);//改
	hero.ap = get_hero_ap(id);//改
	hero.ap_def = get_hero_ap_def(id);//改
	hero.attack_distance = 400;
	hero.rehp = 20;//改
	hero.money = 10000;//改
	hero.move_speed = 500;//改
	hero.survival = true;

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

 int HeroSprite::myrand()          //改↓
{
	srand((unsigned)time(NULL));
	int ai_id = rand() % 3 + 3;
	return ai_id;
}                                  //改↑


