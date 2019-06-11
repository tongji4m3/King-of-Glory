/*
*单机版本的，游戏主场景
*内容及其丰富，全新版本，等你来玩！
*主要包括了地图构建，英雄操作，敌方英雄AI，小兵AI，防御塔AI
*还有商店装备逻辑，死亡逻辑，金币经验等级系统等等
*详细的就不说了，代码里面注释很丰富了
*/
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
	void ai_hero_attack(float);//ai自动攻击
	void gameOver(float);//判断是否游戏结束
	void tower_attack(float);//防御塔攻击
	void resurrection(float);	//复活相关
	void MainScene::display_resurrection(float); //即刻显示复活标签
	void MainScene::ai_soldiers_attack(float); //小兵ai

	//商店的一些函数
	void OpenShop(Ref* pSender);		//打开商店界面            
	void CloseShop(Ref* pSender);       //关闭商店界面       
	void CloseShop_1(Ref* pSender);       //从第二页关闭商店界面       
	void second_page(Ref* pSender);     //商店第二页
	void first_page(Ref* pSender);     //商店第一页
	bool create_shop();//创建商店按钮

	/*
	函数重载，适用与各种掉血，如敌我防御塔，小兵，英雄等
	第一个参数为掉血的对象，第二个参数为掉血值（攻击力)
	*/
	void dropBlood(HeroSprite* hero, int attack);//英雄掉血         
	void dropBlood(TowerSprite* tower, int attack, int id);//塔掉血              
	void dropBlood(SoldierSprite* soldier, int attack, int id);//小兵掉血          
	void reBlood(HeroSprite* hero);//英雄泉水回血    

	//经验等级系统
	bool hero_survive;//敌方英雄是否存活
	bool tower_survive_1;//敌方一塔是否存活
	bool tower_survive_2;//敌方二塔是否存活
	Label* hero_name_label;//显示英雄名字
	Label* experience_label;//显示经验
	Label* level_label;//显示等级
	void MainScene::level(float);
	void MainScene::experience(float);
	void MainScene::money(float);

	//主要用于地图场景绘制，装备商店系统
	cocos2d::Sprite* map;//地图
	cocos2d::Layer* map_layer;//地图的图层，防御塔也放在上面
	cocos2d::Layer* shop_second_layer;//商店第二页图层  
	cocos2d::Sprite* shop;//商店       
	cocos2d::Sprite* shop_second_sprite;//商店第二页   
	cocos2d::Sprite* equipments_introduction;//装备介绍  
	cocos2d::Layer* shop_layer;//商店图层             
	cocos2d::Menu* shop_close_menu;//关闭商店菜单   
	cocos2d::Menu* shop_close_menu_1;//关闭商店菜单第二页的   
	cocos2d::Menu* shop_open_menu;//打开商店菜单   
	cocos2d::Menu* second_page_menu;//第二页商店菜单  
	cocos2d::Menu* first_page_menu;//第二页商店菜单  
	cocos2d::Menu* buy_it_menu;//购买装备                 
	cocos2d::Menu* red_diamond_menu;//红宝石菜单   
	cocos2d::Menu* blue_diamond_menu;//蓝宝石菜单    
	cocos2d::Menu* mp_book_menu;//法强书菜单          
	cocos2d::Menu* iron_sword_menu;//铁剑菜单        
	cocos2d::Menu* def_clothes_menu;//布衣菜单        
	cocos2d::Menu* def_ap_cloak_menu;//抗魔披风菜单        
	cocos2d::Menu* green_diamond_menu;//绿宝石菜单   
	cocos2d::Menu* basic_shoes_menu;//草鞋菜单   
	cocos2d::Menu* belt_strength_menu;//力量腰带菜单   
	cocos2d::Menu* belt_strength_after_red_menu;//力量腰带后菜单   
	cocos2d::Menu* big_stick_menu;//大棒   
	cocos2d::Menu* big_stick_after_book_menu;//大棒后菜单   
	cocos2d::Menu* def_shoes_menu;//防御鞋   
	cocos2d::Menu* def_shoes_after_basic_menu;//防御鞋后菜单   
	cocos2d::Menu* def_armour_menu; //防御甲   
	cocos2d::Menu* def_armour_after_clothes_menu;//防御甲后菜单   
	cocos2d::Menu* big_sword_menu;//大剑菜单        
	cocos2d::Label* time_label;//显示游戏的时间
	cocos2d::Label* money_label;//显示我方英雄金币           
	cocos2d::Label* money_label_shop;//在商店里显示我方英雄金币           
	cocos2d::Label* time_resurrection_label;//显示英雄死亡的复活时间
	cocos2d::Sprite* win;//胜利场景
	cocos2d::Sprite* fail;//失败场景
	cocos2d::Sprite* passive;//被动图片
	cocos2d::Sprite* passive_describe;//被动描述图片
	std::vector<TowerSprite*> towers;//防御塔
	HeroSprite* my_hero;//我方英雄
	HeroSprite* ai_hero;//ai英雄
	int ai_hero_id = HeroSprite::myrand();//随机产生英雄ai的id    
	int time_minute = 0;//时间 分
	int time_second = 0;//时间 秒
	int red_diamond_num = 0;//购买的红水晶数量，用来判断是否需合成力量腰带   
	int mp_book_num = 0;
	int basic_shoes_num = 0;
	int def_clothes_num = 0;
	int iron_sword_num = 0;
	int big_sword_num = 0;
	std::vector<SoldierSprite*>blue_soldier_vec;//蓝色兵的vec
	std::vector<SoldierSprite*>red_soldier_vec;//红色兵的vec
	std::vector<int>vecx;//装备栏位置x            
	std::vector<int>vecy;//装备栏位置y   

	//主要用于战绩页面查看
	bool myhero_survive;
	bool aihero_survive;
	int killed_hero_result;//击杀英雄数
	int killed_soldier_result;//击杀小兵数
	int destroyed_tower_result;//摧毁塔数
	int be_killed_result;//被击杀数
	void MainScene::myhero_be_killed(float);//计算英雄被击杀数
	void CheckResult(Ref* pSender);
	void CloseResult(Ref* pSender);
	bool create_CombatResult();
	cocos2d::Menu* open_combat_result;
	cocos2d::Menu* close_combat_result;
	cocos2d::Layer* result_layer;
	cocos2d::Sprite* result;
	cocos2d::Sprite* avatar_image;
	cocos2d::Layer* hero_image;
	cocos2d::Label* HP_label;//显示生命值
	cocos2d::Label* level_result_label;//显示等级
	cocos2d::Label* attack_label;//显示攻击力
	cocos2d::Label* killed_hero_label;//显示击杀英雄数
	cocos2d::Label* killed_soldier_label;//显示击杀小兵数
	cocos2d::Label* destroyed_tower_label;//显示摧毁塔数
	cocos2d::Label* be_killed_label;//显示被击杀数
};

#endif
