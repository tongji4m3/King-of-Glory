#include"MainScene.h"
#include"SimpleAudioEngine.h"
#include<iostream>
#include<vector>
#include<string>

using namespace CocosDenshion;//播放音乐要用的

USING_NS_CC;

Scene* MainScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

extern int id;//获取之前选的英雄类型  妲己，亚瑟，后羿

bool  MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//循环播放音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm/back.mp3", true);

	//载入地图
	map_layer = Layer::create();
	this->addChild(map_layer);
	map = Sprite::create("map/map.png");
	map_layer->addChild(map);
	map_layer->setAnchorPoint(Vec2(0, 0));
	map->setAnchorPoint(Vec2(0, 0));

	//读入商店里装备栏的位置参数                     
	vecx = { 572,749,926,1103,1280,1457 };
	vecy = { 113 };

	//添加商店按钮         
	create_shop();

	//玩家
	my_hero = HeroSprite::create(id);
	addChild(my_hero);
	my_hero->setPosition(Vec2(960, 540));
	my_hero->setAnchorPoint(Vec2(0.5, 0.5));

	//添加防御塔
	addTower();

	//显示战绩界面
	create_CombatResult();

	//击杀数设为0
	int killed_hero_result=0;//击杀英雄数
	int killed_soldier_result=0;//击杀小兵数
	int destroyed_tower_result=0;//摧毁塔数
	int be_killed_result = 0;//被击杀数

	//载入时间
	time_label = Label::create("00:00", "Arial", 50);
	time_label->setColor(Color3B(200, 150, 156));
	this->addChild(time_label);
	time_label->setPosition(Vec2(960, 960));

	//载入金币                                                                               
	money_label = Label::create(std::to_string(my_hero->hero.money), "Arial", 50);
	money_label->setColor(Color3B(255, 215, 0));
	this->addChild(money_label);
	money_label->setPosition(Vec2(120, 460));

	//显示等级
	level_label = Label::create("LEVEL: 1", "Arial", 40);
	level_label->setColor(Color3B(50, 200, 200));
	this->addChild(level_label);
	level_label->setPosition(Vec2(120, 960));

	//显示经验
	experience_label = Label::create("EXP: 0 / 100", "Arial", 40);
	experience_label->setColor(Color3B(50, 200, 200));
	this->addChild(experience_label);
	experience_label->setPosition(Vec2(120, 880));

	//英雄名字
	std::string hero_name;
	if (id == 0)
	{
		hero_name = "DAJI";
	}
	else if(id == 1)
	{
		hero_name = "YASE";
	}
	else
	{
		hero_name = "HOUYI";
	}

	hero_name_label = Label::create(hero_name, "fonts/Marker Felt.ttf", 40);
	hero_name_label->setColor(Color3B(50, 200, 200));
	this->addChild(hero_name_label);
	hero_name_label->setPosition(Vec2(120, 780));

	//英雄死亡时的复活时间 死亡时才可见
	int time_resurrection = 10;
	std::string resurrection("Resurrection time: ");
	resurrection += std::to_string(time_resurrection);
	time_resurrection_label = Label::create(resurrection, "Arial", 50);
	time_resurrection_label->setColor(Color3B(200, 150, 156));
	this->addChild(time_resurrection_label);
	time_resurrection_label->setPosition(Vec2(960, 540));
	time_resurrection_label->setVisible(false);

	//胜利场景 胜利时可见
	win = Sprite::create("win_or_lose/win.png");
	this->addChild(win);
	win->setPosition(Vec2(960, 540));
	win->setVisible(false);

	//失败场景 失败时可见
	fail = Sprite::create("win_or_lose/lose.png");
	this->addChild(fail);
	fail->setPosition(Vec2(960, 540));
	fail->setVisible(false);

	//被动图片
	if (id == 0)
	{
		passive = Sprite::create("passive/daji_passive_png.png");
	}
	else if (id == 1)
	{
		passive = Sprite::create("passive/yase_passive_png.png");
	}
	else
	{
		passive = Sprite::create("passive/houyi_passive_png.png");
	}
	this->addChild(passive);
	passive->setPosition(Vec2(1650, 500));
	
	passive->setVisible(true);

	//被动描述图片
	if (id == 0)
	{
		my_hero->hero.attack += my_hero->hero.ap;
		passive_describe = Sprite::create("passive/daji_passive.png");
	}
	else if (id == 1)
	{
		my_hero->hero.defect += 100;
		passive_describe = Sprite::create("passive/yase_passive.png");
	}
	else
	{
		my_hero->hero.attack += 50;
		passive_describe = Sprite::create("passive/houyi_passive.png");
	}
	this->addChild(passive_describe);
	passive_describe->setAnchorPoint(Vec2(1, 0));
	passive_describe->setPosition(Vec2(1900, 80));
	passive_describe->setVisible(true);

	//敌人
	int ai_id = MainScene::ai_hero_id;//产生随机数,随机ai英雄    
	ai_hero = HeroSprite::create(ai_id);
	map_layer->addChild(ai_hero);
	ai_hero->setPosition(Vec2(6000, 700));

	//单点触摸事件监听器 控制我方英雄
	auto m = EventListenerTouchOneByOne::create();
	//[=]按值传递捕获当前上下文的所有变量
	m->onTouchBegan =
		[=](Touch* touch, Event* e)
	{
		//convertToNodeSpace 将Vec2转换为节点（本地）空间坐标
		auto touch_position_change = convertToNodeSpace(touch->getLocation());//鼠标点的位置
		auto my_hero_position_change = convertToNodeSpace(my_hero->getPosition());//英雄原来的位置
		//终止之前的操作
		my_hero->stopAllActions();
		map_layer->stopAllActions();

		auto my_hero_position = my_hero->getPosition();
		auto ai_hero_position = ai_hero->getPosition();
		auto map_layer_position = map_layer->getPosition();
		auto touch_position = Vec2(touch->getLocation().x - map_layer_position.x, touch->getLocation().y);

		//攻击操作
		//因为点一下触发一次，所以攻击速度可以无穷大..
		if (my_hero->hero.survival)
		{
			//对于所有红色兵
			for (auto& i : red_soldier_vec)
			{
				/*
				如果点击了小兵，红色小兵如果在攻击范围之内 并且小兵是存活的
				制作攻击特效，并且让小兵掉血
				在一个listener中，我们在判断触摸点是否在精灵上面时，采用的方式为：
				it->boundingBox().containsPoint(t->getLocation())
				其中it为精灵指针，t为listener的touch对象指针，通过这种方式就可以判断点击范围是否在精灵范围内。
				*/
				if (i->boundingBox().containsPoint(touch_position) && i->getPosition().distance(my_hero_position - map_layer_position) <= my_hero->hero.attack_distance && i->soldier.survival == true)
				{
					int soldier_id = 2;//初始为2，即炮车     
					if (i == red_soldier_vec[0])
					{
						soldier_id = 0;
					}
					if (i == red_soldier_vec[1])
					{
						soldier_id = 1;
					}
					soldier_id += 3;//区分敌我小兵，补兵时可以判断
					auto hero_attack_effect = AttackEffect::create(id);
					map_layer->addChild(hero_attack_effect);
					hero_attack_effect->setScale(0.5);
					hero_attack_effect->setPosition(my_hero->getPosition() - map_layer_position);
					auto call = CallFuncN::create([=](Node* t)
						{
							if (i->soldier.survival == true)
							{
								dropBlood(i, my_hero->hero.attack, soldier_id);
							}
						});
					//飞行效果
					//动作序列(Sequence) 是一种封装多个动作的对象，当这个对象执行时被封装的动作会顺序执行
					//先飞行，再掉血，再逐渐消失
					hero_attack_effect->runAction(
						Sequence::create(MoveTo::create((my_hero_position - map_layer_position).distance(i->getPosition()) / 200, i->getPosition()),
							call, FadeOut::create(0.1), 0));
					return false;
				}
			}

			//攻击敌方英雄
			if (ai_hero->boundingBox().containsPoint(touch_position) && ai_hero->getPosition().distance(my_hero_position - map_layer_position) <= my_hero->hero.attack_distance && ai_hero->hero.survival == true)
			{
				auto hero_attack_effect = AttackEffect::create(id);
				map_layer->addChild(hero_attack_effect);
				hero_attack_effect->setScale(0.5);
				hero_attack_effect->setPosition(my_hero->getPosition() - map_layer_position);
				if (ai_hero->hero.armour_effect == true)                       //防御甲减速攻击方效果
				{
					my_hero->hero.move_speed = my_hero->hero.move_speed * 0.85;
					my_hero->hero.time = time_second;
				}
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(ai_hero, my_hero->hero.attack);
					});
				hero_attack_effect->runAction(
					Sequence::create(MoveTo::create((my_hero_position - map_layer_position).distance(ai_hero->getPosition()) / 200, ai_hero->getPosition()),
						call, FadeOut::create(0.1), 0));
				return false;
			}

			//攻击塔
			for (int i = 2; i <= 3; ++i)
			{
				if (towers[i]->boundingBox().containsPoint(touch_position) && towers[i]->getPosition().distance(my_hero_position - map_layer_position) <= my_hero->hero.attack_distance && towers[i]->tower.survival == true)
				{
					auto hero_attack_effect = AttackEffect::create(id);
					map_layer->addChild(hero_attack_effect);
					hero_attack_effect->setScale(0.5);
					hero_attack_effect->setPosition(my_hero->getPosition() - map_layer_position);
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(towers[i], my_hero->hero.attack, i);
						});
					hero_attack_effect->runAction(
						Sequence::create(MoveTo::create((my_hero_position - map_layer_position).distance(towers[i]->getPosition()) / 200, towers[i]->getPosition()),
							call, FadeOut::create(0.1), 0));
					return false;
				}
			}
		}
		/*
		英雄移动与图层的移动，保证操控的英雄始终在屏幕x轴的中心
		具体操作是英雄负责上下移动，图层负责左右移动
		MoveBy是从当前坐标点移动给定的坐标点这么多的距离
		可以通过speed控制移动速度
		参数：
			duration：动作持续时间。
			deltaPosition：偏移坐标。这里会给出一对儿(x, y)坐标值
		*/

		my_hero->runAction(MoveBy::create(touch_position_change.distance(e->getCurrentTarget()->getPosition()) / my_hero->hero.move_speed, Vec2(0, touch_position_change.y - my_hero_position_change.y)));
		map_layer->runAction(MoveBy::create(touch_position_change.distance(e->getCurrentTarget()->getPosition()) / my_hero->hero.move_speed, Vec2((my_hero_position_change.x - touch_position_change.x), 0)));

		return false;
	};
	map_layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m, my_hero);

	//第二个参数为回调的间隔时间，如果不传参数，默认是每一帧
	schedule(schedule_selector(MainScene::time), 1);
	schedule(schedule_selector(MainScene::sport));
	schedule(schedule_selector(MainScene::ai_hero_attack), 1.0f);//攻击速度写死了
	schedule(schedule_selector(MainScene::gameOver));
	schedule(schedule_selector(MainScene::tower_attack), 1.0f);
	schedule(schedule_selector(MainScene::resurrection), 1.0f);
	schedule(schedule_selector(MainScene::display_resurrection));
	schedule(schedule_selector(MainScene::ai_soldiers_attack));//攻击速度写死了
	schedule(schedule_selector(MainScene::level));
	schedule(schedule_selector(MainScene::experience));
	schedule(schedule_selector(MainScene::money));
	schedule(schedule_selector(MainScene::myhero_be_killed));

	return true;
}

