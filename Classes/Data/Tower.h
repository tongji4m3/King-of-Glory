/*
*此类定义了防御塔具有的各种属性
*具体包含的内容见下方注释
*/
#ifndef _TOWER_H_
#define _TOWER_H_

class Tower
{
public:
	int attack;//攻击力
	int defect;//防御力   
	double HP;//血量
	int attack_distance;//攻击范围
	bool survival;//是否存活
	bool red;//是否是红方的
};

#endif