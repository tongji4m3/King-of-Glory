#ifndef _SOLDIER_H_
#define _SOLDIER_H_

class Soldier
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
};

#endif