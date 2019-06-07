#ifndef  _TOWER_SPRITE_H_
#define  _TOWER_SPRITE_H_

#include"../Data/Tower.h"
#include "cocos2d.h"

USING_NS_CC;

class TowerSprite :public Sprite
{
public:
	Tower tower;
	TowerSprite(int id);
	static TowerSprite* create(int id);
	bool init(int id);
	cocos2d::Sprite* blood;
};

#endif