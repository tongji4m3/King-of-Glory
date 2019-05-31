#include"MainScene.h"
#include"SimpleAudioEngine.h"
#include<iostream>

#include<vector>
#include<string>

//播放音乐要用的
using namespace CocosDenshion;
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

	//读入商店里装备栏的位置参数                     //改↓
	vecx = { 572,749,926,1103,1280,1457 };
	vecy = { 113 };                           //改↑

	//添加商店按钮         //改
	create_shop();         //改

	//玩家
	my_hero = HeroSprite::create(id);
	addChild(my_hero);
	my_hero->setPosition(Vec2(960, 540));
	my_hero->setAnchorPoint(Vec2(0.5, 0.5));

	//添加防御塔
	addTower();

	//载入时间
	time_label = Label::create("00:00", "Arial", 50);
	time_label->setColor(Color3B(200, 150, 156));
	this->addChild(time_label);
	time_label->setPosition(Vec2(960, 960));

	//载入金币                                                                               //改↓
	money_label = Label::create(std::to_string(my_hero->hero.money), "Arial", 50);
	money_label->setColor(Color3B(255, 215, 0));
	this->addChild(money_label);
	money_label->setPosition(Vec2(120,460));                                                         //改↑


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

	//敌人
	int ai_id = MainScene::ai_hero_id;//产生随机数,随机ai英雄    //改
	ai_hero = HeroSprite::create(ai_id);//改
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
					int soldier_id = 2;//初始为2，即炮车     //改↓
					if (i == red_soldier_vec[0])
					{
						soldier_id = 0;
					}
					if (i == red_soldier_vec[1])
					{
						soldier_id = 1;
					}                                        //改↑
					auto hero_attack_effect = AttackEffect::create(id);
					map_layer->addChild(hero_attack_effect);
					hero_attack_effect->setScale(0.5);
					hero_attack_effect->setPosition(my_hero->getPosition() - map_layer_position);
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(i, my_hero->hero.attack,soldier_id);//改
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
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(ai_hero, my_hero->hero.attack);//改
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
							dropBlood(towers[i], my_hero->hero.attack,i);//改
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
		int speed = 800;
		my_hero->runAction(MoveBy::create(touch_position_change.distance(e->getCurrentTarget()->getPosition()) / speed, Vec2(0, touch_position_change.y - my_hero_position_change.y))); 
		map_layer->runAction(MoveBy::create(touch_position_change.distance(e->getCurrentTarget()->getPosition()) / speed, Vec2((my_hero_position_change.x - touch_position_change.x), 0)));
		
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
				int soldier_id = 2;//默认值为2，即炮车                           //改↓
				if ((red_iter - red_soldier_vec.begin()) %3 == 0)
				{
					soldier_id = 0;
				}
				if ((red_iter - red_soldier_vec.begin())%3 == 1)
				{
					soldier_id = 1;
				}                                                                //改↑
				blue_flag = false;
				if ((*blue_iter)->soldier.attack_speed == 100)//因为为每秒刷新，所以就b_num就高一点
				{
					auto tower_attack_effect = AttackEffect::create(3);//小兵攻击特效
					map_layer->addChild(tower_attack_effect);
					tower_attack_effect->setPosition((*blue_iter)->getPosition());
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(*red_iter, (*blue_iter)->soldier.attack,soldier_id);//改
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
							dropBlood(towers[3], (*blue_iter)->soldier.attack,3);//改
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
							dropBlood(towers[2], (*blue_iter)->soldier.attack,2);//改
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
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(ai_hero, (*blue_iter)->soldier.attack);//改
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
			(*blue_iter)->runAction(MoveBy::create(0.02, Vec2(1.5, 0)));
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
				int soldier_id = 2;//默认值为2，即炮车                            //改↓
				if ((blue_iter - blue_soldier_vec.begin())%3== 0)
				{
					soldier_id = 0;
				}
				if ((blue_iter - blue_soldier_vec.begin())%3== 1)
				{
					soldier_id = 1;
				}                                                                 //改↑
				red_flag = false;
				if ((*red_iter)->soldier.attack_speed == 100)//因为为每秒刷新，所以就b_num就高一点
				{
					auto tower_attack_effect = AttackEffect::create(3);//小兵攻击特效
					map_layer->addChild(tower_attack_effect);
					tower_attack_effect->setPosition((*red_iter)->getPosition());
					auto call = CallFuncN::create([=](Node* t)
						{
							dropBlood(*blue_iter, (*red_iter)->soldier.attack,soldier_id);//改
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
						dropBlood(towers[1], (*red_iter)->soldier.attack,1);//改
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
						dropBlood(towers[0], (*red_iter)->soldier.attack,0);//改
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
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood(my_hero, (*red_iter)->soldier.attack);//改
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
			(*red_iter)->runAction(MoveBy::create(0.02, Vec2(-1.5, 0)));
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
		my_hero->hero.HP = my_hero->hero.FullHP;//改
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
		ai_hero->hero.HP = ai_hero->hero.FullHP;//改
		ai_hero->blood->setScaleX(0.5);
		ai_hero->setPosition(Vec2(6000, 700));
		ai_hero->hero.resurrection_time = 10;
	}
	if (ai_hero->hero.survival == false)
	{
		--ai_hero->hero.resurrection_time;
	}
}

void MainScene::tower_attack(float)
{
	if (my_hero->hero.survival == true) //改↓
	{
		reBlood(my_hero);
	}                          
	if (ai_hero->hero.survival == true) 
	{
		reBlood(ai_hero);
	}                                   //改↑
	bool flag0 = true;
	if (towers[0]->tower.survival == true)
	{
		for (auto iter = red_soldier_vec.begin(); iter != red_soldier_vec.end() && flag0 == true; ++iter)
		{
			if ((*iter)->getPosition().distance(towers[0]->getPosition()) < towers[0]->tower.attack_distance && (*iter)->soldier.survival == true)
			{
				int soldier_id = 2;//默认值为2，即炮车                         //改↓
				if ((iter - red_soldier_vec.begin())%3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - red_soldier_vec.begin())%3 == 1)
				{
					soldier_id = 1;
				}                                                              //改↑
				flag0 = false;
				auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[0]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[0]->tower.attack,soldier_id);//改
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
					dropBlood(ai_hero, towers[0]->tower.attack);//改
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
				int soldier_id = 2;//默认值为2，即炮车                             //改↓
				if ((iter - red_soldier_vec.begin())%3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - red_soldier_vec.begin())%3 == 1)
				{
					soldier_id = 1;
				}                                                                 //改↑
				flag1 = false;
				auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[1]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[1]->tower.attack,soldier_id);//改
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
					dropBlood(ai_hero, towers[1]->tower.attack);//改
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
				int soldier_id = 2;//默认值为2，即炮车                  //改↓
				if ((iter - blue_soldier_vec.begin())%3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - blue_soldier_vec.begin())%3 == 1)
				{
					soldier_id = 1;
				}                                                       //改↑
				flag2 = false;
				auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[2]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[2]->tower.attack,soldier_id);//改
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
					dropBlood(my_hero, towers[2]->tower.attack);//改
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
				int soldier_id = 2;//默认值为2，即炮车                          //改↓
				if ((iter - blue_soldier_vec.begin())%3 == 0)
				{
					soldier_id = 0;
				}
				if ((iter - blue_soldier_vec.begin())%3 == 1)
				{
					soldier_id = 1;
				}                                                               //改↑
				flag3 = false;
				auto tower_attack_effect = AttackEffect::create(4);//防御塔攻击特效
				map_layer->addChild(tower_attack_effect);
				tower_attack_effect->setScale(0.5);
				tower_attack_effect->setPosition(towers[3]->getPosition());
				auto call = CallFuncN::create([=](Node* t)
					{
						dropBlood((*iter), towers[3]->tower.attack,soldier_id);//改
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
					dropBlood(my_hero, towers[3]->tower.attack);//改
				});
			tower_attack_effect->runAction(Sequence::create(MoveTo::create(1.2f, my_hero->getPosition() - map_layer->getPosition()), call, FadeOut::create(0.1), 0));
		}
	}
}