//小兵ai
void MainScene::ai_soldiers_attack(float)
{
	//对于蓝方每个小兵
	for (auto blue_iter = blue_soldier_vec.begin(); blue_iter != blue_soldier_vec.end(); ++blue_iter)
	{
		bool blue_flag = true;
		for (auto red_iter = red_soldier_vec.begin(); red_iter != red_soldier_vec.end(); ++red_iter)
		{
			if ((*blue_iter)->getPosition().distance((*red_iter)->getPosition()) < (*blue_iter)->soldier.attack_distance && (*red_iter)->soldier.survival == true && (*blue_iter)->soldier.survival == true)
			{
				int soldier_id = 2;//默认值为2，即炮车                           
				if ((red_iter - red_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((red_iter - red_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				blue_flag = false;
				if ((*blue_iter)->soldier.attack_speed == 100)//因为为每秒刷新，所以就b_num就高一点
				{
					auto tower_attack_effect = AttackEffect::create(3);//小兵攻击特效
					map_layer->addChild(tower_attack_effect);
					tower_attack_effect->setPosition((*blue_iter)->getPosition());
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(*red_iter, (*blue_iter)->soldier.attack, soldier_id);
						});
					tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, (*red_iter)->getPosition()), call, FadeOut::create(0.1), 0));
					(*blue_iter)->soldier.attack_speed = 0;
				}
				else
				{
					++(*blue_iter)->soldier.attack_speed;
				}
			}
		}
		if ((*blue_iter)->getPosition().distance(towers[3]->getPosition()) <= (*blue_iter)->soldier.attack_distance && towers[3]->tower.survival == true && (*blue_iter)->soldier.survival == true)
		{
			blue_flag = false;
			if ((*blue_iter)->soldier.attack_speed == 100)//因为为每秒刷新，所以就b_num就高一点
			{
				auto tower_attack_effect = AttackEffect::create(3);//小兵攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*blue_iter)->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(towers[3], (*blue_iter)->soldier.attack, 3);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, towers[3]->getPosition()), call, FadeOut::create(0.1), 0));
				(*blue_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*blue_iter)->soldier.attack_speed;
			}
		}
		else if ((*blue_iter)->getPosition().distance(towers[2]->getPosition()) <= (*blue_iter)->soldier.attack_distance && towers[2]->tower.survival == true && (*blue_iter)->soldier.survival == true)
		{
			blue_flag = false;
			if ((*blue_iter)->soldier.attack_speed == 100)//因为为每秒刷新，所以就b_num就高一点
			{
				auto tower_attack_effect = AttackEffect::create(3);//小兵攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*blue_iter)->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(towers[2], (*blue_iter)->soldier.attack, 2);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, towers[2]->getPosition()), call, FadeOut::create(0.1), 0));
				(*blue_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*blue_iter)->soldier.attack_speed;
			}
		}
		else if ((*blue_iter)->getPosition().distance(ai_hero->getPosition()) <= (*blue_iter)->soldier.attack_distance && ai_hero->hero.survival == true && (*blue_iter)->soldier.survival == true)
		{
			blue_flag = false;
			if ((*blue_iter)->soldier.attack_speed == 100)//因为为每秒刷新，所以就b_num就高一点
			{
				auto tower_attack_effect = AttackEffect::create(3);//小兵攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*blue_iter)->getPosition());
				if (ai_hero->hero.armour_effect == true)                       //防御甲减速攻击方效果
				{
					(*blue_iter)->soldier.move_speed = (*blue_iter)->soldier.move_speed * 0.85;
					(*blue_iter)->soldier.time = time_second;
				}
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(ai_hero, (*blue_iter)->soldier.attack);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, ai_hero->getPosition()), call, FadeOut::create(0.1), 0));
				(*blue_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*blue_iter)->soldier.attack_speed;
			}
		}

		if (blue_flag == true)
		{
			(*blue_iter)->runAction(MoveBy::create(6 / (*blue_iter)->soldier.move_speed, Vec2(1.5, 0)));
		}
	}

	//对于红方每个小兵
	for (auto red_iter = red_soldier_vec.begin(); red_iter != red_soldier_vec.end(); ++red_iter)
	{
		bool red_flag = true;
		for (auto blue_iter = blue_soldier_vec.begin(); blue_iter != blue_soldier_vec.end(); ++blue_iter)
		{
			if ((*red_iter)->getPosition().distance((*blue_iter)->getPosition()) < (*red_iter)->soldier.attack_distance && (*blue_iter)->soldier.survival == true && (*red_iter)->soldier.survival == true)
			{
				int soldier_id = 2;//默认值为2，即炮车                            
				if ((blue_iter - blue_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((blue_iter - blue_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				red_flag = false;
				if ((*red_iter)->soldier.attack_speed == 100)//因为为每秒刷新，所以就b_num就高一点
				{
					auto tower_attack_effect = AttackEffect::create(3);//小兵攻击特效
					map_layer->addChild(tower_attack_effect);
					tower_attack_effect->setPosition((*red_iter)->getPosition());
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(*blue_iter, (*red_iter)->soldier.attack, soldier_id);
						});
					tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, (*blue_iter)->getPosition()), call, FadeOut::create(0.1), 0));
					(*red_iter)->soldier.attack_speed = 0;
				}
				else
				{
					++(*red_iter)->soldier.attack_speed;
				}
			}
		}
		if ((*red_iter)->getPosition().distance(towers[1]->getPosition()) <= (*red_iter)->soldier.attack_distance && towers[1]->tower.survival == true && (*red_iter)->soldier.survival == true)
		{
			red_flag = false;
			if ((*red_iter)->soldier.attack_speed == 100)//因为为每秒刷新，所以就b_num就高一点
			{
				auto tower_attack_effect = AttackEffect::create(3);//小兵攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*red_iter)->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(towers[1], (*red_iter)->soldier.attack, 1);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, towers[1]->getPosition()), call, FadeOut::create(0.1), 0));
				(*red_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*red_iter)->soldier.attack_speed;
			}
		}
		else if ((*red_iter)->getPosition().distance(towers[0]->getPosition()) <= (*red_iter)->soldier.attack_distance && towers[0]->tower.survival == true && (*red_iter)->soldier.survival == true)
		{
			red_flag = false;
			if ((*red_iter)->soldier.attack_speed == 100)//因为为每秒刷新，所以就b_num就高一点
			{
				auto tower_attack_effect = AttackEffect::create(3);//小兵攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*red_iter)->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(towers[0], (*red_iter)->soldier.attack, 0);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, towers[0]->getPosition()), call, FadeOut::create(0.1), 0));
				(*red_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*red_iter)->soldier.attack_speed;
			}
		}
		else if ((*red_iter)->getPosition().distance(my_hero->getPosition() - map_layer->getPosition()) <= (*red_iter)->soldier.attack_distance && my_hero->hero.survival == true && (*red_iter)->soldier.survival == true)
		{
			red_flag = false;
			if ((*red_iter)->soldier.attack_speed == 100)//因为为每秒刷新，所以就b_num就高一点
			{
				auto tower_attack_effect = AttackEffect::create(3);//小兵攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setPosition((*red_iter)->getPosition());
				if (my_hero->hero.armour_effect == true)                       //防御甲减速攻击方效果
				{
					(*red_iter)->soldier.move_speed = (*red_iter)->soldier.move_speed * 0.85;
					(*red_iter)->soldier.time = time_second;
				}
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(my_hero, (*red_iter)->soldier.attack);
					});
				tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, my_hero->getPosition() - map_layer->getPosition()), call, FadeOut::create(0.1), 0));
				(*red_iter)->soldier.attack_speed = 0;
			}
			else
			{
				++(*red_iter)->soldier.attack_speed;
			}
		}
		if (red_flag == true)
		{
			(*red_iter)->runAction(MoveBy::create(6 / (*red_iter)->soldier.move_speed, Vec2(-1.5, 0)));
		}
	}

}

//为的是即刻显示复活标签，不用等那一秒
void MainScene::display_resurrection(float)
{
	if (my_hero->hero.survival == false)
	{
		time_resurrection_label->setVisible(true);
		map->setColor(Color3B(128, 128, 128));
	}
	else
	{
		time_resurrection_label->setVisible(false);
		map->setColor(Color3B(255, 255, 255));
	}
}

//复活相关
void MainScene::resurrection(float)
{
	if (my_hero->hero.survival == false && my_hero->hero.resurrection_time == 0)//如果我方英雄阵亡，且复活时间为0
	{
		map_layer->setPosition(Vec2(0, 0));
		my_hero->setVisible(true);
		my_hero->hero.HP = my_hero->hero.FullHP;
		my_hero->blood->setScaleX(0.5);
		my_hero->hero.survival = true;
		my_hero->setPosition(Vec2(960, 540));
		my_hero->hero.resurrection_time = 10;
	}

	std::string resurrection_time("Resurrection time: ");
	resurrection_time += std::to_string(my_hero->hero.resurrection_time);
	time_resurrection_label->setString(resurrection_time);
	if (my_hero->hero.survival == false)
	{
		--my_hero->hero.resurrection_time;
	}
	if (ai_hero->hero.survival == false && ai_hero->hero.resurrection_time == 0)
	{
		ai_hero->setVisible(true);
		ai_hero->hero.survival = true;
		ai_hero->hero.HP = ai_hero->hero.FullHP;
		ai_hero->blood->setScaleX(0.5);
		ai_hero->setPosition(Vec2(6000, 700));
		ai_hero->hero.resurrection_time = 10;
	}
	if (ai_hero->hero.survival == false)
	{
		--ai_hero->hero.resurrection_time;
	}
}

//防御塔攻击
void MainScene::tower_attack(float)
{
	if (my_hero->hero.survival == true)
	{
		reBlood(my_hero);
	}
	if (ai_hero->hero.survival == true)
	{
		reBlood(ai_hero);
	}
	bool flag0 = true;
	if (towers[0]->tower.survival == true)
	{
		for (auto iter = red_soldier_vec.begin(); iter != red_soldier_vec.end() && flag0 == true; ++iter)
		{
			if ((*iter)->getPosition().distance(towers[0]->getPosition()) < towers[0]->tower.attack_distance && (*iter)->soldier.survival == true)
			{
				int soldier_id = 2;//默认值为2，即炮车                         
				if ((iter - red_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - red_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				flag0 = false;
				auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[0]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[0]->tower.attack, soldier_id);
					});
				//飞行效果
				tower_attack_effect->runAction(
					Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
						call, FadeOut::create(0.1), 0));
				break;//只攻击一次
			}
		}

		if (ai_hero->getPosition().distance(towers[0]->getPosition()) < towers[0]->tower.attack_distance && ai_hero->hero.survival == true && flag0 == true)
		{
			flag0 = false;
			auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
			map_layer->addChild(tower_attack_effect);
			tower_attack_effect->setScale(0.5);
			tower_attack_effect->setPosition(towers[0]->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(ai_hero, towers[0]->tower.attack);
				});
			tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, ai_hero->getPosition()), call, FadeOut::create(0.1), 0));
		}
	}

	bool flag1 = true;
	if (towers[1]->tower.survival == true)
	{
		for (auto iter = red_soldier_vec.begin(); iter != red_soldier_vec.end() && flag1 == true; ++iter)
		{
			if ((*iter)->getPosition().distance(towers[1]->getPosition()) < towers[1]->tower.attack_distance && (*iter)->soldier.survival == true)
			{
				int soldier_id = 2;//默认值为2，即炮车                             
				if ((iter - red_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - red_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				flag1 = false;
				auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[1]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[1]->tower.attack, soldier_id);
					});
				//飞行效果
				tower_attack_effect->runAction(
					Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
						call, FadeOut::create(0.1), 0));
				break;//只攻击一次
			}
		}

		if (ai_hero->getPosition().distance(towers[1]->getPosition()) < towers[1]->tower.attack_distance && ai_hero->hero.survival == true && flag1 == true)
		{
			flag1 = false;
			auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
			map_layer->addChild(tower_attack_effect);
			tower_attack_effect->setScale(0.5);
			tower_attack_effect->setPosition(towers[1]->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(ai_hero, towers[1]->tower.attack);
				});
			tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, ai_hero->getPosition()), call, FadeOut::create(0.1), 0));
		}
	}

	bool flag2 = true;
	if (towers[2]->tower.survival == true)
	{
		for (auto iter = blue_soldier_vec.begin(); iter != blue_soldier_vec.end() && flag2 == true; ++iter)
		{
			if ((*iter)->getPosition().distance(towers[2]->getPosition()) < towers[2]->tower.attack_distance && (*iter)->soldier.survival == true)
			{
				int soldier_id = 2;//默认值为2，即炮车                  
				if ((iter - blue_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - blue_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				flag2 = false;
				auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[2]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[2]->tower.attack, soldier_id);
					});
				//飞行效果
				tower_attack_effect->runAction(
					Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
						call, FadeOut::create(0.1), 0));
				break;//只攻击一次
			}
		}

		if ((my_hero->getPosition() - map_layer->getPosition()).distance(towers[2]->getPosition()) < towers[2]->tower.attack_distance && my_hero->hero.survival == true && flag2 == true)
		{
			flag2 = false;
			auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
			map_layer->addChild(tower_attack_effect);
			tower_attack_effect->setScale(0.5);
			tower_attack_effect->setPosition(towers[2]->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(my_hero, towers[2]->tower.attack);
				});
			tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, my_hero->getPosition() - map_layer->getPosition()), call, FadeOut::create(0.1), 0));
		}
	}

	bool flag3 = true;
	if (towers[3]->tower.survival == true)
	{
		for (auto iter = blue_soldier_vec.begin(); iter != blue_soldier_vec.end() && flag3 == true; ++iter)
		{
			if ((*iter)->getPosition().distance(towers[3]->getPosition()) < towers[3]->tower.attack_distance && (*iter)->soldier.survival == true)
			{
				int soldier_id = 2;//默认值为2，即炮车                          
				if ((iter - blue_soldier_vec.begin()) % 3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - blue_soldier_vec.begin()) % 3 == 1)
				{
					soldier_id = 1;
				}
				flag3 = false;
				auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[3]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[3]->tower.attack, soldier_id);
					});
				//飞行效果
				tower_attack_effect->runAction(
					Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
						call, FadeOut::create(0.1), 0));
				break;//只攻击一次
			}
		}

		if ((my_hero->getPosition() - map_layer->getPosition()).distance(towers[3]->getPosition()) < towers[3]->tower.attack_distance && my_hero->hero.survival == true && flag3 == true)
		{
			flag3 = false;
			auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
			map_layer->addChild(tower_attack_effect);
			tower_attack_effect->setScale(0.5);
			tower_attack_effect->setPosition(towers[3]->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(my_hero, towers[3]->tower.attack);
				});
			tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, my_hero->getPosition() - map_layer->getPosition()), call, FadeOut::create(0.1), 0));
		}
	}
}

