#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"
#include"../Sprite/TowerSprite.h"
#include"../Sprite/HeroSprite.h"
#include"../Sprite/SoldierSprite.h"
#include"../Sprite/AttackEffect.h"
#include<vector>

class MainScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(MainScene);

private:
	void addTower();       //添加防御塔
	void time(float);      //显示时间，出兵线
	void sport(float);     //是否能够运动
	void ai_hero_attack(float);
	void gameOver(float);
	void tower_attack(float);
	void resurrection(float);	//复活相关
	void MainScene::display_resurrection(float); //即刻显示复活标签
	void MainScene::ai_soldiers_attack(float); //小兵ai
	//改：void MainScene::onPushSceneCheckResult(float dt);

	/*
		函数重载，试用与各种掉血，如敌我防御塔，小兵，英雄等
		第一个参数为掉血的对象，第二个参数为掉血值（攻击力)
	*/
	void dropBlood(HeroSprite* hero,int attack);//英雄掉血
	void dropBlood(TowerSprite* tower,int attack);//塔掉血
	void dropBlood(SoldierSprite* soldier,int attack);//小兵掉血

	cocos2d::Sprite* map;//地图
	cocos2d::Layer* map_layer;//地图的图层，防御塔也放在上面
	cocos2d::Label* time_label;//显示游戏的时间
	cocos2d::Label* time_resurrection_label;//显示英雄死亡的复活时间
	cocos2d::Sprite* win;//胜利场景
	cocos2d::Sprite* fail;//失败场景
	std::vector<TowerSprite*> towers;//防御塔
	HeroSprite* my_hero;//我方英雄
	HeroSprite* ai_hero;//ai英雄
	int time_minute = 0;//时间 分
	int time_second = 0;//时间 秒
	std::vector<SoldierSprite*>blue_soldier_vec;//蓝色兵的vec
	std::vector<SoldierSprite*>red_soldier_vec;//红色兵的vec
};

#endif