void MainScene::gameOver(float)
{
	if (towers[0]->tower.survival == false)
	{
		fail->setVisible(true);
	}
	if (towers[3]->tower.survival == false)
	{
		win->setVisible(true);
	}
}

void MainScene::ai_hero_attack(float)
{
	//控制一次只攻击一个
	bool flag = true;
	//对于所有蓝色兵 且ai需要是存活的
	for (auto iter = blue_soldier_vec.begin(); iter != blue_soldier_vec.end()&& ai_hero->hero.survival == true; ++iter)
	{
		if (ai_hero->getPosition().distance((*iter)->getPosition()) < ai_hero->hero.attack_distance && (*iter)->soldier.survival == true)
		{
			int soldier_id = 2;//默认值为2，即炮车                           //改↓
			if ((iter - blue_soldier_vec.begin())%3 == 0)
			{
				soldier_id = 0;
			}
			if ((iter - blue_soldier_vec.begin())%3 == 1)
			{
				soldier_id = 1;
			}                                                                //改↑
			//攻击效果已绑定英雄
			flag = false;
			auto hero_attack_effect = AttackEffect::create(MainScene::ai_hero_id-3);//改
			map_layer->addChild(hero_attack_effect);
			hero_attack_effect->setScale(0.5);
			hero_attack_effect->setPosition(ai_hero->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood((*iter), ai_hero->hero.attack,soldier_id);//改
				});
			//飞行效果
			hero_attack_effect->runAction(
				Sequence::create(MoveTo::create(1.2f, (*iter)->getPosition()),
					call, FadeOut::create(0.1), 0));
			break;
		}
	}

	//攻击英雄
	if (ai_hero->getPosition().distance(my_hero->getPosition() -map_layer->getPosition()) < ai_hero->hero.attack_distance 
		&& ai_hero->hero.survival == true && my_hero->hero.survival == true && flag==true)
	{
		//攻击效果已绑定英雄
		flag = false;
		auto hero_attack_effect = AttackEffect::create(MainScene::ai_hero_id - 3);//改
		map_layer->addChild(hero_attack_effect);
		hero_attack_effect->setScale(0.5);
		hero_attack_effect->setPosition(ai_hero->getPosition());
		auto call = CallFuncN::create([=](Node* t)
			{
				dropBlood(my_hero, ai_hero->hero.attack);//改
			});
		//飞行效果
		hero_attack_effect->runAction(
			Sequence::create(MoveTo::create(1.2f, my_hero->getPosition() - map_layer->getPosition()),
				call, FadeOut::create(0.1), 0));
	}

	//攻击塔
	for (int i = 0; i <= 1 && ai_hero->hero.survival == true; i++)
	{
		if (ai_hero->getPosition().distance(towers[i]->getPosition()) <ai_hero->hero.attack_distance && towers[i]->tower.survival == true && flag == true)
		{
			flag = false;
			//攻击效果已绑定英雄
			auto hero_attack_effect = AttackEffect::create(MainScene::ai_hero_id - 3);//改
			map_layer->addChild(hero_attack_effect);
			hero_attack_effect->setScale(0.5);
			hero_attack_effect->setPosition(ai_hero->getPosition());
			auto call = CallFuncN::create([=](Node* t)
				{
					dropBlood(towers[i], ai_hero->hero.attack,i);//改
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
	towers[1]->setPosition(Vec2(2550, 550));//改
	towers[2]->setPosition(Vec2(3750, 550));
	towers[3]->setPosition(Vec2(4950, 550));

	//防御塔范围的一个圈
	auto blue_tower_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(blue_tower_bg);
	blue_tower_bg->setPosition(Vec2(2550, 550));
	blue_tower_bg->setColor(Color3B(100, 100, 100));

	auto blue_basement_bg = Sprite::create("atk_range/att_a.png");//改↓
	map_layer->addChild(blue_basement_bg);
	blue_basement_bg->setPosition(Vec2(1550, 550));
	blue_basement_bg->setColor(Color3B(100, 100, 100));           //改↑

	auto red_tower_bg = Sprite::create("atk_range/att_a.png");
	map_layer->addChild(red_tower_bg);
	red_tower_bg->setPosition(Vec2(3750, 550));
	red_tower_bg->setColor(Color3B(100, 100, 100));

	auto red_basement_bg = Sprite::create("atk_range/att_a.png");//改↓
	map_layer->addChild(red_basement_bg);
	red_basement_bg->setPosition(Vec2(4950, 550));
	red_basement_bg->setColor(Color3B(100, 100, 100));           //改↑

}

void MainScene::time(float)
{
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
	time_label->setString(time);//显示时间

	//每一分钟出一波兵
	if (time_second == 1)//近战                           
	{
		auto blue_soldier = SoldierSprite::create(0,time_minute);//改
		map_layer->addChild(blue_soldier);
		blue_soldier_vec.push_back(blue_soldier);
		blue_soldier->setPosition(Vec2(1600, 650));
		//blue_soldier->setPosition(Vec2(2600, 650));

		auto red_soldier = SoldierSprite::create(3,time_minute);//改
		map_layer->addChild(red_soldier);
		red_soldier_vec.push_back(red_soldier);
		red_soldier->setPosition(Vec2(4900, 650));
	}
	if (time_second == 3)//远程                                //改↓
	{
		auto blue_soldier = SoldierSprite::create(1,time_minute);
		map_layer->addChild(blue_soldier);
		blue_soldier_vec.push_back(blue_soldier);
		blue_soldier->setPosition(Vec2(1600, 650));

		auto red_soldier = SoldierSprite::create(4,time_minute);
		map_layer->addChild(red_soldier);
		red_soldier_vec.push_back(red_soldier);
		red_soldier->setPosition(Vec2(4900, 650));
	}                                                          
	if (time_second == 5)//炮车                     
	{
		auto blue_soldier = SoldierSprite::create(2,time_minute);
		map_layer->addChild(blue_soldier);
		blue_soldier_vec.push_back(blue_soldier);
		blue_soldier->setPosition(Vec2(1600, 650));

		auto red_soldier = SoldierSprite::create(5,time_minute);
		map_layer->addChild(red_soldier);
		red_soldier_vec.push_back(red_soldier);
		red_soldier->setPosition(Vec2(4900, 650));
	}                                                          //改↑
	
}

//英雄掉血
void MainScene::dropBlood(HeroSprite* hero, int attack)//改
{
	if (attack > hero->hero.defect)
	{
		hero->hero.HP -= (attack - hero->hero.defect);
	}
	else
	{
		--(hero->hero.HP);
	}
	//还活着，让血条相应变化
	if (hero->hero.HP > 0)
	{
		hero->blood->setScaleX((hero->hero.HP / hero->hero.FullHP) * 0.5);//改
	}
	//死了，血条消失，图片消失，逻辑为死亡
	else
	{
		hero->blood->setScaleX(0);
		hero->setVisible(false);
		hero->hero.survival = false;
	}
}

//英雄自身回血                                              //改↓
void MainScene::reBlood(HeroSprite* hero)
{
	if (hero->hero.HP < hero->hero.FullHP)
	{
		hero->hero.HP += 20;
		if (hero->hero.HP > hero->hero.FullHP)
		{
			hero->hero.HP = hero->hero.FullHP;
		}
		hero->blood->setScaleX((hero->hero.HP / hero->hero.FullHP)*0.5);
	}

}                                                           //改↑

//塔掉血
void MainScene::dropBlood(TowerSprite* tower, int attack,int id)
{
	if ((attack - tower->tower.defect) > 40)//攻击塔伤害下限为40             //改↓
	{
		tower->tower.HP -= (attack - tower->tower.defect);
	}
	else
	{
		tower->tower.HP -= 40;
	}                                                                        //改↑
	//还活着，让血条相应变化
	if (tower->tower.HP > 0)
	{
		tower->blood->setScaleX((tower->tower.HP / TowerSprite::get_tower_hp(id)) * 0.5);//改
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
void MainScene::dropBlood(SoldierSprite* soldier, int attack,int id)//改↓
{
	if (attack > soldier->soldier.defect)
	{
		soldier->soldier.HP -= (attack - soldier->soldier.defect);
	}
	else
	{
		--(soldier->soldier.HP);
	}                                                               //改↑
	//还活着，让血条相应变化
	if (soldier->soldier.HP > 0)
	{
		soldier->blood->setScaleX((soldier->soldier.HP / SoldierSprite::get_soldier_hp(id ,time_minute)) * 0.5);//改
	}
	//死了，血条消失，图片消失，逻辑为死亡
	else
	{
		soldier->blood->setScaleX(0);
		soldier->setVisible(false);
		soldier->soldier.survival = false;
	}
}

//是否能够运动
void MainScene::sport(float)
{
	//超出边界
	if (!map->boundingBox().containsPoint(my_hero->getPosition()))
	{
		my_hero->stopAllActions();
	}
	//我方英雄阵亡
	if (my_hero->hero.survival == false)
	{
		my_hero->stopAllActions();
	}
	ai_hero->hero.can_move = true;
	for (auto iter=blue_soldier_vec.begin();iter!= blue_soldier_vec.end();++iter)
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
		ai_hero->setPosition(ai_hero->getPosition() + Vec2(-7, 0));
	}
}

//创建商店按钮                    //改↓
bool  MainScene::create_shop()
{

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//添加进入商店游戏按钮  
	auto item1 = MenuItemImage::create("shop/shop.png", "shop/shopselected.png", CC_CALLBACK_1(MainScene::OpenShop, this));
	shop_open_menu = Menu::create(item1, NULL);
	shop_open_menu->alignItemsVerticallyWithPadding(50);
	//按钮放置在屏幕左中间
	item1->setPosition(Vec2(origin.x-850, origin.y));
	this->addChild(shop_open_menu, 1);

	return true;
}

//打开商店回调函数
void MainScene::OpenShop(Ref* pSender)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	if (shop_layer == NULL)
	{
		//初次载入商店
		shop_layer = Layer::create();
		this->addChild(shop_layer, 2);
		shop = Sprite::create("shop/shopopened.png");
		shop_layer->addChild(shop);
		shop_layer->setAnchorPoint(Vec2(0, 0));
		shop->setAnchorPoint(Vec2(0, 0));
		shop_open_menu->removeFromParent();//打开商店时先删除打开商店按钮，防止误点导致程序关闭商店时崩溃


		//添加关闭商店游戏按钮  
		auto item1 = MenuItemImage::create("shop/close.png", "shop/closeselected.png", CC_CALLBACK_1(MainScene::CloseShop, this));
		shop_close_menu = Menu::create(item1, NULL);
		shop_close_menu->alignItemsVerticallyWithPadding(50);
		//放置关闭商店按钮
		item1->setPosition(Vec2(origin.x + 850, origin.y + 425));
		this->addChild(shop_close_menu, 2);


		//添加装备红宝石
		auto item2 = MenuItemImage::create("shop/red_diamond.png", "shop/red_diamond.png", [&](Ref* sender)
		{
			if (my_hero->hero.money > 300&&my_hero->hero.equipments.size()<6)
			{
				my_hero->hero.money -= 300;;
				my_hero->hero.FullHP += 300;
				my_hero->hero.HP += 300;
				my_hero->hero.equipments.push_back(red_diamond_menu);
				int i = my_hero->hero.equipments.size();
				auto visibleSize = Director::getInstance()->getVisibleSize();
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/red_diamond1.png"));
				my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i-1], origin.y + vecy[0]));
				this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
				my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/red_diamond2.png"));
				my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 423, origin.y + 790));
				this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

			}
		});
		red_diamond_menu = Menu::create(item2, NULL);
		red_diamond_menu->alignItemsVerticallyWithPadding(50);
		//放置红宝石装备按钮
		item2->setPosition(Vec2(origin.x - 500, origin.y + 250));
		this->addChild(red_diamond_menu, 2);

		//添加装备蓝宝石
		auto item3 = MenuItemImage::create("shop/blue_diamond.png", "shop/blue_diamond.png", [&](Ref* sender)
		{
			if (my_hero->hero.money > 220 && my_hero->hero.equipments.size() < 6)
			{
				my_hero->hero.money -= 220;;
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
				my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 923, origin.y + 790));
				this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

			}
		});
		blue_diamond_menu = Menu::create(item3, NULL);
		blue_diamond_menu->alignItemsVerticallyWithPadding(50);
		//放置蓝宝石装备按钮
		item3->setPosition(Vec2(origin.x, origin.y + 250));
		this->addChild(blue_diamond_menu, 2);

		//添加装备法强书
		auto item4 = MenuItemImage::create("shop/mp_book.png", "shop/mp_book.png", [&](Ref* sender)
		{
			if (my_hero->hero.money > 300 && my_hero->hero.equipments.size() < 6)
			{
				my_hero->hero.money -= 300;;
				my_hero->hero.ap += 40;
				my_hero->hero.equipments.push_back(mp_book_menu);
				int i = my_hero->hero.equipments.size();
				auto visibleSize = Director::getInstance()->getVisibleSize();
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/mp_book1.png"));
				my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
				this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
				my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/mp_book2.png"));
				my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1423, origin.y + 790));
				this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

			}
		});
		mp_book_menu = Menu::create(item4, NULL);
		mp_book_menu->alignItemsVerticallyWithPadding(50);
		//放置法强书装备按钮
		item4->setPosition(Vec2(origin.x + 500, origin.y + 250));
		this->addChild(mp_book_menu, 2);

		//添加装备铁剑
		auto item5 = MenuItemImage::create("shop/iron_sword.png", "shop/iron_sword.png", [&](Ref* sender)
		{
			if (my_hero->hero.money > 250 && my_hero->hero.equipments.size() < 6)
			{
				my_hero->hero.money -= 250;;
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
		iron_sword_menu = Menu::create(item5, NULL);
		iron_sword_menu->alignItemsVerticallyWithPadding(50);
		//放置铁剑装备按钮
		item5->setPosition(Vec2(origin.x - 520, origin.y +70));
		this->addChild(iron_sword_menu, 2);

		//添加装备布衣
		auto item6 = MenuItemImage::create("shop/def_clothes.png", "shop/def_clothes.png", [&](Ref* sender)
		{
			if (my_hero->hero.money > 220 && my_hero->hero.equipments.size() < 6)
			{
				my_hero->hero.money -= 220;;
				my_hero->hero.defect += 60;
				my_hero->hero.equipments.push_back(def_clothes_menu);
				int i = my_hero->hero.equipments.size();
				auto visibleSize = Director::getInstance()->getVisibleSize();
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_clothes1.png"));
				my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
				this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
				my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_clothes2.png"));
				my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 958, origin.y + 610));
				this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

			}
		});
		def_clothes_menu = Menu::create(item6, NULL);
		def_clothes_menu->alignItemsVerticallyWithPadding(50);
		//放置布衣装备按钮
		item6->setPosition(Vec2(origin.x , origin.y + 70));
		this->addChild(def_clothes_menu, 2);

		//添加装备抗魔披风
		auto item7 = MenuItemImage::create("shop/def_ap_cloak.png", "shop/def_ap_cloak.png", [&](Ref* sender)
		{
			if (my_hero->hero.money > 220 && my_hero->hero.equipments.size() < 6)
			{
				my_hero->hero.money -= 220;;
				my_hero->hero.ap_def += 60;
				my_hero->hero.equipments.push_back(def_ap_cloak_menu);
				int i = my_hero->hero.equipments.size();
				auto visibleSize = Director::getInstance()->getVisibleSize();
				Vec2 origin = Director::getInstance()->getVisibleOrigin();
				my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_ap_cloak1.png"));
				my_hero->hero.equipmentsImages[i * 2 - 2]->setPosition(Vec2(origin.x + vecx[i - 1], origin.y + vecy[0]));
				this->addChild(my_hero->hero.equipmentsImages[i * 2 - 2], 2);
				my_hero->hero.equipmentsImages.push_back(Sprite::create("shop/def_ap_cloak2.png"));
				my_hero->hero.equipmentsImages[i * 2 - 1]->setPosition(Vec2(origin.x + 1458, origin.y + 610));
				this->addChild(my_hero->hero.equipmentsImages[i * 2 - 1], 2);

			}
		});
		def_ap_cloak_menu = Menu::create(item7, NULL);
		def_ap_cloak_menu->alignItemsVerticallyWithPadding(50);
		//放置抗魔披风装备按钮
		item7->setPosition(Vec2(origin.x+500, origin.y + 70));
		this->addChild(def_ap_cloak_menu, 2);
	}
	else
	{
		shop_layer->setVisible(true);
		shop_close_menu->setVisible(true);
		red_diamond_menu->setVisible(true);
		blue_diamond_menu->setVisible(true);
		mp_book_menu->setVisible(true);
		iron_sword_menu->setVisible(true);
		def_clothes_menu->setVisible(true);
		def_ap_cloak_menu->setVisible(true);
		for (int i = 0; i < my_hero->hero.equipments.size(); ++i)
		{
			my_hero->hero.equipmentsImages[2 * i]->setVisible(true);
			my_hero->hero.equipmentsImages[2 * i + 1]->setVisible(true);
		}
	}

}
//关闭商店界面
void MainScene::CloseShop(Ref* pSender)
{
	shop_layer->setVisible(false); //不能删除图层即图层里的元素，这样第二次打开时又是初始状态了
	shop_close_menu->setVisible(false);
	red_diamond_menu->setVisible(false);
	blue_diamond_menu->setVisible(false);
	mp_book_menu->setVisible(false);
	iron_sword_menu->setVisible(false);
	def_clothes_menu->setVisible(false);
	def_ap_cloak_menu->setVisible(false);
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