//判断游戏结束条件
void MainScene::gameOver(float)
{
	if (towers[0]->tower.survival == false)
	{
		fail->setVisible(true);
		Director::getInstance()->stopAnimation();
	}
	if (towers[3]->tower.survival == false)
	{
		win->setVisible(true);
		Director::getInstance()->stopAnimation();
	}
}

//敌方英雄攻击
void MainScene::ai_hero_attack(float)
{
	//控制一次只攻击一个
	bool flag = true;
	//对于所有蓝色兵 且ai需要是存活的
	for (auto iter = blue_soldier_vec.begin(); iter != blue_soldier_vec.end() && ai_hero->hero.survival == true; ++iter)
	{
		if (ai_hero->getPosition().distance((*iter)->getPosition()) < ai_hero->hero.attack_distance && (*iter)->soldier.survival == true)
		{
			int soldier_id = 2;//默认值为2，即炮车                           
			if ((iter - blue_soldier_vec.begin()) % 3 == 0)
			{
				soldier_id = 0;
			}
			if ((iter - blue_soldier_vec.begin()) % 3 == 1)
			{
				soldier_id = 1;
			}
			//攻击效果已绑定英雄
			flag = false;
			auto hero_attack_effect = AttackEffect::create(MainScene::ai_hero_id - 3);
			map_layer->addChild(hero_attack_effect);
			hero_attack_effect->setScale(0.5);
			hero_attack_effect->setPosition(ai_hero->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood((*iter), ai_hero->hero.attack, soldier_id);
				});
			//飞行效果
			hero_attack_effect->runAction(
				Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
					call, FadeOut::create(0.1), 0));
			break;
		}
	}

	//攻击英雄
	if (ai_hero->getPosition().distance(my_hero->getPosition() - map_layer->getPosition()) < ai_hero->hero.attack_distance
		&& ai_hero->hero.survival == true && my_hero->hero.survival == true && flag == true)
	{
		//攻击效果已绑定英雄
		flag = false;
		auto hero_attack_effect = AttackEffect::create(MainScene::ai_hero_id - 3);
		map_layer->addChild(hero_attack_effect);
		hero_attack_effect->setScale(0.5);
		hero_attack_effect->setPosition(ai_hero->getPosition());
		if (my_hero->hero.armour_effect == true)                       //防御甲减速攻击方效果
		{
			ai_hero->hero.move_speed = ai_hero->hero.move_speed * 0.85;
			ai_hero->hero.time = time_second;
		}
		auto call = CallFuncN::create([=](Node* t)
			{
				dropBlood(my_hero, ai_hero->hero.attack);
			});
		//飞行效果
		hero_attack_effect->runAction(
			Sequence::create(MoveTo::create(1.2f, my_hero->getPosition() - map_layer->getPosition()),
				call, FadeOut::create(0.1), 0));
	}

	//攻击塔
	for (int i = 0; i <= 1 && ai_hero->hero.survival == true; i++)
	{
		if (ai_hero->getPosition().distance(towers[i]->getPosition()) < ai_hero->hero.attack_distance && towers[i]->tower.survival == true && flag == true)
		{
			flag = false;
			//攻击效果已绑定英雄
			auto hero_attack_effect = AttackEffect::create(MainScene::ai_hero_id - 3);
			map_layer->addChild(hero_attack_effect);
			hero_attack_effect->setScale(0.5);
			hero_attack_effect->setPosition(ai_hero->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(towers[i], ai_hero->hero.attack, i);
				});
			//飞行效果
			hero_attack_effect->runAction(
				Sequence::create(MoveTo::create(1.2f, towers[i]->getPosition()),
					call, FadeOut::create(0.1), 0));
			break;
		}
	}
}

//添加防御塔
void MainScene::addTower()
{
	for (int i = 0; i < 4; i++)
	{
		towers.push_back(TowerSprite::create(i));
		map_layer->addChild(towers[i]);
		towers[i]->setZOrder(100);
	}
	towers[0]->setPosition(Vec2(1550, 550));
	towers[1]->setPosition(Vec2(2550, 550));
	towers[2]->setPosition(Vec2(3750, 550));
	towers[3]->setPosition(Vec2(4950, 550));

	//防御塔范围的一个圈
	auto blue_tower_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(blue_tower_bg);
	blue_tower_bg->setPosition(Vec2(2550, 550));
	blue_tower_bg->setColor(Color3B(100, 100, 100));

	auto blue_basement_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(blue_basement_bg);
	blue_basement_bg->setPosition(Vec2(1550, 550));
	blue_basement_bg->setColor(Color3B(100, 100, 100));

	auto red_tower_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(red_tower_bg);
	red_tower_bg->setPosition(Vec2(3750, 550));
	red_tower_bg->setColor(Color3B(100, 100, 100));

	auto red_basement_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(red_basement_bg);
	red_basement_bg->setPosition(Vec2(4950, 550));
	red_basement_bg->setColor(Color3B(100, 100, 100));

}

//更新显示的时间，并且出兵线
void MainScene::time(float)
{
	//每秒加1金币
	++my_hero->hero.money;

	std::string time;
	if (time_second == 60)
	{
		++time_minute;
		time_second = 0;
	}
	if (time_minute < 10)
	{
		time += "0";
	}
	time += std::to_string(time_minute);
	if (time_second < 10)
	{
		time += ":0";
	}
	else
	{
		time += ":";
	}
	time += std::to_string(time_second++);
	if (my_hero->hero.armour_effect == true)
	{
		for (int i = 0; i < red_soldier_vec.size(); ++i)
		{
			if (red_soldier_vec[i]->soldier.time != 61)
			{
				red_soldier_vec[i]->soldier.move_speed = red_soldier_vec[i]->soldier.move_speed / 0.85;
				red_soldier_vec[i]->soldier.time = 61;
			}
		}
		if (ai_hero->hero.time != 61)
		{
			ai_hero->hero.move_speed = ai_hero->hero.move_speed / 0.85;
		}
	}
	if (ai_hero->hero.armour_effect == true)
	{
		for (int i = 0; i < blue_soldier_vec.size(); ++i)
		{
			if (blue_soldier_vec[i]->soldier.time != 61)
			{
				blue_soldier_vec[i]->soldier.move_speed = blue_soldier_vec[i]->soldier.move_speed / 0.85;
				blue_soldier_vec[i]->soldier.time = 61;
			}
		}
		if (my_hero->hero.time != 61)
		{
			my_hero->hero.move_speed = my_hero->hero.move_speed / 0.85;
		}
	}
	time_label->setString(time);//显示时间

	//每一分钟出一波兵
	if (time_second == 1)//近战                           
	{
		auto blue_soldier = SoldierSprite::create(0, time_minute);
		map_layer->addChild(blue_soldier);
		blue_soldier_vec.push_back(blue_soldier);
		blue_soldier->setPosition(Vec2(1600, 650));
		//blue_soldier->setPosition(Vec2(2600, 650));

		auto red_soldier = SoldierSprite::create(3, time_minute);
		map_layer->addChild(red_soldier);
		red_soldier_vec.push_back(red_soldier);
		red_soldier->setPosition(Vec2(4900, 650));
	}
	if (time_second == 3)//远程                                
	{
		auto blue_soldier = SoldierSprite::create(1, time_minute);
		map_layer->addChild(blue_soldier);
		blue_soldier_vec.push_back(blue_soldier);
		blue_soldier->setPosition(Vec2(1600, 650));

		auto red_soldier = SoldierSprite::create(4, time_minute);
		map_layer->addChild(red_soldier);
		red_soldier_vec.push_back(red_soldier);
		red_soldier->setPosition(Vec2(4900, 650));
	}
	if (time_second == 5)//炮车                     
	{
		auto blue_soldier = SoldierSprite::create(2, time_minute);
		map_layer->addChild(blue_soldier);
		blue_soldier_vec.push_back(blue_soldier);
		blue_soldier->setPosition(Vec2(1600, 650));

		auto red_soldier = SoldierSprite::create(5, time_minute);
		map_layer->addChild(red_soldier);
		red_soldier_vec.push_back(red_soldier);
		red_soldier->setPosition(Vec2(4900, 650));
	}

}

//英雄掉血
void MainScene::dropBlood(HeroSprite* hero, int attack)
{
	hero->hero.HP -= (attack * 602 / (602 + hero->hero.defect));

	//还活着，让血条相应变化
	if (hero->hero.HP > 0)
	{
		hero->blood->setScaleX((hero->hero.HP / hero->hero.FullHP) * 0.5);
	}
	//死了，血条消失，图片消失，逻辑为死亡
	else
	{
		hero->blood->setScaleX(0);
		hero->setVisible(false);
		hero->hero.survival = false;
	}
}

//英雄自身回血                                              
void MainScene::reBlood(HeroSprite* hero)
{
	if (hero->hero.HP < hero->hero.FullHP)
	{
		hero->hero.HP += hero->hero.rehp;
		if (hero->hero.HP > hero->hero.FullHP)
		{
			hero->hero.HP = hero->hero.FullHP;
		}
		hero->blood->setScaleX((hero->hero.HP / hero->hero.FullHP) * 0.5);
	}

}

//塔掉血
void MainScene::dropBlood(TowerSprite* tower, int attack, int id)
{
	tower->tower.HP -= (attack * 602 / (602 + tower->tower.defect));

	//还活着，让血条相应变化
	if (tower->tower.HP > 0)
	{
		tower->blood->setScaleX((tower->tower.HP / TowerSprite::get_tower_hp(id)) * 0.5);
	}
	//死了，血条消失，图片消失，逻辑为死亡
	else
	{
		tower->blood->setScaleX(0);
		tower->setVisible(false);
		tower->tower.survival = false;
	}
}

//小兵掉血
void MainScene::dropBlood(SoldierSprite* soldier, int attack, int id)
{
	soldier->soldier.HP -= (attack * 602 / (602 + soldier->soldier.defect));

	//还活着，让血条相应变化
	if (soldier->soldier.HP > 0)
	{
		soldier->blood->setScaleX((soldier->soldier.HP / SoldierSprite::get_soldier_hp(id, time_minute)) * 0.5);
	}
	//死了，血条消失，图片消失，逻辑为死亡
	else
	{
		if (id >= 3 && attack==my_hero->hero.attack)//敌方小兵死了 并且是我打死的
		{
			//加钱加金币
			my_hero->hero.money += 25;
			my_hero->hero.hero_exp += 25;

			++killed_soldier_result;
		}

		soldier->blood->setScaleX(0);
		soldier->setVisible(false);
		soldier->soldier.survival = false;
	}
}

