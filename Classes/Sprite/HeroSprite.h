/*
*英雄精灵类
*将英雄id与对应的英雄图片绑定，将血条与英雄绑定
*得到不同英雄的不同属性值
*/
#ifndef  _HERO_SPRITE_H_
#define  _HERO_SPRITE_H_

#include"../Data/Hero.h"
#include "cocos2d.h"
#include <cstdlib> 
#include <ctime> 

USING_NS_CC;

class HeroSprite :public Sprite
{
public:
	Hero hero;
	HeroSprite(int id);
	static HeroSprite* create(int id);
	bool init(int id);
	static int myrand();
	static int get_hero_atk(int id);
	static int get_hero_hp(int id);
	static int get_hero_def(int id);
	static int get_hero_mp(int id);
	static int get_hero_ap(int id);
	static int get_hero_ap_def(int id);
	static int get_hero_attack_distance(int id);
	cocos2d::Sprite* blood;
};

#endif
