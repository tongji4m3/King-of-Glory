#ifndef _HERO_H_
#define _HERO_H_

#include<vector>
#include<string>

class Hero
{
public:
	std::vector<int> attack;//攻击力
	std::vector<double>  HP;//血量
	std::vector<int> experience;//每一等级升级经验

	int attack_distance;//攻击范围
	bool survival;//是否存活
	bool red;//是否是红方的

	bool can_move = true;//能否移动
	double move_speed;  //移动速度
	int attack_speed;   //攻击速度
	int level;//等级
	int hero_exp;//当前经验
	int killed;      //杀敌数
	int be_killed;   //死亡数
	int money;       //拥有的金币
	int resurrection_time=10;  //复活时间
	std::vector<std::string> equipments;//拥有的装备
};

#endif