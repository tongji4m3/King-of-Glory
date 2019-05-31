#ifndef  _HERO_SPRITE_H_
#define  _HERO_SPRITE_H_

#include"../Data/Hero.h"
#include "cocos2d.h"
#include <cstdlib> // Header file needed to use srand and rand    //改
#include <ctime> // Header file needed to use time              //改

USING_NS_CC;

class HeroSprite :public Sprite
{
public:
	Hero hero;
	HeroSprite(int id);
	static HeroSprite* create(int id);
	bool init(int id);
	static int myrand();//改
	static int get_hero_atk(int id);//改
	static int get_hero_hp(int id);//改
	static int get_hero_def(int id);//改
	static int get_hero_mp(int id);//改
	static int get_hero_ap(int id);//改
	static int get_hero_ap_def(int id);//改
	cocos2d::Sprite* blood;
};

#endif