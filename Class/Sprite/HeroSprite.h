#ifndef  _HERO_SPRITE_H_
#define  _HERO_SPRITE_H_

#include"../Data/Hero.h"
#include "cocos2d.h"

USING_NS_CC;

class HeroSprite :public Sprite
{
public:
	Hero hero;
	HeroSprite(int id);
	static HeroSprite* create(int id);
	bool init(int id);
	cocos2d::Sprite* blood;
};

#endif