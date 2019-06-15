/*
*此类定义了英雄具有的各种属性
*具体包含的内容见下方注释
*/
#ifndef _HERO_H_
#define _HERO_H_

#include<vector>
#include<string>
#include<cocos2d.h>

USING_NS_CC;

class Hero
{
public:
	int attack;//攻击力
	int defect;//防御力     
	int ap_def;//法抗       
	int ap;//法强         
	int rehp;//回血      
	double HP;//血量
	double FullHP;//满血量    
	double MP;//蓝             
	double FullMP;//满蓝量         
	int attack_distance;//攻击范围
	bool survival;//是否存活
	bool red;//是否是红方的
	bool armour_effect = false;//防御甲减速效果是否获得
	bool can_move = true;//能否移动
	double move_speed;  //移动速度
	int attack_speed;   //攻击速度
	int time = 61;        //用于持续性效果开始时间的计时  

	int level;//等级
	int hero_exp;//当前经验值
	std::vector<int> level_add_attack;//每一次升级提升的攻击力
	std::vector<double>  level_add_HP;//每一次升级提升的血量
	std::vector<int> level_add_experience;//每一次升级提升的所需要的经验值

	int killed;      //杀敌数
	int be_killed;   //死亡数
	int money;       //拥有的金币

	int resurrection_time = 10;  //复活时间
	std::vector<Menu*>equipments;//拥有的装备 6件装备6个对象   
	std::vector<Sprite*> equipmentsImages;//拥有的装备购买后的精灵图像 //6件装备共需12个对象    
	std::vector<int> equipmentsImages_2;//第商店装备栏显示的装备是否为大剑   //6个对象  
};

#endif
