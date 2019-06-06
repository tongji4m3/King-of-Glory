#include"TowerSprite.h"
#include<string>
#include<vector>

//读取水晶炮塔初始攻击               //改↓
int TowerSprite::get_tower_atk(int id)
{
	std::vector<int>tower_atk = { 724 ,540 };
	return tower_atk[id % 2];
}

//读取水晶炮塔初始防御力 
int TowerSprite::get_tower_def(int id)
{
	std::vector<int>tower_def = { 200,300 };
	return tower_def[id % 2];
}

//读取水晶炮塔初始生命值 
int TowerSprite::get_tower_hp(int id)
{
	std::vector<int>tower_hp = { 9000,6000 };
	return tower_hp[id % 2];
}                                             //改↑          


//4座塔0,1,是蓝方，2，3是红方
TowerSprite::TowerSprite(int id) 
{
	tower.attack = get_tower_atk(id);//改
	tower.HP = get_tower_hp(id);//改
	tower.defect = get_tower_def(id);//改
	tower.attack_distance = 500;
	tower.survival = true;

	//给红蓝方不同颜色的血条
	if (id == 0 || id == 1)
	{
		blood = Sprite::create("blood_bar&blue_bar/bule_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		tower.red = false;
	}
	else
	{
		blood = Sprite::create("blood_bar&blue_bar/red_h_x.png");
		this->addChild(blood);
		blood->setScaleY(0.8);
		blood->setScaleX(0.5);
		tower.red = true;
	}

	//微调血条的位置
	switch (id)
	{
	case 0:
		blood->setPosition(Vec2(100, 250));
		break;
	case 1:
		blood->setPosition(Vec2(50, 300));//改
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


