#ifndef _MAIN_SCENE_H_
#define _MAIN_SCENE_H_

#include "cocos2d.h"
#include"../Sprite/TowerSprite.h"
#include"../Sprite/HeroSprite.h"
#include"../Sprite/SoldierSprite.h"
#include"../Sprite/AttackEffect.h"
#include"../Sprite/ShopMenu.h"
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
	void OpenShop(Ref* pSender);		//打开商店界面            //改↓
	void CloseShop(Ref* pSender);       //关闭商店界面       
	void CloseShop_1(Ref* pSender);       //从第二页关闭商店界面       
	void second_page(Ref* pSender);     //商店第二页
	void first_page(Ref* pSender);     //商店第一页
	bool create_shop();//创建商店按钮
	

	                                                              //改↑
	/*
		函数重载，试用与各种掉血，如敌我防御塔，小兵，英雄等
		第一个参数为掉血的对象，第二个参数为掉血值（攻击力)
	*/
	void dropBlood(HeroSprite* hero,int attack);//英雄掉血         //改
	void dropBlood(TowerSprite* tower,int attack,int id);//塔掉血              //改
	void dropBlood(SoldierSprite* soldier,int attack,int id);//小兵掉血          //改
	void reBlood(HeroSprite* hero);//英雄泉水回血          //改

	cocos2d::Sprite* map;//地图
	cocos2d::Layer* map_layer;//地图的图层，防御塔也放在上面
	cocos2d::Layer* shop_second_layer;//商店第二页图层  //改
	cocos2d::Sprite* shop;//商店       //改
	cocos2d::Sprite* shop_second_sprite;//商店第二页   //改
	cocos2d::Sprite* equipments_introduction;//装备介绍  //改
	cocos2d::Layer* shop_layer;//商店图层             //改
	cocos2d::Menu* shop_close_menu;//关闭商店菜单   //改
	cocos2d::Menu* shop_close_menu_1;//关闭商店菜单第二页的   //改
	cocos2d::Menu* shop_open_menu;//打开商店菜单   //改
	cocos2d::Menu* second_page_menu;//第二页商店菜单  //改
	cocos2d::Menu* first_page_menu;//第二页商店菜单  //改
	cocos2d::Menu* buy_it_menu;//购买装备                 //改
	cocos2d::Menu* red_diamond_menu;//红宝石菜单   //改
	cocos2d::Menu* blue_diamond_menu;//蓝宝石菜单    //改
	cocos2d::Menu* mp_book_menu;//法强书菜单          //改
	cocos2d::Menu* iron_sword_menu;//铁剑菜单        //改
	cocos2d::Menu* def_clothes_menu;//布衣菜单        //改
	cocos2d::Menu* def_ap_cloak_menu;//抗魔披风菜单        //改
	cocos2d::Menu* green_diamond_menu;//绿宝石菜单   //改
	cocos2d::Menu* basic_shoes_menu;//草鞋菜单   //改
	cocos2d::Menu* belt_strength_menu;//力量腰带菜单   //改
	cocos2d::Menu* belt_strength_after_red_menu;//力量腰带后菜单   //改
	cocos2d::Menu* big_stick_menu;//大棒   //改
	cocos2d::Menu* big_stick_after_book_menu;//大棒后菜单   //改
	cocos2d::Menu* def_shoes_menu;//防御鞋   //改
	cocos2d::Menu* def_shoes_after_basic_menu;//防御鞋后菜单   //改
	cocos2d::Menu* def_armour_menu; //防御甲   //改
	cocos2d::Menu* def_armour_after_clothes_menu;//防御甲后菜单   //改
	cocos2d::Menu* big_sword_menu;//大剑菜单        //改
	cocos2d::Label* time_label;//显示游戏的时间
	cocos2d::Label* money_label;//显示我方英雄金币           //改
	cocos2d::Label* money_label_shop;//在商店里显示我方英雄金币           //改
	cocos2d::Label* time_resurrection_label;//显示英雄死亡的复活时间
	cocos2d::Sprite* win;//胜利场景
	cocos2d::Sprite* fail;//失败场景
	std::vector<TowerSprite*> towers;//防御塔
	HeroSprite* my_hero;//我方英雄
	HeroSprite* ai_hero;//ai英雄
	int ai_hero_id = HeroSprite::myrand();//随机产生英雄ai的id    //改
	int time_minute = 0;//时间 分
	int time_second = 0;//时间 秒
	int red_diamond_num = 0;//购买的红水晶数量，用来判断是否需合成力量腰带   //改
	int mp_book_num = 0;//改
	int basic_shoes_num = 0;//改
	int def_clothes_num = 0;//改
	int iron_sword_num = 0;//改
	int big_sword_num = 0;//改
	std::vector<SoldierSprite*>blue_soldier_vec;//蓝色兵的vec
	std::vector<SoldierSprite*>red_soldier_vec;//红色兵的vec
	std::vector<int>vecx ;//装备栏位置x            //改
	std::vector<int>vecy ;//装备栏位置y            //改
};

#endif