//是否能够运动
void MainScene::sport(float)
{
	//我方英雄阵亡
	if (my_hero->hero.survival == false)
	{
		my_hero->stopAllActions();
	}
	ai_hero->hero.can_move = true;
	for (auto iter = blue_soldier_vec.begin(); iter != blue_soldier_vec.end(); ++iter)
	{
		//如果攻击范围内有蓝色存活兵，则不可移动
		if (ai_hero->getPosition().distance((*iter)->getPosition()) < ai_hero->hero.attack_distance && (*iter)->soldier.survival == true)
		{
			ai_hero->hero.can_move = false;
		}
	}
	//靠近蓝色存活的1塔，或者靠近水晶时
	if (towers[1]->getPosition().distance(ai_hero->getPosition()) < ai_hero->hero.attack_distance && towers[1]->tower.survival == true
		|| towers[0]->getPosition().distance(ai_hero->getPosition()) < ai_hero->hero.attack_distance)
	{
		ai_hero->hero.can_move = false;
	}
	//靠近英雄时
	if (ai_hero->getPosition().distance(my_hero->getPosition() - map_layer->getPosition()) <= ai_hero->hero.attack_distance && my_hero->hero.survival == true)
	{
		ai_hero->hero.can_move = false;
	}
	//如果可以移动，那么就移动
	if (ai_hero->hero.can_move == true && ai_hero->hero.survival == true)
	{
		ai_hero->runAction(MoveBy::create(8 / (ai_hero)->hero.move_speed, Vec2(-3, 0)));
	}
}

//创建商店按钮                    
bool  MainScene::create_shop()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//添加进入商店游戏按钮  
	auto item1 = MenuItemImage::create("shop/shop.png", "shop/shopselected.png", CC_CALLBACK_1(MainScene::OpenShop, this));
	shop_open_menu = Menu::create(item1, NULL);
	shop_open_menu->alignItemsVerticallyWithPadding(50);
	//按钮放置在屏幕左中间
	item1->setPosition(Vec2(origin.x - 850, origin.y));
	this->addChild(shop_open_menu, 1);

	return true;
}

