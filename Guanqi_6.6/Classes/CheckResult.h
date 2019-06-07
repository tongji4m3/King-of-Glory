#ifndef __CHECK_RESULT_H__
#define __CHECK_RESULT_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"

using namespace CocosDenshion;

class CheckResult :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	SimpleAudioEngine* Sound = SimpleAudioEngine::getInstance();

	virtual bool init();
	cocos2d::Label* experience_label;//显示经验
	cocos2d::Label* level_label;//显示等级
	cocos2d::Label* money_label;//显示金币
	cocos2d::Label* killed_hero_label;//显示击杀英雄数
	cocos2d::Label* killed_soldier_label;//显示击杀小兵数
	cocos2d::Label* destroyed_tower_label;//显示摧毁塔数
	cocos2d::Label* be_killed_label;//显示被击杀数


	//转场主菜单  
	void ReturnToWelcome(Ref* pSender);

	CREATE_FUNC(CheckResult);
};
extern int money_result;       //拥有的金币
extern int level_result;//等级
extern int experience_result;//经验
extern int killed_hero_result;      //击杀英雄数
extern int killed_soldier_result;//击杀小兵数
extern int destroyed_tower_result;//摧毁塔数
extern int be_killed_result;   //死亡数

#endif