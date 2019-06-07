#ifndef  _SOLDIER_SPRITE_H_
#define  _SOLDIER_SPRITE_H_

#include"../Data/Soldier.h"
#include "cocos2d.h"

USING_NS_CC;

class SoldierSprite :public Sprite
{
public:
	Soldier soldier;
	SoldierSprite(int id);
	static SoldierSprite* create(int id);
	bool init(int id);
	cocos2d::Sprite* blood;
};

#endif