//打开商店回调函数
void MainScene::OpenShop(Ref* pSender)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (money_label_shop != NULL)//商店里面的金币显示和外界的一样
	{
		money_label_shop->setString(std::to_string(my_hero->hero.money));
	}

	if (shop_layer == NULL)
	{
		//初次载入商店
		shop_layer = Layer::create();
		this->addChild(shop_layer, 2);
		shop = Sprite::create("shop/shopopened_first.png");
		shop_layer->addChild(shop);
		shop_layer->setAnchorPoint(Vec2(0, 0));
		shop->setAnchorPoint(Vec2(0, 0));
		shop_open_menu->removeFromParent();//打开商店时先删除打开商店按钮，防止误点导致程序关闭商店时崩溃

		if (money_label_shop == NULL)
		{
			//载入金币                                                                               
			money_label_shop = Label::create(std::to_string(my_hero->hero.money), "Arial", 50);
			money_label_shop->setColor(Color3B(255, 215, 0));
			this->addChild(money_label_shop, 4);
			money_label_shop->setPosition(Vec2(140, 920));

		}

		//添加关闭商店游戏按钮  
		auto item1 = MenuItemImage::create("shop/close.png", "shop/closeselected.png", CC_CALLBACK_1(MainScene::CloseShop, this));
		shop_close_menu = Menu::create(item1, NULL);
		shop_close_menu->alignItemsVerticallyWithPadding(50);
		//放置关闭商店按钮
		item1->setPosition(Vec2(origin.x + 850, origin.y + 425));
		this->addChild(shop_close_menu, 2);

		//添加切换商店页数按钮
		auto item_2 = MenuItemImage::create("shop/second_page.png", "shop/second_pageselected.png", CC_CALLBACK_1(MainScene::second_page, this));
		second_page_menu = Menu::create(item_2, NULL);
		second_page_menu->alignItemsVerticallyWithPadding(50);
		//放置第二页商店按钮
		item_2->setPosition(Vec2(origin.x - 850, origin.y + 130));
		this->addChild(second_page_menu, 2);

		//添加装备红宝石
		auto item2 = MenuItemImage::create("shop/red_diamond.png", "shop/red_diamond.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 300 && my_hero->hero.equipments.size() < 6)
						{
							++red_diamond_num;
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.money -= 300;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.FullHP += 300;
							my_hero->hero.HP += 300;
							my_hero->hero.equipments.push_back(red_diamond_menu);
							int i = my_hero->hero.equipments.size();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/red_diamond1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/red_diamond2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 423, origin.y + 790));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 3);

							if (belt_strength_after_red_menu != NULL)   //由于买了个红宝石就创建一个买了红宝石后的力量菜单，所以需每次把前一个给设为不可见
							{
								belt_strength_after_red_menu->setVisible(false);
							}
							//添加装备力量腰带   
							auto item10 = MenuItemImage::create("shop/belt_strength_after_red.png", "shop/belt_strength_after_red.png", [&](Ref* sender)
								{
									auto visibleSize = Director::getInstance()->getVisibleSize();
									Vec2 origin = Director::getInstance()->getVisibleOrigin();
									auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
										{
											if (my_hero->hero.money > 600)
											{
												my_hero->hero.money -= 600;
												money_label->setString(std::to_string(my_hero->hero.money));
												money_label_shop->setString(std::to_string(my_hero->hero.money));
												my_hero->hero.FullHP += 700;
												my_hero->hero.HP += 700;
												auto visibleSize = Director::getInstance()->getVisibleSize();
												Vec2 origin = Director::getInstance()->getVisibleOrigin();
												int size = my_hero->hero.equipments.size();
												int j = 0;
												for (j = 0; j < size; j++)
												{
													if (my_hero->hero.equipments[j] == red_diamond_menu)
													{
														my_hero->hero.equipments[j] = belt_strength_after_red_menu;
														red_diamond_num--;
														break;
													}
												}
												my_hero->hero.equipmentsImages[j * 2]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2] = (Sprite::create("shop/belt_strength1.png"));
												my_hero->hero.equipmentsImages[j * 2]->setPosition(Vec2(origin.x + vecx[j], origin.y + vecy[0]));
												this->addChild(my_hero->hero.equipmentsImages[j * 2], 2);
												my_hero->hero.equipmentsImages[j * 2 + 1]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2 + 1] = (Sprite::create("shop/belt_strength_after_red.png"));
												my_hero->hero.equipmentsImages[j * 2 + 1]->setPosition(Vec2(origin.x + 1358, origin.y + 430));
												this->addChild(my_hero->hero.equipmentsImages[j * 2 + 1], 2);
												if (red_diamond_num == 0)
												{
													if (equipments_introduction != NULL)
													{
														equipments_introduction->removeFromParent();
														equipments_introduction = NULL;
														buy_it_menu->removeFromParent();
														buy_it_menu = NULL;
													}
													if (belt_strength_menu != NULL)            //此处用来把打开商店时创建的力量腰带菜单设为不可见，不然指针指向这之后又创建的新力量腰带菜单后原菜单就清除不掉了
													{
														belt_strength_menu->setVisible(false);
													}
													//添加装备力量腰带   
													auto item10 = MenuItemImage::create("shop/belt_strength2.png", "shop/belt_strength2.png", [&](Ref* sender)
														{
															auto visibleSize = Director::getInstance()->getVisibleSize();
															Vec2 origin = Director::getInstance()->getVisibleOrigin();
															auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
																{
																	if (my_hero->hero.money > 900 && my_hero->hero.equipments.size() < 6)
																	{
																		my_hero->hero.money -= 900;
																		money_label->setString(std::to_string(my_hero->hero.money));
																		money_label_shop->setString(std::to_string(my_hero->hero.money));
																		my_hero->hero.FullHP += 1000;
																		my_hero->hero.HP += 1000;
																		my_hero->hero.equipments.push_back(belt_strength_menu);
																		int i = my_hero->hero.equipments.size();
																		auto visibleSize = Director::getInstance()->getVisibleSize();
																		Vec2 origin = Director::getInstance()->getVisibleOrigin();
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/belt_strength1.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/belt_strength2.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1358, origin.y + 430));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

																	}
																});
															if (equipments_introduction != NULL)
															{
																equipments_introduction->removeFromParent();
															}
															equipments_introduction = Sprite::create("shop/belt_strength3.png");
															equipments_introduction->setPosition(Vec2(1730, 630));
															this->addChild(equipments_introduction, 2);
															if (buy_it_menu != NULL)
															{
																buy_it_menu->removeFromParent();
															}
															buy_it_menu = Menu::create(item, NULL);
															buy_it_menu->alignItemsVerticallyWithPadding(50);
															item->setPosition(Vec2(origin.x + 750, origin.y - 250));
															this->addChild(buy_it_menu, 2);

														});
													belt_strength_menu = Menu::create(item10, NULL);
													belt_strength_menu->alignItemsVerticallyWithPadding(50);
													//放置力量腰带装备按钮
													item10->setPosition(Vec2(origin.x + 400, origin.y - 110));
													this->addChild(belt_strength_menu, 2);
												}

											}
										});
									if (equipments_introduction != NULL)
									{
										equipments_introduction->removeFromParent();
									}
									equipments_introduction = Sprite::create("shop/belt_strength3.png");
									equipments_introduction->setPosition(Vec2(1730, 630));
									this->addChild(equipments_introduction, 2);
									if (buy_it_menu != NULL)
									{
										buy_it_menu->removeFromParent();
									}
									buy_it_menu = Menu::create(item, NULL);
									buy_it_menu->alignItemsVerticallyWithPadding(50);
									item->setPosition(Vec2(origin.x + 750, origin.y - 250));
									this->addChild(buy_it_menu, 2);

								});
							belt_strength_after_red_menu = Menu::create(item10, NULL);
							belt_strength_after_red_menu->alignItemsVerticallyWithPadding(50);
							//放置力量腰带装备按钮
							item10->setPosition(Vec2(origin.x + 400, origin.y - 110));
							this->addChild(belt_strength_after_red_menu, 2);
						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/red_diamond3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		red_diamond_menu = Menu::create(item2, NULL);
		red_diamond_menu->alignItemsVerticallyWithPadding(50);
		//放置红宝石装备按钮
		item2->setPosition(Vec2(origin.x - 500, origin.y + 250));
		this->addChild(red_diamond_menu, 3);//zorder设为3用来解决，可能会导致新问题//问题：购买最后一个合成的大剑后，关闭商店重新打开时红宝石处显示的菜单为为购买时的菜单

		//添加装备蓝宝石
		auto item3 = MenuItemImage::create("shop/blue_diamond.png", "shop/blue_diamond.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 220 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 220;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.FullMP += 300;
							my_hero->hero.MP += 300;
							my_hero->hero.equipments.push_back(blue_diamond_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/blue_diamond1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/blue_diamond2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 883, origin.y + 790));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/blue_diamond3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		blue_diamond_menu = Menu::create(item3, NULL);
		blue_diamond_menu->alignItemsVerticallyWithPadding(50);
		//放置蓝宝石装备按钮
		item3->setPosition(Vec2(origin.x - 100, origin.y + 250));
		this->addChild(blue_diamond_menu, 2);

		//添加装备法强书
		auto item4 = MenuItemImage::create("shop/mp_book.png", "shop/mp_book.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 300 && my_hero->hero.equipments.size() < 6)
						{
							++mp_book_num;
							my_hero->hero.money -= 300;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.ap += 40;
							my_hero->hero.equipments.push_back(mp_book_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/mp_book1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/mp_book2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1333, origin.y + 790));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);
							if (big_stick_after_book_menu != NULL)   //由于买了个法强书就创建一个买了法强书后的力量菜单，所以需每次把前一个给设为不可见
							{
								big_stick_after_book_menu->setVisible(false);
							}
							//添加装备大棒   
							auto item11 = MenuItemImage::create("shop/big_stick_after_book.png", "shop/big_stick_after_book.png", [&](Ref* sender)
								{
									auto visibleSize = Director::getInstance()->getVisibleSize();
									Vec2 origin = Director::getInstance()->getVisibleOrigin();
									auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
										{
											if (my_hero->hero.money > 520)
											{
												my_hero->hero.money -= 520;
												money_label->setString(std::to_string(my_hero->hero.money));
												money_label_shop->setString(std::to_string(my_hero->hero.money));
												my_hero->hero.ap += 80;
												auto visibleSize = Director::getInstance()->getVisibleSize();
												Vec2 origin = Director::getInstance()->getVisibleOrigin();
												int size = my_hero->hero.equipments.size();
												int j = 0;
												for (j = 0; j < size; j++)
												{
													if (my_hero->hero.equipments[j] == mp_book_menu)
													{
														my_hero->hero.equipments[j] = big_stick_after_book_menu;
														mp_book_num--;
														break;
													}
												}
												my_hero->hero.equipmentsImages[j * 2]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2] = (Sprite::create("shop/big_stick1.png"));
												my_hero->hero.equipmentsImages[j * 2]->setPosition(Vec2(origin.x + vecx[j], origin.y + vecy[0]));
												this->addChild(my_hero->hero.equipmentsImages[j * 2], 2);
												my_hero->hero.equipmentsImages[j * 2 + 1]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2 + 1] = (Sprite::create("shop/big_stick_after_book.png"));
												my_hero->hero.equipmentsImages[j * 2 + 1]->setPosition(Vec2(origin.x + 438, origin.y + 250));
												this->addChild(my_hero->hero.equipmentsImages[j * 2 + 1], 2);
												if (mp_book_num == 0)
												{
													if (equipments_introduction != NULL)
													{
														equipments_introduction->removeFromParent();
														equipments_introduction = NULL;
														buy_it_menu->removeFromParent();
														buy_it_menu = NULL;
													}
													if (big_stick_menu != NULL)            //此处用来把打开商店时创建的大棒菜单设为不可见，不然指针指向这之后又创建的新大棒菜单后原菜单就清除不掉了
													{
														big_stick_menu->setVisible(false);
													}
													//添加装备大棒 
													auto item11 = MenuItemImage::create("shop/big_stick2.png", "shop/big_stick2.png", [&](Ref* sender)
														{
															auto visibleSize = Director::getInstance()->getVisibleSize();
															Vec2 origin = Director::getInstance()->getVisibleOrigin();
															auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
																{
																	if (my_hero->hero.money > 820 && my_hero->hero.equipments.size() < 6)
																	{
																		my_hero->hero.money -= 820;
																		money_label->setString(std::to_string(my_hero->hero.money));
																		money_label_shop->setString(std::to_string(my_hero->hero.money));
																		my_hero->hero.ap += 120;
																		my_hero->hero.equipments.push_back(big_stick_menu);
																		int i = my_hero->hero.equipments.size();
																		auto visibleSize = Director::getInstance()->getVisibleSize();
																		Vec2 origin = Director::getInstance()->getVisibleOrigin();
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_stick1.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_stick2.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 438, origin.y + 250));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

																	}
																});
															if (equipments_introduction != NULL)
															{
																equipments_introduction->removeFromParent();
															}
															equipments_introduction = Sprite::create("shop/big_stick3.png");
															equipments_introduction->setPosition(Vec2(1730, 630));
															this->addChild(equipments_introduction, 2);
															if (buy_it_menu != NULL)
															{
																buy_it_menu->removeFromParent();
															}
															buy_it_menu = Menu::create(item, NULL);
															buy_it_menu->alignItemsVerticallyWithPadding(50);
															item->setPosition(Vec2(origin.x + 750, origin.y - 250));
															this->addChild(buy_it_menu, 2);

														});
													big_stick_menu = Menu::create(item11, NULL);
													big_stick_menu->alignItemsVerticallyWithPadding(50);
													//放置大棒装备按钮
													item11->setPosition(Vec2(origin.x - 520, origin.y - 290));
													this->addChild(big_stick_menu, 2);
												}

											}
										});
									if (equipments_introduction != NULL)
									{
										equipments_introduction->removeFromParent();
									}
									equipments_introduction = Sprite::create("shop/big_stick3.png");
									equipments_introduction->setPosition(Vec2(1730, 630));
									this->addChild(equipments_introduction, 2);
									if (buy_it_menu != NULL)
									{
										buy_it_menu->removeFromParent();
									}
									buy_it_menu = Menu::create(item, NULL);
									buy_it_menu->alignItemsVerticallyWithPadding(50);
									item->setPosition(Vec2(origin.x + 750, origin.y - 250));
									this->addChild(buy_it_menu, 2);

								});
							big_stick_after_book_menu = Menu::create(item11, NULL);
							big_stick_after_book_menu->alignItemsVerticallyWithPadding(50);
							//放置大棒装备按钮
							item11->setPosition(Vec2(origin.x - 520, origin.y - 290));
							this->addChild(big_stick_after_book_menu, 2);
						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/mp_book3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		mp_book_menu = Menu::create(item4, NULL);
		mp_book_menu->alignItemsVerticallyWithPadding(50);
		//放置法强书装备按钮
		item4->setPosition(Vec2(origin.x + 380, origin.y + 250));
		this->addChild(mp_book_menu, 2);

		//添加装备铁剑
		auto item5 = MenuItemImage::create("shop/iron_sword.png", "shop/iron_sword.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 250 && my_hero->hero.equipments.size() < 6)
						{
							iron_sword_num++;
							my_hero->hero.money -= 250;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.attack += 20;
							my_hero->hero.equipments.push_back(iron_sword_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/iron_sword1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/iron_sword2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 438, origin.y + 610));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/iron_sword3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		iron_sword_menu = Menu::create(item5, NULL);
		iron_sword_menu->alignItemsVerticallyWithPadding(50);
		//放置铁剑装备按钮
		item5->setPosition(Vec2(origin.x - 520, origin.y + 70));
		this->addChild(iron_sword_menu, 2);

		//添加装备布衣
		auto item6 = MenuItemImage::create("shop/def_clothes.png", "shop/def_clothes.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 220 && my_hero->hero.equipments.size() < 6)
						{
							++def_clothes_num;
							my_hero->hero.money -= 220;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.defect += 60;
							my_hero->hero.equipments.push_back(def_clothes_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_clothes1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_clothes2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 858, origin.y + 610));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);
							if (def_armour_after_clothes_menu != NULL)   //由于买了个布衣就创建一个买了布衣后的防御甲菜单，所以需每次把前一个给设为不可见
							{
								def_armour_after_clothes_menu->setVisible(false);
							}
							//添加装备防御甲
							auto item13 = MenuItemImage::create("shop/def_armour_after_clothes.png", "shop/def_armour_after_clothes.png", [&](Ref* sender)
								{
									auto visibleSize = Director::getInstance()->getVisibleSize();
									Vec2 origin = Director::getInstance()->getVisibleOrigin();
									auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
										{
											if (my_hero->hero.money > 510)
											{
												my_hero->hero.money -= 510;
												money_label->setString(std::to_string(my_hero->hero.money));
												money_label_shop->setString(std::to_string(my_hero->hero.money));
												my_hero->hero.defect += 120;
												my_hero->hero.armour_effect = true;
												auto visibleSize = Director::getInstance()->getVisibleSize();
												Vec2 origin = Director::getInstance()->getVisibleOrigin();
												int size = my_hero->hero.equipments.size();
												int j = 0;
												for (j = 0; j < size; j++)
												{
													if (my_hero->hero.equipments[j] == def_clothes_menu)
													{
														my_hero->hero.equipments[j] = def_armour_after_clothes_menu;
														def_clothes_num--;
														break;
													}
												}
												my_hero->hero.equipmentsImages[j * 2]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2] = (Sprite::create("shop/def_armour1.png"));
												my_hero->hero.equipmentsImages[j * 2]->setPosition(Vec2(origin.x + vecx[j], origin.y + vecy[0]));
												this->addChild(my_hero->hero.equipmentsImages[j * 2], 2);
												my_hero->hero.equipmentsImages[j * 2 + 1]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2 + 1] = (Sprite::create("shop/def_armour_after_clothes.png"));
												my_hero->hero.equipmentsImages[j * 2 + 1]->setPosition(Vec2(origin.x + 1358, origin.y + 250));
												this->addChild(my_hero->hero.equipmentsImages[j * 2 + 1], 2);
												if (def_clothes_num == 0)
												{
													if (equipments_introduction != NULL)
													{
														equipments_introduction->removeFromParent();
														equipments_introduction = NULL;
														buy_it_menu->removeFromParent();
														buy_it_menu = NULL;
													}
													if (def_armour_menu != NULL)            //此处用来把打开商店时创建的防御甲菜单设为不可见，不然指针指向这之后又创建的新防御甲菜单后原菜单就清除不掉了
													{
														def_armour_menu->setVisible(false);
													}
													//添加装备防御甲
													auto item13 = MenuItemImage::create("shop/def_armour2.png", "shop/def_armour2.png", [&](Ref* sender)
														{
															auto visibleSize = Director::getInstance()->getVisibleSize();
															Vec2 origin = Director::getInstance()->getVisibleOrigin();
															auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
																{
																	if (my_hero->hero.money > 730 && my_hero->hero.equipments.size() < 6)
																	{
																		my_hero->hero.money -= 730;
																		money_label->setString(std::to_string(my_hero->hero.money));
																		money_label_shop->setString(std::to_string(my_hero->hero.money));
																		my_hero->hero.defect += 210;
																		my_hero->hero.armour_effect = true;
																		my_hero->hero.equipments.push_back(def_armour_menu);
																		int i = my_hero->hero.equipments.size();
																		auto visibleSize = Director::getInstance()->getVisibleSize();
																		Vec2 origin = Director::getInstance()->getVisibleOrigin();
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_armour1.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_armour2.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1358, origin.y + 250));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

																	}
																});
															if (equipments_introduction != NULL)
															{
																equipments_introduction->removeFromParent();
															}
															equipments_introduction = Sprite::create("shop/def_armour3.png");
															equipments_introduction->setPosition(Vec2(1730, 630));
															this->addChild(equipments_introduction, 2);
															if (buy_it_menu != NULL)
															{
																buy_it_menu->removeFromParent();
															}
															buy_it_menu = Menu::create(item, NULL);
															buy_it_menu->alignItemsVerticallyWithPadding(50);
															item->setPosition(Vec2(origin.x + 750, origin.y - 250));
															this->addChild(buy_it_menu, 2);

														});
													def_armour_menu = Menu::create(item13, NULL);
													def_armour_menu->alignItemsVerticallyWithPadding(50);
													//放置防御甲装备按钮
													item13->setPosition(Vec2(origin.x + 400, origin.y - 290));
													this->addChild(def_armour_menu, 2);
												}

											}
										});
									if (equipments_introduction != NULL)
									{
										equipments_introduction->removeFromParent();
									}
									equipments_introduction = Sprite::create("shop/def_armour3.png");
									equipments_introduction->setPosition(Vec2(1730, 630));
									this->addChild(equipments_introduction, 2);
									if (buy_it_menu != NULL)
									{
										buy_it_menu->removeFromParent();
									}
									buy_it_menu = Menu::create(item, NULL);
									buy_it_menu->alignItemsVerticallyWithPadding(50);
									item->setPosition(Vec2(origin.x + 750, origin.y - 250));
									this->addChild(buy_it_menu, 2);

								});
							def_armour_after_clothes_menu = Menu::create(item13, NULL);
							def_armour_after_clothes_menu->alignItemsVerticallyWithPadding(50);
							//放置防御甲装备按钮
							item13->setPosition(Vec2(origin.x + 400, origin.y - 290));
							this->addChild(def_armour_after_clothes_menu, 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/def_clothes3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		def_clothes_menu = Menu::create(item6, NULL);
		def_clothes_menu->alignItemsVerticallyWithPadding(50);
		//放置布衣装备按钮
		item6->setPosition(Vec2(origin.x - 100, origin.y + 70));
		this->addChild(def_clothes_menu, 2);

		//添加装备抗魔披风
		auto item7 = MenuItemImage::create("shop/def_ap_cloak.png", "shop/def_ap_cloak.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 220 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 220;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.ap_def += 60;
							my_hero->hero.equipments.push_back(def_ap_cloak_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_ap_cloak1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_ap_cloak2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1338, origin.y + 610));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/def_ap_cloak3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		def_ap_cloak_menu = Menu::create(item7, NULL);
		def_ap_cloak_menu->alignItemsVerticallyWithPadding(50);
		//放置抗魔披风装备按钮
		item7->setPosition(Vec2(origin.x + 380, origin.y + 70));
		this->addChild(def_ap_cloak_menu, 2);

		//添加装备绿水晶
		auto item8 = MenuItemImage::create("shop/green_diamond.png", "shop/green_diamond.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 140 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 140;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.rehp += 6;
							my_hero->hero.equipments.push_back(green_diamond_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/green_diamond1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/green_diamond2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 438, origin.y + 430));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/green_diamond3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);

			});
		green_diamond_menu = Menu::create(item8, NULL);
		green_diamond_menu->alignItemsVerticallyWithPadding(50);
		//放置绿水晶装备按钮
		item8->setPosition(Vec2(origin.x - 520, origin.y - 110));
		this->addChild(green_diamond_menu, 2);

		//添加装备草鞋   
		auto item9 = MenuItemImage::create("shop/basic_shoes.png", "shop/basic_shoes.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 250 && my_hero->hero.equipments.size() < 6 && my_hero->hero.move_speed == 500)//此处限制只能买一双鞋,500为初始速度，记得以后更改
						{
							++basic_shoes_num;
							my_hero->hero.money -= 250;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.move_speed += 30;
							my_hero->hero.equipments.push_back(basic_shoes_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/basic_shoes1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/basic_shoes2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 858, origin.y + 430));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);
							if (def_shoes_after_basic_menu != NULL)   //由于买了个草鞋就创建一个买了草鞋后的防御鞋菜单，所以需每次把前一个给设为不可见
							{
								def_shoes_after_basic_menu->setVisible(false);
							}
							//添加装备大棒   
							auto item12 = MenuItemImage::create("shop/def_shoes_after_basic.png", "shop/def_shoes_after_basic.png", [&](Ref* sender)
								{
									auto visibleSize = Director::getInstance()->getVisibleSize();
									Vec2 origin = Director::getInstance()->getVisibleOrigin();
									auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
										{
											if (my_hero->hero.money > 460)
											{
												my_hero->hero.money -= 460;
												money_label->setString(std::to_string(my_hero->hero.money));
												money_label_shop->setString(std::to_string(my_hero->hero.money));
												my_hero->hero.move_speed += 30;
												my_hero->hero.defect = my_hero->hero.defect * 1.17 + 102;
												auto visibleSize = Director::getInstance()->getVisibleSize();
												Vec2 origin = Director::getInstance()->getVisibleOrigin();
												int size = my_hero->hero.equipments.size();
												int j = 0;
												for (j = 0; j < size; j++)
												{
													if (my_hero->hero.equipments[j] == basic_shoes_menu)
													{
														my_hero->hero.equipments[j] = def_shoes_after_basic_menu;
														basic_shoes_num--;
														break;
													}
												}
												my_hero->hero.equipmentsImages[j * 2]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2] = (Sprite::create("shop/def_shoes1.png"));
												my_hero->hero.equipmentsImages[j * 2]->setPosition(Vec2(origin.x + vecx[j], origin.y + vecy[0]));
												this->addChild(my_hero->hero.equipmentsImages[j * 2], 2);
												my_hero->hero.equipmentsImages[j * 2 + 1]->setVisible(false);
												my_hero->hero.equipmentsImages[j * 2 + 1] = (Sprite::create("shop/def_shoes_after_basic.png"));
												my_hero->hero.equipmentsImages[j * 2 + 1]->setPosition(Vec2(origin.x + 898, origin.y + 250));
												this->addChild(my_hero->hero.equipmentsImages[j * 2 + 1], 2);
												if (basic_shoes_num == 0)
												{
													if (equipments_introduction != NULL)
													{
														equipments_introduction->removeFromParent();
														equipments_introduction = NULL;
														buy_it_menu->removeFromParent();
														buy_it_menu = NULL;
													}
													if (def_shoes_menu != NULL)            //此处用来把打开商店时创建的防御鞋菜单设为不可见，不然指针指向这之后又创建的新防御鞋菜单后原菜单就清除不掉了
													{
														def_shoes_menu->setVisible(false);
													}
													//添加装备防御鞋
													auto item12 = MenuItemImage::create("shop/def_shoes2.png", "shop/def_shoes2.png", [&](Ref* sender)
														{
															auto visibleSize = Director::getInstance()->getVisibleSize();
															Vec2 origin = Director::getInstance()->getVisibleOrigin();
															auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
																{
																	if (my_hero->hero.money > 710 && my_hero->hero.equipments.size() < 6 && my_hero->hero.move_speed == 500)//限制只能为一双鞋
																	{
																		my_hero->hero.money -= 710;
																		money_label->setString(std::to_string(my_hero->hero.money));
																		money_label_shop->setString(std::to_string(my_hero->hero.money));
																		my_hero->hero.move_speed += 120;
																		my_hero->hero.equipments.push_back(def_shoes_menu);
																		int i = my_hero->hero.equipments.size();
																		auto visibleSize = Director::getInstance()->getVisibleSize();
																		Vec2 origin = Director::getInstance()->getVisibleOrigin();
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_shoes1.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
																		my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_shoes2.png"));
																		my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 898, origin.y + 250));
																		this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

																	}
																});
															if (equipments_introduction != NULL)
															{
																equipments_introduction->removeFromParent();
															}
															equipments_introduction = Sprite::create("shop/def_shoes3.png");
															equipments_introduction->setPosition(Vec2(1730, 630));
															this->addChild(equipments_introduction, 2);
															if (buy_it_menu != NULL)
															{
																buy_it_menu->removeFromParent();
															}
															buy_it_menu = Menu::create(item, NULL);
															buy_it_menu->alignItemsVerticallyWithPadding(50);
															item->setPosition(Vec2(origin.x + 750, origin.y - 250));
															this->addChild(buy_it_menu, 2);

														});
													def_shoes_menu = Menu::create(item12, NULL);
													def_shoes_menu->alignItemsVerticallyWithPadding(50);
													//放置防御鞋装备按钮
													item12->setPosition(Vec2(origin.x - 60, origin.y - 290));
													this->addChild(def_shoes_menu, 2);
												}

											}
										});
									if (equipments_introduction != NULL)
									{
										equipments_introduction->removeFromParent();
									}
									equipments_introduction = Sprite::create("shop/def_shoes3.png");
									equipments_introduction->setPosition(Vec2(1730, 630));
									this->addChild(equipments_introduction, 2);
									if (buy_it_menu != NULL)
									{
										buy_it_menu->removeFromParent();
									}
									buy_it_menu = Menu::create(item, NULL);
									buy_it_menu->alignItemsVerticallyWithPadding(50);
									item->setPosition(Vec2(origin.x + 750, origin.y - 250));
									this->addChild(buy_it_menu, 2);

								});
							def_shoes_after_basic_menu = Menu::create(item12, NULL);
							def_shoes_after_basic_menu->alignItemsVerticallyWithPadding(50);
							//放置防御鞋装备按钮
							item12->setPosition(Vec2(origin.x - 60, origin.y - 290));
							this->addChild(def_shoes_after_basic_menu, 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/basic_shoes3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);

			});
		basic_shoes_menu = Menu::create(item9, NULL);
		basic_shoes_menu->alignItemsVerticallyWithPadding(50);
		//放置草鞋装备按钮
		item9->setPosition(Vec2(origin.x - 100, origin.y - 110));
		this->addChild(basic_shoes_menu, 2);

		//添加装备力量腰带   
		auto item10 = MenuItemImage::create("shop/belt_strength.png", "shop/belt_strength.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 900 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 900;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.FullHP += 1000;
							my_hero->hero.HP += 1000;
							my_hero->hero.equipments.push_back(belt_strength_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/belt_strength1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/belt_strength2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1358, origin.y + 430));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/belt_strength3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);

			});
		belt_strength_menu = Menu::create(item10, NULL);
		belt_strength_menu->alignItemsVerticallyWithPadding(50);
		//放置力量腰带装备按钮
		item10->setPosition(Vec2(origin.x + 400, origin.y - 110));
		this->addChild(belt_strength_menu, 2);

		//添加装备大棒 
		auto item11 = MenuItemImage::create("shop/big_stick.png", "shop/big_stick.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 820 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 820;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.ap += 120;
							my_hero->hero.equipments.push_back(big_stick_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_stick1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_stick2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 438, origin.y + 250));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/big_stick3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		big_stick_menu = Menu::create(item11, NULL);
		big_stick_menu->alignItemsVerticallyWithPadding(50);
		//放置大棒装备按钮
		item11->setPosition(Vec2(origin.x - 520, origin.y - 290));
		this->addChild(big_stick_menu, 2);

		//添加装备防御鞋
		auto item12 = MenuItemImage::create("shop/def_shoes.png", "shop/def_shoes.png", [&](Ref* sender)
			{
				auto visibleSize = Director::getInstance()->getVisibleSize();
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 710 && my_hero->hero.equipments.size() < 6 && my_hero->hero.move_speed == 500)
						{
							my_hero->hero.money -= 710;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.move_speed += 120;
							my_hero->hero.equipments.push_back(def_shoes_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_shoes1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_shoes2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 898, origin.y + 250));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/def_shoes3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);

			});
		def_shoes_menu = Menu::create(item12, NULL);
		def_shoes_menu->alignItemsVerticallyWithPadding(50);
		//放置防御鞋装备按钮
		item12->setPosition(Vec2(origin.x - 60, origin.y - 290));
		this->addChild(def_shoes_menu, 2);

		//添加装备防御甲
		auto item13 = MenuItemImage::create("shop/def_armour.png", "shop/def_armour.png", [&](Ref* sender)
			{
				auto visibleSize = Director::getInstance()->getVisibleSize();
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 730 && my_hero->hero.equipments.size() < 6)
						{
							my_hero->hero.money -= 730;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.defect += 210;
							my_hero->hero.armour_effect = true;
							my_hero->hero.equipments.push_back(def_armour_menu);
							int i = my_hero->hero.equipments.size();
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_armour1.png"));
							my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
							my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_armour2.png"));
							my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1358, origin.y + 250));
							this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/def_armour3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);

			});
		def_armour_menu = Menu::create(item13, NULL);
		def_armour_menu->alignItemsVerticallyWithPadding(50);
		//放置防御甲装备按钮
		item13->setPosition(Vec2(origin.x + 400, origin.y - 290));
		this->addChild(def_armour_menu, 2);

	}
	else
	{
		shop_layer->setVisible(true);
		shop_close_menu->setVisible(true);
		money_label_shop->setVisible(true);
		red_diamond_menu->setVisible(true);
		blue_diamond_menu->setVisible(true);
		mp_book_menu->setVisible(true);
		iron_sword_menu->setVisible(true);
		def_clothes_menu->setVisible(true);
		def_ap_cloak_menu->setVisible(true);
		green_diamond_menu->setVisible(true);
		basic_shoes_menu->setVisible(true);
		belt_strength_menu->setVisible(true);
		second_page_menu->setVisible(true);
		if (belt_strength_after_red_menu != NULL)
		{
			belt_strength_after_red_menu->setVisible(true);
		}
		big_stick_menu->setVisible(true);
		if (big_stick_after_book_menu != NULL)
		{
			big_stick_after_book_menu->setVisible(true);
		}
		def_shoes_menu->setVisible(true);
		if (def_shoes_after_basic_menu != NULL)
		{
			def_shoes_after_basic_menu->setVisible(true);
		}
		def_armour_menu->setVisible(true);
		if (def_armour_after_clothes_menu != NULL)
		{
			def_armour_after_clothes_menu->setVisible(true);
		}
		for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
		{
			my_hero->hero.equipmentsImages[2 * i]->setVisible(true);
			if (my_hero->hero.equipments[i] != big_sword_menu)
			{
				my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(true);
			}
		}
	}

}

