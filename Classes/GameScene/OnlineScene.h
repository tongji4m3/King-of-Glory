/*
*联机版本的，游戏主场景
*基本上是在单机版本上进行加工
*通过client，server实现信息交互联机
*主要工作量在于红方场景绘制，基本与蓝方对称相反
*还有就是信息交互之间也比较麻烦
*/
#ifndef _ONLINE_SCENE_H_
#define _ONLINE_SCENE_H_

#include "cocos2d.h"
#include"../Sprite/TowerSprite.h"
#include"../Sprite/HeroSprite.h"
#include"../Sprite/SoldierSprite.h"
#include"../Sprite/AttackEffect.h"

#include"../NetWork/Client.h"

#include<vector>

class OnlineScene :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(Client* client);

	virtual bool init();

	CREATE_FUNC(OnlineScene);

private:
	void addTower(bool is_red);  //添加防御塔，与单机的有区别
	void time(float);      //显示时间，出兵线
	void sport(float);     //是否能够运动
	void ai_hero_attack(float);
	void gameOver(float);
	void tower_attack(float);
	void resurrection(float);	//复活相关
	void OnlineScene::display_resurrection(float); //即刻显示复活标签
	void OnlineScene::ai_soldiers_attack(float); //小兵ai

	//商店页面
	void OpenShop(Ref* pSender);		//打开商店界面            
	void CloseShop(Ref* pSender);       //关闭商店界面       
	void CloseShop_1(Ref* pSender);       //从第二页关闭商店界面       
	void second_page(Ref* pSender);     //商店第二页
	void first_page(Ref* pSender);     //商店第一页
	bool create_shop();//创建商店按钮

	//联机核心两个函数
	void send_message(float);
	void update(float);

	//经验等级系统
	bool hero_survive;//敌方英雄是否存活
	bool tower_survive_1;//敌方一塔是否存活
	bool tower_survive_2;//敌方二塔是否存活
	Label* hero_name_label;//显示英雄名字
	Label* experience_label;//显示经验
	Label* level_label;//显示等级
	void level(float);
	void experience(float);
	void money(float);
	
	/*
		函数重载，适用与各种掉血，如敌我防御塔，小兵，英雄等
		第一个参数为掉血的对象，第二个参数为掉血值（攻击力)
	*/
	void dropBlood(HeroSprite* hero, int attack);//英雄掉血         
	void dropBlood(TowerSprite* tower, int attack, int id);//塔掉血              
	void dropBlood(SoldierSprite* soldier, int attack, int id);//小兵掉血          
	void reBlood(HeroSprite* hero);//英雄泉水回血          

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
	int ai_id = 0;
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

	//战绩页面相关
	int killed_hero_result;//击杀英雄数
	int killed_soldier_result;//击杀小兵数
	int destroyed_tower_result;//摧毁塔数
	int be_killed_result;//被击杀数
	bool myhero_survive;
	bool aihero_survive;
	void OnlineScene::myhero_be_killed(float);//计算英雄被击杀数
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

