/*
*防御塔精灵类
*将防御塔id与对应的防御塔图片绑定，将血条与防御塔绑定
*得到不同防御塔的不同属性值
*/
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
	static int get_tower_atk(int id);
	static int get_tower_hp(int id);
	static int get_tower_def(int id);
	bool init(int id);
	cocos2d::Sprite* blood;
};

#endif