//关闭商店界面
void MainScene::CloseShop(Ref* pSender)
{
	if (buy_it_menu != NULL)
	{
		buy_it_menu->removeFromParent();
		equipments_introduction->removeFromParent();
		buy_it_menu = NULL;
		equipments_introduction = NULL;
	}
	shop_layer->setVisible(false); //不能删除图层即图层里的元素，这样第二次打开时又是初始状态了
	shop_close_menu->setVisible(false);
	money_label_shop->setVisible(false);
	second_page_menu->setVisible(false);
	red_diamond_menu->setVisible(false);
	blue_diamond_menu->setVisible(false);
	mp_book_menu->setVisible(false);
	iron_sword_menu->setVisible(false);
	def_clothes_menu->setVisible(false);
	def_ap_cloak_menu->setVisible(false);
	green_diamond_menu->setVisible(false);
	basic_shoes_menu->setVisible(false);
	belt_strength_menu->setVisible(false);
	if (belt_strength_after_red_menu != NULL)
	{
		belt_strength_after_red_menu->setVisible(false);
	}
	big_stick_menu->setVisible(false);
	if (big_stick_after_book_menu != NULL)
	{
		big_stick_after_book_menu->setVisible(false);
	}
	def_shoes_menu->setVisible(false);
	if (def_shoes_after_basic_menu != NULL)
	{
		def_shoes_after_basic_menu->setVisible(false);
	}
	def_armour_menu->setVisible(false);
	if (def_armour_after_clothes_menu != NULL)
	{
		def_armour_after_clothes_menu->setVisible(false);
	}
	for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
	{
		my_hero->hero.equipmentsImages[2 * i]->setVisible(false);
		my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(false);
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//添加进入商店游戏按钮  
	auto item1 = MenuItemImage::create("shop/shop.png", "shop/shopselected.png", CC_CALLBACK_1(MainScene::OpenShop, this));
	shop_open_menu = Menu::create(item1, NULL);
	shop_open_menu->alignItemsVerticallyWithPadding(50);
	//按钮放置在屏幕左中间
	item1->setPosition(Vec2(origin.x - 850, origin.y));
	this->addChild(shop_open_menu, 1);
}

//第二页商店回调函数
void MainScene::second_page(Ref* pSender)
{
	//先使第一页全不可见
	if (buy_it_menu != NULL)
	{
		buy_it_menu->removeFromParent();
		equipments_introduction->removeFromParent();
		buy_it_menu = NULL;
		equipments_introduction = NULL;
	}
	shop_close_menu->setVisible(false);
	red_diamond_menu->setVisible(false);
	blue_diamond_menu->setVisible(false);
	mp_book_menu->setVisible(false);
	iron_sword_menu->setVisible(false);
	def_clothes_menu->setVisible(false);
	def_ap_cloak_menu->setVisible(false);
	green_diamond_menu->setVisible(false);
	basic_shoes_menu->setVisible(false);
	belt_strength_menu->setVisible(false);
	if (belt_strength_after_red_menu != NULL)
	{
		belt_strength_after_red_menu->setVisible(false);
	}
	big_stick_menu->setVisible(false);
	if (big_stick_after_book_menu != NULL)
	{
		big_stick_after_book_menu->setVisible(false);
	}
	def_shoes_menu->setVisible(false);
	if (def_shoes_after_basic_menu != NULL)
	{
		def_shoes_after_basic_menu->setVisible(false);
	}
	def_armour_menu->setVisible(false);
	if (def_armour_after_clothes_menu != NULL)
	{
		def_armour_after_clothes_menu->setVisible(false);
	}
	for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
	{
		my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(false);
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (shop_second_layer == NULL)
	{
		//初次载入第二页商店        
		shop_second_layer = Layer::create();
		this->addChild(shop_second_layer, 2);
		shop_second_sprite = Sprite::create("shop/shopopened_second1.png");
		shop_second_layer->addChild(shop_second_sprite, 2);
		shop_second_layer->setAnchorPoint(Vec2(0, 0));
		shop_second_sprite->setAnchorPoint(Vec2(0, 0));
		shop_second_sprite->setPosition(Vec2(0, 207));
		second_page_menu->setVisible(false);



		//添加关闭商店游戏按钮  
		auto item1 = MenuItemImage::create("shop/close.png", "shop/closeselected.png", CC_CALLBACK_1(MainScene::CloseShop_1, this));
		shop_close_menu_1 = Menu::create(item1, NULL);
		shop_close_menu_1->alignItemsVerticallyWithPadding(50);
		//放置关闭商店按钮
		item1->setPosition(Vec2(origin.x + 850, origin.y + 425));
		this->addChild(shop_close_menu_1, 2);

		//添加切换到第一页商店页数按钮
		auto item_1 = MenuItemImage::create("shop/first_page.png", "shop/first_pageselected.png", CC_CALLBACK_1(MainScene::first_page, this));
		first_page_menu = Menu::create(item_1, NULL);
		first_page_menu->alignItemsVerticallyWithPadding(50);
		//放置第一页商店按钮
		item_1->setPosition(Vec2(origin.x - 850, origin.y + 260));
		this->addChild(first_page_menu, 2);
	}
	else
	{
		shop_second_layer->setVisible(true);
		first_page_menu->setVisible(true);
		shop_close_menu_1->setVisible(true);

	}

	if (iron_sword_num != 0)//放置大剑，已购买铁剑的
	{
		if (big_sword_menu != NULL)
		{
			big_sword_menu->setVisible(false);
		}
		//添加装备大剑
		auto item2 = MenuItemImage::create("shop/big_sword_after_iron.png", "shop/big_sword_after_iron.png", [&](Ref* sender)
			{
				auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
					{
						if (my_hero->hero.money > 660)
						{
							++big_sword_num;
							my_hero->hero.money -= 660;
							money_label->setString(std::to_string(my_hero->hero.money));
							money_label_shop->setString(std::to_string(my_hero->hero.money));
							my_hero->hero.attack += 60;
							auto visibleSize = Director::getInstance()->getVisibleSize();
							Vec2 origin = Director::getInstance()->getVisibleOrigin();
							int size = my_hero->hero.equipments.size();
							int j = 0;
							for (j = 0; j < size; j++)
							{
								if (my_hero->hero.equipments[j] == iron_sword_menu)
								{
									my_hero->hero.equipments[j] = big_sword_menu;
									iron_sword_num--;
									break;
								}
							}
							my_hero->hero.equipmentsImages[j * 2]->setVisible(false);
							my_hero->hero.equipmentsImages[j * 2] = (Sprite::create("shop/big_sword1.png"));
							my_hero->hero.equipmentsImages[j * 2]->setPosition(Vec2(origin.x + vecx[j], origin.y + vecy[0]));
							this->addChild(my_hero->hero.equipmentsImages[j * 2], 2);
							my_hero->hero.equipmentsImages[j * 2 + 1] = (Sprite::create("shop/big_sword2.png"));
							my_hero->hero.equipmentsImages[j * 2 + 1]->setPosition(Vec2(origin.x + 423, origin.y + 790));
							this->addChild(my_hero->hero.equipmentsImages[j * 2 + 1], 2);
							if (iron_sword_num == 0)
							{
								if (equipments_introduction != NULL)
								{
									equipments_introduction->removeFromParent();
									equipments_introduction = NULL;
									buy_it_menu->removeFromParent();
									buy_it_menu = NULL;
								}
								if (big_sword_menu != NULL)            //此处用来把打开商店时创建的大剑菜单设为不可见，不然指针指向这之后又创建的新大剑菜单后原菜单就清除不掉了
								{
									big_sword_menu->setVisible(false);
								}
								//添加装备大剑
								auto item2 = MenuItemImage::create("shop/big_sword2.png", "shop/big_sword2.png", [&](Ref* sender)
									{
										auto visibleSize = Director::getInstance()->getVisibleSize();
										Vec2 origin = Director::getInstance()->getVisibleOrigin();
										auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
											{
												if (my_hero->hero.money > 910 && my_hero->hero.equipments.size() < 6)
												{
													++big_sword_num;
													my_hero->hero.money -= 910;
													money_label->setString(std::to_string(my_hero->hero.money));
													money_label_shop->setString(std::to_string(my_hero->hero.money));
													my_hero->hero.attack += 80;
													my_hero->hero.equipments.push_back(big_sword_menu);
													int i = my_hero->hero.equipments.size();
													auto visibleSize = Director::getInstance()->getVisibleSize();
													Vec2 origin = Director::getInstance()->getVisibleOrigin();
													my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword1.png"));
													my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
													this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
													my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword2.png"));
													my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 423, origin.y + 790));
													this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

												}
											});
										if (equipments_introduction != NULL)
										{
											equipments_introduction->removeFromParent();
										}
										equipments_introduction = Sprite::create("shop/big_sword3.png");
										equipments_introduction->setPosition(Vec2(1730, 630));
										this->addChild(equipments_introduction, 2);
										if (buy_it_menu != NULL)
										{
											buy_it_menu->removeFromParent();
										}
										buy_it_menu = Menu::create(item, NULL);
										buy_it_menu->alignItemsVerticallyWithPadding(50);
										item->setPosition(Vec2(origin.x + 750, origin.y - 250));
										this->addChild(buy_it_menu, 2);
									});
								big_sword_menu = Menu::create(item2, NULL);
								big_sword_menu->alignItemsVerticallyWithPadding(50);
								//放置大剑装备按钮
								item2->setPosition(Vec2(origin.x - 500, origin.y + 250));
								this->addChild(big_sword_menu, 2);
							}
						}
					});
				if (equipments_introduction != NULL)
				{
					equipments_introduction->removeFromParent();
				}
				equipments_introduction = Sprite::create("shop/big_sword3.png");
				equipments_introduction->setPosition(Vec2(1730, 630));
				this->addChild(equipments_introduction, 2);
				if (buy_it_menu != NULL)
				{
					buy_it_menu->removeFromParent();
				}
				buy_it_menu = Menu::create(item, NULL);
				buy_it_menu->alignItemsVerticallyWithPadding(50);
				item->setPosition(Vec2(origin.x + 750, origin.y - 250));
				this->addChild(buy_it_menu, 2);
			});
		big_sword_menu = Menu::create(item2, NULL);
		big_sword_menu->alignItemsVerticallyWithPadding(50);
		//放置大剑装备按钮
		item2->setPosition(Vec2(origin.x - 500, origin.y + 250));
		this->addChild(big_sword_menu, 2);
	}
	else	//放置大剑，未购买铁剑的
	{
		if (big_sword_num == 0)
			//添加装备大剑
		{
			auto item2 = MenuItemImage::create("shop/big_sword.png", "shop/big_sword.png", [&](Ref* sender)
				{
					auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
						{
							if (my_hero->hero.money > 910 && my_hero->hero.equipments.size() < 6)
							{
								++big_sword_num;
								my_hero->hero.money -= 910;
								money_label->setString(std::to_string(my_hero->hero.money));
								money_label_shop->setString(std::to_string(my_hero->hero.money));
								my_hero->hero.attack += 80;
								my_hero->hero.equipments.push_back(big_sword_menu);
								int i = my_hero->hero.equipments.size();
								auto visibleSize = Director::getInstance()->getVisibleSize();
								Vec2 origin = Director::getInstance()->getVisibleOrigin();
								my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword1.png"));
								my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
								this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
								my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword2.png"));
								my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 423, origin.y + 790));
								this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

							}
						});
					if (equipments_introduction != NULL)
					{
						equipments_introduction->removeFromParent();
					}
					equipments_introduction = Sprite::create("shop/big_sword3.png");
					equipments_introduction->setPosition(Vec2(1730, 630));
					this->addChild(equipments_introduction, 2);
					if (buy_it_menu != NULL)
					{
						buy_it_menu->removeFromParent();
					}
					buy_it_menu = Menu::create(item, NULL);
					buy_it_menu->alignItemsVerticallyWithPadding(50);
					item->setPosition(Vec2(origin.x + 750, origin.y - 250));
					this->addChild(buy_it_menu, 2);
				});
			big_sword_menu = Menu::create(item2, NULL);
			big_sword_menu->alignItemsVerticallyWithPadding(50);
			//放置大剑装备按钮
			item2->setPosition(Vec2(origin.x - 500, origin.y + 250));
			this->addChild(big_sword_menu, 2);


		}
		else
		{
			auto item2 = MenuItemImage::create("shop/big_sword2.png", "shop/big_sword2.png", [&](Ref* sender)
				{
					auto item = MenuItemImage::create("shop/buy_it.png", "shop/buy_it_selected.png", [&](Ref* sender)
						{
							if (my_hero->hero.money > 910 && my_hero->hero.equipments.size() < 6)
							{
								++big_sword_num;
								my_hero->hero.money -= 910;
								money_label->setString(std::to_string(my_hero->hero.money));
								money_label_shop->setString(std::to_string(my_hero->hero.money));
								my_hero->hero.attack += 80;
								my_hero->hero.equipments.push_back(big_sword_menu);
								int i = my_hero->hero.equipments.size();
								auto visibleSize = Director::getInstance()->getVisibleSize();
								Vec2 origin = Director::getInstance()->getVisibleOrigin();
								my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword1.png"));
								my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
								this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
								my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/big_sword2.png"));
								my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 423, origin.y + 790));
								this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

							}
						});
					if (equipments_introduction != NULL)
					{
						equipments_introduction->removeFromParent();
					}
					equipments_introduction = Sprite::create("shop/big_sword3.png");
					equipments_introduction->setPosition(Vec2(1730, 630));
					this->addChild(equipments_introduction, 2);
					if (buy_it_menu != NULL)
					{
						buy_it_menu->removeFromParent();
					}
					buy_it_menu = Menu::create(item, NULL);
					buy_it_menu->alignItemsVerticallyWithPadding(50);
					item->setPosition(Vec2(origin.x + 750, origin.y - 250));
					this->addChild(buy_it_menu, 2);
				});
			big_sword_menu = Menu::create(item2, NULL);
			big_sword_menu->alignItemsVerticallyWithPadding(50);
			//放置大剑装备按钮
			item2->setPosition(Vec2(origin.x - 500, origin.y + 250));
			this->addChild(big_sword_menu, 2);
		}
	}
}

