/*
*攻击特效类
*根据不同的英雄id绑定不同的攻击特效
*/
#ifndef  _ATTACK_EFFECT_H_
#define  _ATTACK_EFFECT_H_

#include "cocos2d.h"

USING_NS_CC;

//攻击特效
class AttackEffect :public Sprite
{
public:
	static AttackEffect* create(int id);
	void init(int id);
};

#endif