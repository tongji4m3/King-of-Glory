#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"
#include"../Sprite/TowerSprite.h"
#include"../Sprite/HeroSprite.h"
#include<vector>

class MainScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(MainScene);

private:
	void addTower();

	cocos2d::Sprite* map;//地图
	cocos2d::Layer* map_layer;//地图的图层，防御塔也放在上面
	cocos2d::Label* time_label;//显示游戏的时间
	cocos2d::Label* time_resurrection_label;//显示英雄死亡的复活时间
	cocos2d::Sprite* win;//胜利场景
	cocos2d::Sprite* fail;//失败场景
	std::vector<TowerSprite*> towers;
	HeroSprite* my_hero;
};

#endif