//第一页商店回调函数
void MainScene::first_page(Ref* pSender)
{
	if (buy_it_menu != NULL)
	{
		buy_it_menu->removeFromParent();
		equipments_introduction->removeFromParent();
		buy_it_menu = NULL;
		equipments_introduction = NULL;
	}
	shop_second_layer->setVisible(false); //不能删除图层即图层里的元素，这样第二次打开时又是初始状态了
	shop_layer->setVisible(false);
	first_page_menu->setVisible(false);
	second_page_menu->setVisible(false);
	shop_close_menu_1->setVisible(false);
	big_sword_menu->setVisible(false);
	for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
	{
		my_hero->hero.equipmentsImages[2 * i]->setVisible(false);
		my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(false);
	}
	shop_layer->setVisible(true);
	shop_close_menu->setVisible(true);
	red_diamond_menu->setVisible(true);
	blue_diamond_menu->setVisible(true);
	mp_book_menu->setVisible(true);
	iron_sword_menu->setVisible(true);
	def_clothes_menu->setVisible(true);
	def_ap_cloak_menu->setVisible(true);
	green_diamond_menu->setVisible(true);
	basic_shoes_menu->setVisible(true);
	belt_strength_menu->setVisible(true);
	second_page_menu->setVisible(true);
	if (belt_strength_after_red_menu != NULL)
	{
		belt_strength_after_red_menu->setVisible(true);
	}
	big_stick_menu->setVisible(true);
	if (big_stick_after_book_menu != NULL)
	{
		big_stick_after_book_menu->setVisible(true);
	}
	def_shoes_menu->setVisible(true);
	if (def_shoes_after_basic_menu != NULL)
	{
		def_shoes_after_basic_menu->setVisible(true);
	}
	def_armour_menu->setVisible(true);
	if (def_armour_after_clothes_menu != NULL)
	{
		def_armour_after_clothes_menu->setVisible(true);
	}
	for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
	{
		my_hero->hero.equipmentsImages[2 * i]->setVisible(true);
		if (my_hero->hero.equipments[i] != big_sword_menu)
		{
			my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(true);
		}
	}
}

