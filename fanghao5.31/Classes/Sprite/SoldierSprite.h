#ifndef  _SOLDIER_SPRITE_H_
#define  _SOLDIER_SPRITE_H_

#include"../Data/Soldier.h"
#include "cocos2d.h"

USING_NS_CC;

class SoldierSprite :public Sprite
{
public:
	Soldier soldier;
	SoldierSprite(int id,int time_minute);//改
	static SoldierSprite* create(int id,int time_minute);//改
	bool init(int id);
	cocos2d::Sprite* blood;
	static int get_soldier_atk(int id,int time_minute);//改
	static int get_soldier_hp(int id, int time_minute);//改
	static int get_soldier_def(int id,int time_minute);//改
};

#endif