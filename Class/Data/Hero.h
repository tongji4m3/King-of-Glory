#ifndef _HERO_H_
#define _HERO_H_

#include<vector>
#include<string>

class Hero
{
public:
	int attack;//攻击力
	double HP;//血量
	int attack_distance;//攻击范围
	bool survival;//是否存活
	bool red;//是否是红方的

	bool can_move = true;//能否移动
	double move_speed;  //移动速度
	int attack_speed;   //攻击速度

	int experience;  //经验
	int killed;      //杀敌数
	int be_killed;   //死亡数
	int money;       //拥有的金币
	int resurrection_time;  //复活时间
	std::vector<std::string> equipments;//拥有的装备
};

#endif