//从第二页关闭商店界面    
void MainScene::CloseShop_1(Ref* pSender)
{
	if (buy_it_menu != NULL)
	{
		buy_it_menu->removeFromParent();
		equipments_introduction->removeFromParent();
		buy_it_menu = NULL;
		equipments_introduction = NULL;
	}
	shop_second_layer->setVisible(false); //不能删除图层即图层里的元素，这样第二次打开时又是初始状态了
	shop_layer->setVisible(false);
	money_label_shop->setVisible(false);
	first_page_menu->setVisible(false);
	second_page_menu->setVisible(false);
	shop_close_menu_1->setVisible(false);
	big_sword_menu->setVisible(false);
	for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
	{
		my_hero->hero.equipmentsImages[2 * i]->setVisible(false);
		my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(false);
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//添加进入商店游戏按钮  
	auto item1 = MenuItemImage::create("shop/shop.png", "shop/shopselected.png", CC_CALLBACK_1(MainScene::OpenShop, this));
	shop_open_menu = Menu::create(item1, NULL);
	shop_open_menu->alignItemsVerticallyWithPadding(50);
	//按钮放置在屏幕左中间
	item1->setPosition(Vec2(origin.x - 850, origin.y));
	this->addChild(shop_open_menu, 1);
}

//显示等级
void MainScene::level(float) 
{
	std::string level_text;
	int level = my_hero->hero.level;
	//此次升级所需要的经验
	int level_up_exp = my_hero->hero.level_add_experience[level];
	//经验值满时升级
	if (my_hero->hero.hero_exp >= level_up_exp) 
	{
		my_hero->hero.hero_exp -= level_up_exp;
		my_hero->hero.level++;
		my_hero->hero.HP += my_hero->hero.level_add_HP[level];
		my_hero->hero.attack += my_hero->hero.level_add_attack[level];
	}
	level_text += "LEVEL: ";
	level_text += std::to_string(level);
	level_label->setString(level_text);
}

//显示经验
void MainScene::experience(float) 
{
	std::string exp_text;

	//消灭敌方英雄+80点经验，+100金币
	if (ai_hero->hero.survival == true)
		MainScene::hero_survive = 1;
	if (ai_hero->hero.survival == false && MainScene::hero_survive == 1) 
	{
		my_hero->hero.hero_exp += 80;
		my_hero->hero.money += 100;
		MainScene::hero_survive = 0;
	}
	
	//消灭敌方防御塔或水晶+100点经验,+120金币
	if (towers[2]->tower.survival == true)
		MainScene::tower_survive_1 = 1;
	if (towers[3]->tower.survival == true)
		MainScene::tower_survive_2 = 1;
	if (towers[2]->tower.survival == false && MainScene::tower_survive_1 == 1) 
	{
		my_hero->hero.hero_exp += 100;
		my_hero->hero.money += 120;
		MainScene::tower_survive_1 = 0;
	}
	if (towers[3]->tower.survival == false && MainScene::tower_survive_2 == 1) 
	{
		my_hero->hero.hero_exp += 100;
		my_hero->hero.money += 120;
		MainScene::tower_survive_2 = 0;
	}

	exp_text += "EXP: ";
	exp_text += std::to_string(my_hero->hero.hero_exp);
	exp_text += " / ";
	exp_text += std::to_string(my_hero->hero.level_add_experience[my_hero->hero.level]);
	experience_label->setString(exp_text);
}

//显示金币
void MainScene::money(float)
{
	money_label->setString(std::to_string(my_hero->hero.money));
}

//更新战绩数据
void MainScene::myhero_be_killed(float)
{
	//被击杀数
	if (my_hero->hero.survival == true)
		MainScene::myhero_survive = 1;
	if (my_hero->hero.survival == false && MainScene::myhero_survive == 1) 
	{
		++be_killed_result;
		MainScene::myhero_survive = 0;
	}

	//击杀数
	if (ai_hero->hero.survival == true)
		MainScene::aihero_survive = 1;
	if (ai_hero->hero.survival == false && MainScene::aihero_survive == 1)
	{
		++killed_hero_result;
		MainScene::aihero_survive = 0;
	}

	//摧毁敌方防御塔数
	if (towers[2]->tower.survival == false && towers[3]->tower.survival == false)
	{
		destroyed_tower_result = 2;
	}
	else if (towers[2]->tower.survival == true && towers[3]->tower.survival == true)
	{
		destroyed_tower_result = 0;
	}
	else
	{
		destroyed_tower_result = 1;
	}
}
//创建战绩界面
bool  MainScene::create_CombatResult()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//添加查看战绩游戏按钮  
	auto item1 = MenuItemImage::create("combat_result/check_result.png", "combat_result/check_result_selected.png", CC_CALLBACK_1(MainScene::CheckResult, this));
	open_combat_result = Menu::create(item1, NULL);
	//open_combat_result->alignItemsVerticallyWithPadding(50);
	//按钮放置在屏幕右上角
	item1->setPosition(Vec2(origin.x + 700, origin.y + 200));
	this->addChild(open_combat_result, 1);
	return true;
}
//打开战绩界面
void MainScene::CheckResult(Ref* pSender)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	result_layer = Layer::create();
	this->addChild(result_layer, 2);
	result = Sprite::create("combat_result/result.png");
	result_layer->addChild(result);
	result_layer->setAnchorPoint(Vec2(0, 0));
	result->setAnchorPoint(Vec2(-0.36, -0.36));
	open_combat_result->removeFromParent();

	//添加关闭按钮  
	auto item1 = MenuItemImage::create("combat_result/close.png", "combat_result/close_selected.png", CC_CALLBACK_1(MainScene::CloseResult, this));
	close_combat_result = Menu::create(item1, NULL);
	item1->setPosition(Vec2(origin.x + 468, origin.y + 260));
	this->addChild(close_combat_result, 2);

	//展示英雄属性
	//根据id展示头像
	hero_image = Layer::create();
	result_layer->addChild(hero_image, 1);
	if (id == 0)
		avatar_image = Sprite::create("characters/daji/daji.png");
	else if (id == 1)
		avatar_image = Sprite::create("characters/yase/yase.png");
	else if (id == 2)
		avatar_image = Sprite::create("characters/houyi/houyi.png");
	hero_image->addChild(avatar_image, 1);
	hero_image->setPosition(Vec2(726, 700));

	int hero_level = my_hero->hero.level;
	level_result_label = Label::create(std::to_string(my_hero->hero.level), "Arial", 50);
	level_result_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(level_result_label, 1);
	level_result_label->setPosition(Vec2(726, 564));

	attack_label = Label::create(std::to_string(my_hero->hero.attack), "Arial", 50);
	attack_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(attack_label, 1);
	attack_label->setPosition(Vec2(726, 445));

	std::string HP_text = std::to_string(int(my_hero->hero.HP));//为了避免HP出现多位小数
	HP_label = Label::create(HP_text, "Arial", 50);
	HP_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(HP_label, 1);
	HP_label->setPosition(Vec2(740, 327));

	//展示英雄杀敌数
	killed_hero_label = Label::create(std::to_string(killed_hero_result), "Arial", 50);
	killed_hero_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(killed_hero_label, 1);
	killed_hero_label->setPosition(Vec2(1260, 688));


	killed_soldier_label = Label::create(std::to_string(killed_soldier_result), "Arial", 50);
	killed_soldier_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(killed_soldier_label, 1);
	killed_soldier_label->setPosition(Vec2(1260, 570));


	destroyed_tower_label = Label::create(std::to_string(destroyed_tower_result), "Arial", 50);
	destroyed_tower_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(destroyed_tower_label, 1);
	destroyed_tower_label->setPosition(Vec2(1260, 448));

	be_killed_label = Label::create(std::to_string(be_killed_result), "Arial", 50);
	be_killed_label->setColor(Color3B(255, 255, 255));
	result_layer->addChild(be_killed_label, 1);
	be_killed_label->setPosition(Vec2(1260, 334));

	result_layer->setVisible(true);
	close_combat_result->setVisible(true);
	level_result_label->setVisible(true);
	attack_label->setVisible(true);
	HP_label->setVisible(true);
	killed_hero_label->setVisible(true);
	killed_soldier_label->setVisible(true);
	destroyed_tower_label->setVisible(true);
	be_killed_label->setVisible(true);
}
//关闭战绩界面
void MainScene::CloseResult(Ref* pSender)
{
	result_layer->setVisible(false);
	close_combat_result->setVisible(false);
	result_layer->setVisible(false);
	close_combat_result->setVisible(false);
	level_result_label->setVisible(false);
	attack_label->setVisible(false);
	HP_label->setVisible(false);
	killed_hero_label->setVisible(false);
	killed_soldier_label->setVisible(false);
	destroyed_tower_label->setVisible(false);
	be_killed_label->setVisible(false);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//添加查看战绩游戏按钮  
	auto item1 = MenuItemImage::create("combat_result/check_result.png", "combat_result/check_result_selected.png", CC_CALLBACK_1(MainScene::CheckResult, this));
	open_combat_result = Menu::create(item1, NULL);
	open_combat_result->alignItemsVerticallyWithPadding(50);
	//按钮放置在屏幕右上角
	item1->setPosition(Vec2(origin.x + 700, origin.y + 200));
	this->addChild(open_combat_result, 1);
}
