#include"SoldierSprite.h"
#include<string>
#include<vector>

//读取近程远程炮车初始物攻数据并按时间进行递增                             //改↓
int SoldierSprite::get_soldier_atk(int id,int time_minute)
{
	std::vector<int>soldier_atk = { 273,260,350 };
	return (soldier_atk[id % 3] + time_minute * 10);
	
}

//读取近程远程炮车初始防御力并按时间进行递增
int SoldierSprite::get_soldier_def(int id,int time_minute)
{
	std::vector<int>soldier_def = { 45,40,50 };
	return (soldier_def[id % 3] + time_minute * 5);
}

//读取近程远程炮车初始生命值并按时间进行递增
int SoldierSprite::get_soldier_hp(int id,int time_minute)
{
	std::vector<int>soldier_hp = { 1800,1600,3200 };
	return (soldier_hp[id % 3] + time_minute * 100);
}                                                                          //改↑

SoldierSprite::SoldierSprite(int id,int time_minute)//改
{
	soldier.attack = get_soldier_atk(id,time_minute);//改
	soldier.HP = get_soldier_hp(id,time_minute);//改
	soldier.defect = get_soldier_def(id, time_minute);//改
	soldier.attack_distance = 100;
	soldier.survival = true;

	//给红蓝方不同颜色的血条
	if (id == 0 || id == 1|| id == 2 ) //改
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
	if (id == 2 || id == 5)//改
	{
		soldier.attack_distance = 300;
	}
}

SoldierSprite* SoldierSprite::create(int id,int time_minute)//改
{
	SoldierSprite* soldierSprite = new SoldierSprite(id,time_minute);//改
	soldierSprite->init(id);
	soldierSprite->autorelease();
	return soldierSprite;
}
bool SoldierSprite::init(int id)
{
	//很奇怪，soldier3命名创建炮车会创建成远程
	std::vector<std::string> soldier_image =
	{
		"soldiers\\soldier1_blue.png"  ,
		"soldiers\\soldier2_blue.png"  ,//改   远程
		"soldiers\\soldier4_blue.png"  ,//改   炮车
		"soldiers\\soldier1_red.png"   ,
		"soldiers\\soldier2_red.png"   ,//改
		"soldiers\\soldier4_red.png"    //改
	};

	//添加小兵
	Sprite::initWithFile(soldier_image[id]);
	return true;
}


