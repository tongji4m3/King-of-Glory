#ifndef _HERO_H_
#define _HERO_H_

#include<vector>
#include<string>
#include<cocos2d.h>//改
USING_NS_CC;//改

class Hero
{
public:
	int attack;//攻击力
	int defect;//防御力     //改
	int ap_def;//法抗       //改
	int ap;//法强         //改
	double HP;//血量
	double FullHP;//满血量    //改
	double MP;//蓝             //改
	double FullMP;//满蓝量         //改
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
	int resurrection_time=10;  //复活时间
	std::vector<Menu*>equipments;//拥有的装备 6件装备6个对象   //改
    std::vector<Sprite*> equipmentsImages;//拥有的装备购买后的精灵图像 //6件装备共需12个对象    //改
};

#endif