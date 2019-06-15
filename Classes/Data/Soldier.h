/*
*此类定义了小兵具有的各种属性
*具体包含的内容见下方注释
*/
#ifndef _SOLDIER_H_
#define _SOLDIER_H_

class Soldier
{
public:
	int attack;//攻击力
	int defect;//防御力    
	double HP;//血量
	int attack_distance;//攻击范围
	bool survival;//是否存活
	bool red;//是否是红方的
	bool can_move = true;//能否移动
	double move_speed;  //移动速度
	int attack_speed=0;   //攻击速度
	int time = 61;        //用于持续性效果开始时间的计时      
};

#endif